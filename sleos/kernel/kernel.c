/* kernel.c — kernel_main entry point | SLeOS
 * Sierra Leone Operating System
 * Limkokwing University of Creative Technology, Freetown
 * COMP 323 — Operating Systems Group Project */

#include "../include/logo.h"
#include "../include/types.h"
#include "../include/vga.h"
#include "../include/kernel.h"
#include "../include/scheduler.h"
#include "../include/memory.h"
#include "../include/fs.h"
#include "../include/shell.h"

#define SLEOS_VERSION "1.0.0-sl"
#define MULTIBOOT_MAGIC 0x2BADB002

static void print_banner(void) {
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  _____ _       ___  ___\n");
    kernel_printf(" / ___/| |     / _ \\/ __|\n");
    kernel_printf(" \\___ \\| |    |  __/ (__\n");
    kernel_printf(" /____/|_|____|\\___|\\___|  OS v%s\n", SLEOS_VERSION);
    kernel_printf("        |_____|\n");
    vga_set_color(VGA_GREEN, VGA_BLACK);
    kernel_printf("\n  Welcome to Sierra Leone OS\n");
    kernel_printf("  Built by Limkokwing Students, Freetown\n");
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  ================================================\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
}

static void print_sysinfo(void) {
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("  Kernel   : SLeOS %s x86_64 (QEMU)\n", SLEOS_VERSION);
    kernel_printf("  Arch     : x86 Protected Mode (32-bit)\n");
    kernel_printf("  Memory   : %u KB total, %u pages x %u KB\n",
                  (uint32_t)128, (uint32_t)64, (uint32_t)2);
    kernel_printf("  Toolchain: GCC/NASM, MIT License\n");
    kernel_printf("  GitHub   : github.com/limkokwing-sl/sleos\n");
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  ================================================\n\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
}

/* kernel_main — called from boot.asm after CPU init */
void kernel_main(uint32_t multiboot_magic, uint32_t multiboot_info) {
    (void)multiboot_info;   /* suppress unused warning */

    /* 1. VGA display */
    vga_init();
    print_banner();

    /* 2. Verify Multiboot magic */
    if (multiboot_magic != MULTIBOOT_MAGIC) {
        kernel_printf("[WARN] Not loaded by a Multiboot bootloader (magic=0x%x)\n",
                      multiboot_magic);
    }
    print_sysinfo();

    /* 3. Memory manager */
    vga_set_color(VGA_CYAN, VGA_BLACK);
    kernel_printf("[ MEM ] Initialising memory manager...\n");
    mem_init();
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("[ MEM ] OK — %u pages free (%u KB)\n\n",
                  mem_free_pages(), mem_free_pages() * 2);

    /* 4. Process scheduler */
    vga_set_color(VGA_CYAN, VGA_BLACK);
    kernel_printf("[ SCH ] Initialising process scheduler...\n");
    sched_init();
    /* Spawn default system processes */
    sched_create("init",       120, 0, SCHED_FCFS);
    sched_create("scheduler",   20, 0, SCHED_ROUNDROBIN);
    sched_create("memmanager",  40, 1, SCHED_ROUNDROBIN);
    sched_create("shell",       80, 2, SCHED_FCFS);
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("[ SCH ] OK — %u processes registered\n\n", sched_count());

    /* 5. File system */
    vga_set_color(VGA_CYAN, VGA_BLACK);
    kernel_printf("[  FS ] Initialising in-memory file system...\n");
    fs_init();
    /* Seed with default files */
    fs_create("readme.txt", FS_FILE);
    fs_write("readme.txt",
        "Welcome to SLeOS!\nBuilt by Limkokwing Students, Sierra Leone.\n"
        "Type 'help' for available commands.\n", 80);
    fs_create("bin", FS_DIR);
    fs_create("etc", FS_DIR);
    fs_create("home", FS_DIR);
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("[  FS ] OK — VFS mounted, 3 dirs + 1 file ready\n\n");

    /* 6. Launch interactive shell */
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  ================================================\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  System ready. Starting shell...\n\n");

    shell_init();
    shell_run();   /* blocks here — main event loop */

    /* Should never reach here */
    kernel_panic("shell_run() returned unexpectedly");
}
