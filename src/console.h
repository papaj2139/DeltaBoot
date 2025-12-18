#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdint.h>

//PSF1 font header
#define PSF1_MAGIC0     0x36
#define PSF1_MAGIC1     0x04
#define PSF1_MODE512    0x01
#define PSF1_MODEHASTAB 0x02
#define PSF1_MODEHASSEQ 0x04

typedef struct {
    uint8_t magic[2];
    uint8_t mode;
    uint8_t charsize;  //height in pixels (width is always 8)
} PSF1Header;

//PSF2 font header
#define PSF2_MAGIC0     0x72
#define PSF2_MAGIC1     0xb5
#define PSF2_MAGIC2     0x4a
#define PSF2_MAGIC3     0x86

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
} PSF2Header;

void con_init(void);
void con_set_color(uint32_t fg, uint32_t bg);
void con_clear(void);
void con_putchar(char c);
void con_print(const char *str);
void con_print_at(uint32_t x, uint32_t y, const char *str);
void con_draw_char(uint32_t x, uint32_t y, char c, uint32_t fg, uint32_t bg);
uint32_t con_get_char_width(void);
uint32_t con_get_char_height(void);

#endif
