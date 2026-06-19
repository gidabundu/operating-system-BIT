/* vga.c — VGA text-mode 80x25 driver | SLeOS */
#include "../include/vga.h"
#include "../include/types.h"

#define VGA_MEMORY 0xB8000
#define VGA_CMD    0x3D4
#define VGA_DATA   0x3D5

static uint16_t *vga_buf  = (uint16_t *)VGA_MEMORY;
static uint8_t   vga_row  = 0;
static uint8_t   vga_col  = 0;
static uint8_t   vga_attr = 0;

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}
static uint16_t make_entry(char c, uint8_t attr) {
    return (uint16_t)c | ((uint16_t)attr << 8);
}
void vga_set_color(vga_color_t fg, vga_color_t bg) {
    vga_attr = (uint8_t)((bg << 4) | fg);
}
static void update_cursor(void) {
    uint16_t pos = (uint16_t)(vga_row * VGA_WIDTH + vga_col);
    outb(VGA_CMD, 0x0F); outb(VGA_DATA, (uint8_t)(pos & 0xFF));
    outb(VGA_CMD, 0x0E); outb(VGA_DATA, (uint8_t)((pos >> 8) & 0xFF));
}
void vga_clear(void) {
    uint16_t blank = make_entry(' ', vga_attr);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) vga_buf[i] = blank;
    vga_row = 0; vga_col = 0; update_cursor();
}
void vga_init(void) {
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    vga_clear();
}
static void scroll(void) {
    uint16_t blank = make_entry(' ', vga_attr);
    for (int r = 1; r < VGA_HEIGHT; r++)
        for (int c = 0; c < VGA_WIDTH; c++)
            vga_buf[(r-1)*VGA_WIDTH+c] = vga_buf[r*VGA_WIDTH+c];
    for (int c = 0; c < VGA_WIDTH; c++)
        vga_buf[(VGA_HEIGHT-1)*VGA_WIDTH+c] = blank;
    vga_row = VGA_HEIGHT - 1;
}
void vga_putchar(char c) {
    if (c == '\n') { vga_col = 0; if (++vga_row >= VGA_HEIGHT) scroll(); }
    else if (c == '\r') { vga_col = 0; }
    else if (c == '\b') {
        if (vga_col > 0) { vga_col--; vga_buf[vga_row*VGA_WIDTH+vga_col]=make_entry(' ',vga_attr); }
    } else {
        vga_buf[vga_row*VGA_WIDTH+vga_col] = make_entry(c, vga_attr);
        if (++vga_col >= VGA_WIDTH) { vga_col=0; if (++vga_row>=VGA_HEIGHT) scroll(); }
    }
    update_cursor();
}
void vga_puts(const char *s) { while (*s) vga_putchar(*s++); }
void vga_set_cursor(uint8_t row, uint8_t col) { vga_row=row; vga_col=col; update_cursor(); }
