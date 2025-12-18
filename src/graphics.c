#include "graphics.h"

static Framebuffer fb;

void gfx_init(uint64_t base, uint32_t width, uint32_t height, uint32_t pitch) {
    fb.base = (uint32_t *)base;
    fb.width = width;
    fb.height = height;
    fb.pitch = pitch;
}

Framebuffer *gfx_get_fb(void) {
    return &fb;
}

void gfx_clear(uint32_t color) {
    for (uint32_t y = 0; y < fb.height; y++) {
        for (uint32_t x = 0; x < fb.width; x++) {
            fb.base[y * fb.pitch + x] = color;
        }
    }
}

void gfx_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x < fb.width && y < fb.height) {
        fb.base[y * fb.pitch + x] = color;
    }
}

void gfx_fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t py = y; py < y + h && py < fb.height; py++) {
        for (uint32_t px = x; px < x + w && px < fb.width; px++) {
            fb.base[py * fb.pitch + px] = color;
        }
    }
}

void gfx_draw_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    //top and boottom
    for (uint32_t px = x; px < x + w && px < fb.width; px++) {
        if (y < fb.height) fb.base[y * fb.pitch + px] = color;
        if (y + h - 1 < fb.height) fb.base[(y + h - 1) * fb.pitch + px] = color;
    }
    //left and right
    for (uint32_t py = y; py < y + h && py < fb.height; py++) {
        if (x < fb.width) fb.base[py * fb.pitch + x] = color;
        if (x + w - 1 < fb.width) fb.base[py * fb.pitch + x + w - 1] = color;
    }
}
