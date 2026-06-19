/* kernel.h — core kernel interface */
#ifndef KERNEL_H
#define KERNEL_H
#include "types.h"

void kernel_printf(const char *fmt, ...);
void kernel_panic(const char *msg);

/* kstring helpers */
size_t kstrlen(const char *s);
int    kstrcmp(const char *a, const char *b);
int    kstrncmp(const char *a, const char *b, size_t n);
char  *kstrcpy(char *dst, const char *src);
char  *kstrcat(char *dst, const char *src);
void  *kmemset(void *ptr, int val, size_t n);
void  *kmemcpy(void *dst, const void *src, size_t n);
int    kitoa(int val, char *buf, int base);

#endif
