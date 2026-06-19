/* memory.c — page-frame memory manager | SLeOS
 * 128 KB total RAM, 64 frames of 2 KB each.
 * Bitmap: 0=free, 1=used. First KERNEL_PAGES frames reserved for kernel. */
#include "../include/memory.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/types.h"

static uint8_t  bitmap[PAGE_COUNT];   /* 1 byte per frame */
static uint32_t used_count = 0;

void mem_init(void) {
    kmemset(bitmap, 0, sizeof(bitmap));
    /* Reserve kernel frames */
    for (uint32_t i = 0; i < KERNEL_PAGES; i++) {
        bitmap[i] = 1;
    }
    used_count = KERNEL_PAGES;
}

uint32_t mem_alloc_page(void) {
    for (uint32_t i = KERNEL_PAGES; i < PAGE_COUNT; i++) {
        if (!bitmap[i]) {
            bitmap[i] = 1;
            used_count++;
            return i;
        }
    }
    return 0xFF;  /* out of memory */
}

void mem_free_page(uint32_t frame) {
    if (frame >= KERNEL_PAGES && frame < PAGE_COUNT && bitmap[frame]) {
        bitmap[frame] = 0;
        used_count--;
    }
}

uint32_t mem_used_pages(void) { return used_count; }
uint32_t mem_free_pages(void) { return PAGE_COUNT - used_count; }

void mem_status(void) {
    vga_set_color(VGA_CYAN, VGA_BLACK);
    kernel_printf("  Memory Map (%u frames x %u KB = %u KB total)\n",
                  (uint32_t)PAGE_COUNT, (uint32_t)PAGE_SIZE_KB, (uint32_t)MEM_TOTAL_KB);
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  [K]=kernel  [#]=used  [.]=free\n\n  ");

    for (uint32_t i = 0; i < PAGE_COUNT; i++) {
        if (i > 0 && i % 16 == 0) kernel_printf("\n  ");
        if (i < KERNEL_PAGES) {
            vga_set_color(VGA_LIGHT_BLUE, VGA_BLACK);
            kernel_printf("K ");
        } else if (bitmap[i]) {
            vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
            kernel_printf("# ");
        } else {
            vga_set_color(VGA_DARK_GREY, VGA_BLACK);
            kernel_printf(". ");
        }
    }
    kernel_printf("\n\n");

    uint32_t free_kb  = mem_free_pages()  * PAGE_SIZE_KB;
    uint32_t used_kb  = mem_used_pages()  * PAGE_SIZE_KB;
    uint32_t total_kb = MEM_TOTAL_KB;

    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("  Total : %u KB  |  Used : %u KB  |  Free : %u KB\n",
                  total_kb, used_kb, free_kb);
    kernel_printf("  Frames: %u total | %u kernel | %u used | %u free\n",
                  (uint32_t)PAGE_COUNT, (uint32_t)KERNEL_PAGES,
                  used_count - KERNEL_PAGES, mem_free_pages());
}
