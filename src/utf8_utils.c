#include <stddef.h>
#include "utf8_utils.h"

size_t get_visual_width(const char *str)
{
    size_t width = 0;
    while (*str) {
        if ((*str & 0xC0) != 0x80) {
            if (*str & 0x80) {
                width += 2;
            } else {
                width += 1;
            }
        }
        str++;
    }
    return width;
}