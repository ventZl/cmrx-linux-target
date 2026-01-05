#include "xpm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>


typedef struct {
    char *chars;
    uint32_t rgba;
} ColorEntry;

/**
 * Parse a hex color string (#RGB or #RRGGBB) to RGBA value
 * Returns RGBA as 0xAABBGGRR (little-endian RGBA)
 */
static uint32_t parse_hex_color(const char *color_str) {
    if (color_str[0] != '#') {
        assert(0);
        return 0x000000FF; // Default to black with full alpha
    }

    unsigned long hex_val = strtoul(color_str + 1, NULL, 16);
    uint8_t r, g, b, a = 0xFF;

    size_t len = strlen(color_str + 1);

    if (len == 3) {
        // #RGB format - expand to #RRGGBB
        r = ((hex_val >> 8) & 0xF) * 17;
        g = ((hex_val >> 4) & 0xF) * 17;
        b = (hex_val & 0xF) * 17;
    } else if (len == 6) {
        // #RRGGBB format
        r = (hex_val >> 16) & 0xFF;
        g = (hex_val >> 8) & 0xFF;
        b = hex_val & 0xFF;
    } else {
        assert(0);
        // Unknown format, return black
        return 0x000000FF;
    }

    // Pack as RGBA in little-endian (ABGR in memory order)
    return (r << 24) | (g << 16) | (b << 8) | a;
}

/**
 * Extract color value from XPM color definition line
 * Format: "chars c color" or "chars c #RRGGBB"
 */
static uint32_t extract_color(const char *color_def) {
    const char *c_pos = strstr(color_def, " c ");
    if (!c_pos) {
        c_pos = strstr(color_def, "\tc ");
        if (!c_pos) {
            assert(0);
            return 0x000000FF; // Default black
        }
    }

    c_pos += 3; // Skip " c "

    // Skip whitespace
    while (*c_pos && isspace(*c_pos)) {
        c_pos++;
    }

    // Check for "None" (transparent)
    if (strncmp(c_pos, "None", 4) == 0) {
        return 0x00000000; // Transparent
    }

    // Parse hex color
    if (*c_pos == '#') {
        return parse_hex_color(c_pos);
    }

    assert(0);
    // For named colors, just return black (could extend with color name lookup)
    return 0x000000FF;
}

/**
 * Convert XPM format to Pixmap structure
 *
 * @param xpm Array of strings in XPM format:
 *            xpm[0]: "width height num_colors chars_per_pixel"
 *            xpm[1..num_colors]: color definitions
 *            xpm[num_colors+1..]: pixel data rows
 * @return Pixmap structure with RGBA data, or {0, 0, NULL} on error
 */
struct Pixmap xpm_to_pixmap(const char **xpm) {
    struct Pixmap result = {{0, 0}, NULL};

    if (!xpm || !xpm[0]) {
        return result;
    }

    // Parse header: "width height num_colors chars_per_pixel"
    unsigned width, height, num_colors, chars_per_pixel;
    if (sscanf(xpm[0], "%u %u %u %u", &width, &height, &num_colors, &chars_per_pixel) != 4) {
        fprintf(stderr, "Error: Invalid XPM header\n");
        return result;
    }

    // Allocate color map
    ColorEntry *color_map = calloc(num_colors, sizeof(ColorEntry));
    if (!color_map) {
        fprintf(stderr, "Error: Failed to allocate color map\n");
        return result;
    }

    // Parse color definitions
    for (unsigned i = 0; i < num_colors; i++) {
        const char *line = xpm[1 + i];

        // Extract character(s) identifying this color
        color_map[i].chars = strndup(line, chars_per_pixel);

        // Extract RGBA color value
        color_map[i].rgba = extract_color(line);
    }

    // Allocate pixel data
    result.dim.width = width;
    result.dim.height = height;
    result.data = calloc(width * height, sizeof(uint32_t));

    if (!result.data) {
        fprintf(stderr, "Error: Failed to allocate pixel data\n");
        for (unsigned i = 0; i < num_colors; i++) {
            free(color_map[i].chars);
        }
        free(color_map);
        result.dim.width = 0;
        result.dim.height = 0;
        return result;
    }

    // Parse pixel data
    for (unsigned y = 0; y < height; y++) {
        const char *row = xpm[1 + num_colors + y];

        for (unsigned x = 0; x < width; x++) {
            const char *pixel_chars = row + (x * chars_per_pixel);

            // Find matching color in color map
            uint32_t pixel_color = 0xFF000000; // Default black
            for (unsigned c = 0; c < num_colors; c++) {
                if (strncmp(pixel_chars, color_map[c].chars, chars_per_pixel) == 0) {
                    pixel_color = color_map[c].rgba;
                    break;
                }
            }

            result.data[y * width + x] = pixel_color;
        }
    }

    // Clean up color map
    for (unsigned i = 0; i < num_colors; i++) {
        free(color_map[i].chars);
    }
    free(color_map);

    return result;
}

/**
 * Free Pixmap data
 */
void pixmap_free(struct Pixmap *pixmap) {
    if (pixmap && pixmap->data) {
        free(pixmap->data);
        pixmap->data = NULL;
        pixmap->dim.width = 0;
        pixmap->dim.height = 0;
    }
}

/**
 * Print Pixmap as hex values (for debugging)
 */
void pixmap_print(const struct Pixmap *pixmap) {
    printf("Pixmap: %ux%u\n", pixmap->dim.width, pixmap->dim.height);
    for (unsigned y = 0; y < pixmap->dim.height; y++) {
        for (unsigned x = 0; x < pixmap->dim.width; x++) {
            uint32_t pixel = pixmap->data[y * pixmap->dim.width + x];
            printf("%08X ", pixel);
        }
        printf("\n");
    }
}
