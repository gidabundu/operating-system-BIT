/* klib.c — kernel string/print library | SLeOS */
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/types.h"

size_t kstrlen(const char *s) {
    size_t n = 0; while (*s++) n++; return n;
}
int kstrcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}
int kstrncmp(const char *a, const char *b, size_t n) {
    while (n-- && *a && *a == *b) { a++; b++; }
    return n == (size_t)-1 ? 0 : (unsigned char)*a - (unsigned char)*b;
}
char *kstrcpy(char *dst, const char *src) {
    char *d = dst; while ((*d++ = *src++)); return dst;
}
char *kstrcat(char *dst, const char *src) {
    char *d = dst; while (*d) d++; while ((*d++ = *src++)); return dst;
}
void *kmemset(void *ptr, int val, size_t n) {
    unsigned char *p = ptr; while (n--) *p++ = (unsigned char)val; return ptr;
}
void *kmemcpy(void *dst, const void *src, size_t n) {
    char *d = dst; const char *s = src; while (n--) *d++ = *s++; return dst;
}
int kitoa(int val, char *buf, int base) {
    char tmp[32]; int i = 0, neg = 0;
    if (val == 0) { buf[0]='0'; buf[1]=0; return 1; }
    if (val < 0 && base == 10) { neg = 1; val = -val; }
    unsigned int uval = (unsigned int)val;
    while (uval) {
        int r = uval % base;
        tmp[i++] = (char)(r < 10 ? '0'+r : 'a'+r-10);
        uval /= base;
    }
    int len = 0;
    if (neg) buf[len++] = '-';
    for (int j = i-1; j >= 0; j--) buf[len++] = tmp[j];
    buf[len] = 0;
    return len;
}

/* kernel_printf — supports %s %d %u %x %c %% */
void kernel_printf(const char *fmt, ...) {
    /* va_list via __builtin */
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    char buf[32];
    while (*fmt) {
        if (*fmt != '%') { vga_putchar(*fmt++); continue; }
        fmt++;
        switch (*fmt++) {
        case 's': { const char *s = __builtin_va_arg(args, const char*); vga_puts(s ? s : "(null)"); break; }
        case 'd': { int v = __builtin_va_arg(args, int); kitoa(v, buf, 10); vga_puts(buf); break; }
        case 'u': { unsigned v = __builtin_va_arg(args, unsigned); kitoa((int)v, buf, 10); vga_puts(buf); break; }
        case 'x': { unsigned v = __builtin_va_arg(args, unsigned); kitoa((int)v, buf, 16); vga_puts(buf); break; }
        case 'c': { char c = (char)__builtin_va_arg(args, int); vga_putchar(c); break; }
        case '%': vga_putchar('%'); break;
        default:  vga_putchar('?'); break;
        }
    }
    __builtin_va_end(args);
}

void kernel_panic(const char *msg) {
    vga_set_color(VGA_WHITE, VGA_RED);
    vga_puts("\n\n!!! KERNEL PANIC: ");
    vga_puts(msg);
    vga_puts(" !!!\nSystem halted.\n");
    __asm__ volatile ("cli; hlt");
}
