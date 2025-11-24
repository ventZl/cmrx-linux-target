# Guest OS Virtualization Architecture

## Overview

This document describes the architecture for virtualizing a multi-threaded guest operating system within a single Linux host process. The target guest OS operates on hardware without memory mapping support, using a shared address space with region-based memory protection. The design prioritizes portability, simplicity, and the ability to preemptively control individual guest threads from outside those threads.

## System Requirements

### Target System Characteristics
- Multi-threaded guest OS (non-Linux)
- Shared address space (no MMU/virtual memory)
- Memory protection via region-based access control
- Preemptive thread scheduling required
- Single Linux host process container

### Key Constraints
- Must control individual guest threads externally
- Portable implementation preferred (POSIX only)
- No Linux syscall interception needed
- Simplest possible architecture

## Core Architecture

### Design Choice: Signal-Based Cooperative Preemption with pthread Backend

The architecture uses a **hybrid pthread + signal-based preemption model** that leverages host threading capabilities while providing external scheduling control.

```
┌─────────────────────────────────────────┐
│              Host Linux Process          │
├─────────────────────────────────────────┤
│  Guest OS Scheduler (Main Thread)       │
│  ├─ Thread Queue Management             │
│  ├─ Timer-based Preemption              │
│  └─ Memory Region Management            │
├─────────────────────────────────────────┤
│  Guest Thread Pool (pthread-based)      │
│  ├─ Thread 1: [Stack] [Context]         │
│  ├─ Thread 2: [Stack] [Context]         │
│  └─ Thread N: [Stack] [Context]         │
├─────────────────────────────────────────┤
│  Shared Guest Memory Space               │
│  ├─ Protected Regions (mprotect)        │
│  ├─ Thread-local Storage                │
│  └─ Guest OS Data Structures            │
├─────────────────────────────────────────┤
│  Syscall Interface Layer                │
│  ├─ Thread Management                   │
│  ├─ Memory Management                   │
│  └─ I/O Operations                      │
└─────────────────────────────────────────┘
```

## Thread Control Mechanism

### Guest Thread Structure

```c
typedef struct guest_thread {
    pthread_t host_thread;
    int thread_id;
    ucontext_t saved_context;
    volatile int should_yield;
    volatile int is_running;
    volatile int in_syscall;
    sem_t resume_sem;
    pthread_mutex_t state_lock;
    void *stack_base;
    size_t stack_size;
} guest_thread_t;
```

### Thread Creation

```c
guest_thread_t* create_guest_thread(void (*entry_point)(void*), void *arg) {
    guest_thread_t *gt = calloc(1, sizeof(guest_thread_t));
    
    // Initialize synchronization primitives
    sem_init(&gt->resume_sem, 0, 0);
    pthread_mutex_init(&gt->state_lock, NULL);
    
    // Allocate isolated stack with guard pages
    void *stack = mmap(NULL, GUEST_STACK_SIZE, 
                      PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    mprotect(stack, 4096, PROT_NONE);  // Guard page
    
    // Configure pthread attributes
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstack(&attr, stack + 4096, GUEST_STACK_SIZE - 4096);
    
    // Create suspended pthread
    pthread_create(&gt->host_thread, &attr, guest_thread_wrapper, gt);
    
    return gt;
}
```

### Thread Execution Wrapper

```c
void* guest_thread_wrapper(void *arg) {
    guest_thread_t *gt = (guest_thread_t*)arg;
    
    // Set up preemption signal handler
    signal(SIGUSR1, preemption_signal_handler);
    
    while (true) {
        // Wait for scheduler permission to run
        sem_wait(&gt->resume_sem);
        
        pthread_mutex_lock(&gt->state_lock);
        gt->is_running = 1;
        pthread_mutex_unlock(&gt->state_lock);
        
        // Run guest code with periodic yield checks
        run_guest_code_with_yields(gt);
        
        pthread_mutex_lock(&gt->state_lock);
        gt->is_running = 0;
        pthread_mutex_unlock(&gt->state_lock);
    }
}
```

### Preemption Implementation

```c
// Signal-safe preemption using self-pipe trick
int preemption_pipe[2];

void preemption_signal_handler(int sig) {
    char byte = 1;
    write(preemption_pipe[1], &byte, 1);  // async-signal-safe
}

void scheduler_preempt_thread(guest_thread_t *gt) {
    pthread_mutex_lock(&gt->state_lock);
    if (gt->is_running && !gt->in_syscall) {
        gt->should_yield = 1;
        pthread_kill(gt->host_thread, SIGUSR1);  // Trigger yield check
    }
    pthread_mutex_unlock(&gt->state_lock);
}

void scheduler_resume_thread(guest_thread_t *gt) {
    sem_post(&gt->resume_sem);
}
```

## Memory Protection System

### Memory Region Management

```c
typedef struct memory_region {
    void *base_addr;
    size_t size;
    int permissions;  // R/W/X flags
    int owner_thread; // -1 for shared
} memory_region_t;

typedef struct memory_manager {
    memory_region_t *regions;
    int region_count;
    pthread_rwlock_t regions_lock;
    void *guest_memory_base;
    size_t total_memory_size;
} memory_manager_t;
```

### Protection Interface

```c
int guest_mprotect(void *addr, size_t len, int prot, int thread_id) {
    pthread_rwlock_wrlock(&memory_mgr.regions_lock);
    
    // Find or create region
    memory_region_t *region = find_or_create_region(addr, len);
    region->permissions = prot;
    region->owner_thread = thread_id;
    
    // Apply host-level protection
    int host_prot = 0;
    if (prot & GUEST_PROT_READ) host_prot |= PROT_READ;
    if (prot & GUEST_PROT_WRITE) host_prot |= PROT_WRITE;
    if (prot & GUEST_PROT_EXEC) host_prot |= PROT_EXEC;
    
    int result = mprotect(addr, len, host_prot);
    
    pthread_rwlock_unlock(&memory_mgr.regions_lock);
    return result;
}
```

### Access Violation Handling

```c
void memory_access_handler(int sig, siginfo_t *info, void *context) {
    void *fault_addr = info->si_addr;
    guest_thread_t *current = get_current_guest_thread();
    
    memory_region_t *region = find_region_containing(fault_addr);
    if (!region) {
        // Invalid memory access - terminate thread
        kill_guest_thread(current);
        return;
    }
    
    // Check permissions
    if (region->owner_thread != -1 && 
        region->owner_thread != current->thread_id) {
        // Access violation - signal guest OS
        deliver_guest_exception(current, GUEST_SEGFAULT, fault_addr);
        return;
    }
    
    // Restore access temporarily and single-step
    mprotect(fault_addr, 4096, PROT_READ | PROT_WRITE);
    setup_single_step_trap(current);
}
```

## Guest OS Syscall Interface

### Syscall Table Structure

```c
typedef struct guest_syscall_table {
    guest_syscall_func_t *syscalls;
    int num_syscalls;
    pthread_mutex_t syscall_lock;
} guest_syscall_table_t;

typedef long (*guest_syscall_func_t)(guest_thread_t *caller, 
                                   long arg1, long arg2, long arg3, long arg4);
```

### Syscall Dispatch

```c
long guest_syscall(int syscall_num, long arg1, long arg2, long arg3, long arg4) {
    guest_thread_t *current = get_current_guest_thread();
    
    // Mark thread as in syscall (prevents preemption)
    mark_thread_in_syscall(current);
    
    pthread_mutex_lock(&syscall_table.syscall_lock);
    
    long result = -ENOSYS;
    if (syscall_num < syscall_table.num_syscalls) {
        result = syscall_table.syscalls[syscall_num](
            current, arg1, arg2, arg3, arg4);
    }
    
    pthread_mutex_unlock(&syscall_table.syscall_lock);
    
    mark_thread_not_in_syscall(current);
    check_for_yield(current);  // Check if preemption was requested
    
    return result;
}
```

### Example Syscall Implementations

```c
long guest_sys_thread_create(guest_thread_t *caller, 
                           void *entry, void *arg, long flags, long unused) {
    guest_thread_t *new_thread = create_guest_thread(entry, arg);
    add_thread_to_scheduler(new_thread);
    return new_thread->thread_id;
}

long guest_sys_mprotect(guest_thread_t *caller,
                       void *addr, size_t len, int prot, long unused) {
    return guest_mprotect(addr, len, prot, caller->thread_id);
}

long guest_sys_yield(guest_thread_t *caller, 
                    long unused1, long unused2, long unused3, long unused4) {
    yield_current_thread(caller);
    return 0;
}
```

## Scheduler Implementation

### Main Scheduler Loop

```c
void scheduler_main_loop() {
    while (system_running) {
        // Find next runnable thread (not in syscall)
        guest_thread_t *next = find_runnable_thread_not_in_syscall();
        
        if (current_thread && !current_thread->in_syscall) {
            preempt_thread(current_thread);
        }
        
        if (next) {
            resume_thread(next);
            current_thread = next;
        }
        
        // Time slice
        usleep(TIME_SLICE_MICROSECONDS);
    }
}
```

### Deadlock Prevention

```c
guest_thread_t* find_runnable_thread_not_in_syscall() {
    pthread_mutex_lock(&thread_queue_lock);
    
    for (int i = 0; i < num_threads; i++) {
        guest_thread_t *gt = &guest_threads[i];
        if (gt->state == THREAD_RUNNABLE && !gt->in_syscall) {
            pthread_mutex_unlock(&thread_queue_lock);
            return gt;
        }
    }
    
    pthread_mutex_unlock(&thread_queue_lock);
    return NULL;  // No runnable threads
}
```

## Implementation Architecture

### Component Breakdown

**Core Components:**
1. **`guest_scheduler.c`** - Core scheduling logic and main loop
2. **`guest_threads.c`** - Thread creation, management, and lifecycle
3. **`memory_manager.c`** - Memory protection and region management
4. **`syscall_interface.c`** - Guest OS syscall handlers and dispatch
5. **`signal_handling.c`** - Preemption and fault signal handlers

### Initialization Sequence

```c
int main() {
    // 1. Initialize memory manager
    init_memory_manager(GUEST_MEMORY_SIZE);
    
    // 2. Set up signal handlers for preemption and faults
    setup_signal_handlers();
    
    // 3. Initialize syscall table
    init_syscall_table();
    
    // 4. Create preemption pipe for signal safety
    if (pipe(preemption_pipe) < 0) {
        perror("pipe");
        exit(1);
    }
    
    // 5. Create initial guest thread (guest OS kernel)
    guest_thread_t *kernel_thread = create_guest_thread(guest_os_main, NULL);
    add_thread_to_scheduler(kernel_thread);
    
    // 6. Start scheduler in main thread
    scheduler_main_loop();
    
    // 7. Cleanup when all threads exit
    cleanup_resources();
    
    return 0;
}
```

## Design Verification

### Strengths

**1. Portability:**
- Uses only standard POSIX primitives (pthread, signals, semaphores)
- No ptrace dependency, works on any POSIX system
- No kernel module or special permissions required

**2. Simplicity:**
- Single host process eliminates SKAS complexity
- Direct function calls for syscalls (no interception overhead)
- Shared memory model matches target architecture

**3. Performance:**
- No system call interception overhead
- No process creation/switching overhead
- Minimal context switching (just pthread + semaphore)

### Identified Issues and Solutions

**Issue 1: Signal Safety in Preemption**
- **Problem:** Signal handlers have limited safe operations
- **Solution:** Use self-pipe trick for async-signal-safe communication

**Issue 2: Stack Management**
- **Problem:** Guest threads need isolated stacks
- **Solution:** Manual stack allocation with guard pages per thread

**Issue 3: Deadlock Prevention**
- **Problem:** Scheduler might deadlock with syscalls
- **Solution:** Never preempt threads during syscall execution

**Issue 4: Memory Protection Granularity**
- **Problem:** Host mprotect uses page granularity
- **Solution:** Track sub-page permissions separately if needed

### Trade-offs Accepted

1. **Cooperative elements:** Threads must reach yield points for preemption
2. **Signal complexity:** Requires careful async-signal-safe programming
3. **Stack management:** Manual stack allocation per thread
4. **Memory protection granularity:** Limited to page-level protection

### Alternative Approaches Rejected

1. **Pure ptrace approach:** More complex, less portable
2. **setjmp/longjmp scheduling:** Limited stack switching capability
3. **Process-per-thread:** Violates single address space requirement
4. **ucontext switching:** Deprecated, signal safety issues

## Conclusion

This architecture provides the **most straightforward, portable solution** that meets all requirements while maintaining reasonable performance and implementation complexity. The hybrid approach leverages host threading capabilities while providing the external control necessary for guest OS scheduling.

The design successfully addresses the core challenge of implementing preemptive thread control in userspace by combining:
- **pthread backend** for robust thread management
- **Signal-based preemption** for external control
- **Semaphore coordination** for precise scheduling
- **Function call syscalls** for clean guest OS interface

This creates an elegant virtualization layer that can host a multi-threaded guest OS within a single Linux process while providing the memory protection and preemptive scheduling capabilities required by the target system.