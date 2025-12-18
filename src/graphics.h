#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <stdint.h>

//framebuffer info
typedef struct {
    uint32_t *base;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;  //pixels per scanline
} Framebuffer;

//colors (BGRA format)
#define COLOR_BLACK     0x00000000
#define COLOR_WHITE     0x00FFFFFF
#define COLOR_RED       0x00FF0000
#define COLOR_GREEN     0x0000FF00
#define COLOR_BLUE      0x000000FF
#define COLOR_DARK_GRAY 0x00202020
#define COLOR_GRAY      0x00404040
#define COLOR_LIGHT_GRAY 0x00808080
#define COLOR_CYAN      0x0000FFFF
#define COLOR_YELLOW    0x00FFFF00
#define COLOR_ORANGE    0x00FF8000
#define COLOR_PURPLE    0x008000FF

//menu colors
#define COLOR_BG        0x00000000  //baclk
#define COLOR_BG_LIGHT  0x00000000  //black
#define COLOR_ACCENT    0x00000000  //black
#define COLOR_HIGHLIGHT 0x00AAAAAA  //light gray
#define COLOR_FG_DIM    0x00888888  //Dimmed text

void gfx_init(uint64_t base, uint32_t width, uint32_t height, uint32_t pitch);

Framebuffer *gfx_get_fb(void);

void gfx_clear(uint32_t color);
void gfx_put_pixel(uint32_t x, uint32_t y, uint32_t color);
void gfx_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
void gfx_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

#endif
