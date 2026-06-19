/* memory.h — memory manager interface */
#ifndef MEMORY_H
#define MEMORY_H
#include "types.h"

#define MEM_TOTAL_KB    128
#define PAGE_SIZE_KB    2
#define PAGE_COUNT      (MEM_TOTAL_KB / PAGE_SIZE_KB)   /* 64 frames */
#define KERNEL_PAGES    4                                /* frames 0-3 reserved */

void     mem_init(void);
uint32_t mem_alloc_page(void);          /* returns frame index, 0xFF=fail */
void     mem_free_page(uint32_t frame);
void     mem_status(void);              /* print map + stats to shell */
uint32_t mem_used_pages(void);
uint32_t mem_free_pages(void);

#endif
