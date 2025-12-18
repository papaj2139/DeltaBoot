#include "console.h"
#include "graphics.h"
#include "font.h"

//console state
static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;
static uint32_t fg_color = COLOR_WHITE;
static uint32_t bg_color = COLOR_BLACK;
static const uint32_t CHAR_WIDTH = 8;
static const uint32_t CHAR_HEIGHT = 16;

void con_init(void) {
    cursor_x = 0;
    cursor_y = 0;
    fg_color = COLOR_WHITE;
    bg_color = 0;  //transparent
}

void con_set_color(uint32_t fg, uint32_t bg) {
    fg_color = fg;
    bg_color = bg;
}

void con_clear(void) {
    cursor_x = 0;
    cursor_y = 0;
    gfx_clear(bg_color ? bg_color : COLOR_BLACK);
}

void con_draw_char(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg) {
    if ((uint8_t)c >= 128) return;
    
    const uint8_t *glyph = &font[(uint8_t)c * 16];
    
    for (uint32_t py = 0; py < CHAR_HEIGHT; py++) {
        uint8_t row = glyph[py];
        for (uint32_t px = 0; px < CHAR_WIDTH; px++) {
            if (row & (0x80 >> px)) {
                gfx_put_pixel(x + px, y + py, fg);
            } else if (bg != 0) {
                gfx_put_pixel(x + px, y + py, bg);
            }
        }
    }
}

void con_putchar(char c) {
    Framebuffer *fb = gfx_get_fb();
    uint32_t cols = fb->width / CHAR_WIDTH;
    uint32_t rows = fb->height / CHAR_HEIGHT;
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 4) & ~3;
    } else {
        con_draw_char(cursor_x * CHAR_WIDTH, cursor_y * CHAR_HEIGHT, c, fg_color, bg_color);
        cursor_x++;
    }
    
    if (cursor_x >= cols) {
        cursor_x = 0;
        cursor_y++;
    }
    
    //scroll will be here laters
    if (cursor_y >= rows) {
        cursor_y = rows - 1;
    }
}

void con_print(const char *str) {
    while (*str) {
        con_putchar(*str++);
    }
}

void con_print_at(uint32_t x, uint32_t y, const char *str) {
    while (*str) {
        con_draw_char(x, y, *str, fg_color, bg_color);
        x += CHAR_WIDTH;
        str++;
    }
}

uint32_t con_get_char_width(void) { return CHAR_WIDTH; }
uint32_t con_get_char_height(void) { return CHAR_HEIGHT; }
