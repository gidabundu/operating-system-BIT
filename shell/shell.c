/* shell.c — interactive keyboard shell | SLeOS */
#include "../include/shell.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/scheduler.h"
#include "../include/memory.h"
#include "../include/fs.h"
#include "../include/types.h"

/* PS/2 keyboard I/O ports */
#define KB_DATA   0x60
#define KB_STATUS 0x64

static char   cmd_buf[SHELL_BUF_SIZE];
static int    cmd_len = 0;

/* Minimal US QWERTY scancode -> ASCII (set 1, unshifted) */
static const char sc_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/',
    0,  '*', 0, ' ', 0,
    0,0,0,0,0,0,0,0,0,0,  /* F1-F10 */
    0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.', 0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static char kb_getchar(void) {
    /* Poll keyboard until a valid ASCII key is pressed */
    while (1) {
        if (inb(KB_STATUS) & 1) {
            uint8_t sc = inb(KB_DATA);
            if (sc & 0x80) continue;   /* key release, ignore */
            if (sc < 128) {
                char c = sc_to_ascii[sc];
                if (c) return c;
            }
        }
    }
}

static void print_prompt(void) {
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("sleos@root:~$ ");
    vga_set_color(VGA_WHITE, VGA_BLACK);
}

/* ── command: help ──────────────────────────────────────────────────────── */
static void cmd_help(void) {
    vga_set_color(VGA_CYAN, VGA_BLACK);
    kernel_printf("\n  SLeOS Shell — Available Commands\n");
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  ================================\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  ps               ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("List all processes with state and CPU usage\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  mem              ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Show memory map and allocation statistics\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  ls               ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("List files in the virtual file system\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  cat <file>       ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Print contents of a file\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  touch <file>     ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Create a new empty file\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  write <f> <data> ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Write data to a file\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  rm <file>        ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Delete a file\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  mkdir <dir>      ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Create a new directory\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  alloc            ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Allocate one memory page\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  free <frame>     ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Free a memory page by frame number\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  uname            ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Print OS and system information\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  clear            ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Clear the screen\n");
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  help             ");
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("Show this help message\n\n");
}

/* ── command: uname ─────────────────────────────────────────────────────── */
static void cmd_uname(void) {
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
    kernel_printf("  SLeOS 1.0.0-sl x86_64 QEMU\n");
    kernel_printf("  Sierra Leone Operating System\n");
    kernel_printf("  Built by Limkokwing Students, Freetown, Sierra Leone\n");
}

/* ── parse and dispatch a command line ──────────────────────────────────── */
static void dispatch(char *line) {
    /* tokenise: split on spaces */
    char *argv[8];
    int   argc = 0;
    char *p    = line;

    while (*p && argc < 8) {
        while (*p == ' ') p++;
        if (!*p) break;
        argv[argc++] = p;
        while (*p && *p != ' ') p++;
        if (*p) *p++ = 0;
    }
    if (argc == 0) return;

    char *cmd = argv[0];

    if (kstrcmp(cmd, "ps") == 0) {
        sched_tick();
        kernel_printf("\n");
        sched_list();
        kernel_printf("\n");

    } else if (kstrcmp(cmd, "mem") == 0) {
        kernel_printf("\n");
        mem_status();
        kernel_printf("\n");

    } else if (kstrcmp(cmd, "ls") == 0) {
        kernel_printf("\n");
        fs_ls();
        kernel_printf("\n");

    } else if (kstrcmp(cmd, "cat") == 0) {
        if (argc < 2) {
            vga_set_color(VGA_LIGHT_BROWN, VGA_BLACK);
            kernel_printf("  Usage: cat <filename>\n");
        } else {
            char buf[FS_MAX_DATA];
            int n = fs_read(argv[1], buf, sizeof(buf));
            if (n < 0) {
                vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
                kernel_printf("  cat: %s: file not found\n", argv[1]);
            } else {
                vga_set_color(VGA_WHITE, VGA_BLACK);
                kernel_printf("\n%s\n", buf);
            }
        }

    } else if (kstrcmp(cmd, "touch") == 0) {
        if (argc < 2) {
            vga_set_color(VGA_LIGHT_BROWN, VGA_BLACK);
            kernel_printf("  Usage: touch <filename>\n");
        } else {
            int r = fs_create(argv[1], FS_FILE);
            if (r == 0) {
                vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
                kernel_printf("  Created: %s\n", argv[1]);
            } else {
                vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
                kernel_printf("  touch: %s: already exists or table full\n", argv[1]);
            }
        }

    } else if (kstrcmp(cmd, "write") == 0) {
        if (argc < 3) {
            vga_set_color(VGA_LIGHT_BROWN, VGA_BLACK);
            kernel_printf("  Usage: write <filename> <data>\n");
        } else {
            /* argv[2] onwards is the data — rejoin with spaces */
            /* argv[2] already points into the buffer; just use it */
            int r = fs_write(argv[1], argv[2], kstrlen(argv[2]));
            if (r == 0) {
                vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
                kernel_printf("  Written %u bytes to %s\n",
                              (uint32_t)kstrlen(argv[2]), argv[1]);
            } else {
                vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
                kernel_printf("  write: %s: not found or is a directory\n", argv[1]);
            }
        }

    } else if (kstrcmp(cmd, "rm") == 0) {
        if (argc < 2) {
            vga_set_color(VGA_LIGHT_BROWN, VGA_BLACK);
            kernel_printf("  Usage: rm <filename>\n");
        } else {
            int r = fs_delete(argv[1]);
            vga_set_color(r == 0 ? VGA_LIGHT_GREEN : VGA_LIGHT_RED, VGA_BLACK);
            kernel_printf(r == 0 ? "  Deleted: %s\n" : "  rm: %s: not found\n", argv[1]);
        }

    } else if (kstrcmp(cmd, "mkdir") == 0) {
        if (argc < 2) {
            vga_set_color(VGA_LIGHT_BROWN, VGA_BLACK);
            kernel_printf("  Usage: mkdir <dirname>\n");
        } else {
            int r = fs_create(argv[1], FS_DIR);
            vga_set_color(r == 0 ? VGA_LIGHT_GREEN : VGA_LIGHT_RED, VGA_BLACK);
            kernel_printf(r == 0 ? "  Directory created: %s\n"
                                 : "  mkdir: %s: already exists\n", argv[1]);
        }

    } else if (kstrcmp(cmd, "alloc") == 0) {
        uint32_t frame = mem_alloc_page();
        if (frame == 0xFF) {
            vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
            kernel_printf("  alloc: out of memory\n");
        } else {
            vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
            kernel_printf("  Allocated frame %u (addr 0x%x)\n",
                          frame, frame * PAGE_SIZE_KB * 1024);
        }

    } else if (kstrcmp(cmd, "free") == 0) {
        if (argc < 2) {
            vga_set_color(VGA_LIGHT_BROWN, VGA_BLACK);
            kernel_printf("  Usage: free <frame_number>\n");
        } else {
            /* simple atoi */
            uint32_t n = 0;
            for (char *c = argv[1]; *c >= '0' && *c <= '9'; c++)
                n = n * 10 + (*c - '0');
            mem_free_page(n);
            vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
            kernel_printf("  Frame %u freed\n", n);
        }

    } else if (kstrcmp(cmd, "uname") == 0) {
        kernel_printf("\n");
        cmd_uname();
        kernel_printf("\n");

    } else if (kstrcmp(cmd, "clear") == 0) {
        vga_clear();

    } else if (kstrcmp(cmd, "help") == 0) {
        cmd_help();

    } else {
        vga_set_color(VGA_LIGHT_RED, VGA_BLACK);
        kernel_printf("  -sh: %s: command not found  (type 'help' for commands)\n", cmd);
    }
    vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
}

void shell_init(void) {
    kmemset(cmd_buf, 0, sizeof(cmd_buf));
    cmd_len = 0;
}

void shell_run(void) {
    cmd_help();
    print_prompt();

    while (1) {
        char c = kb_getchar();

        if (c == '\n') {
            /* newline — execute */
            kernel_printf("\n");
            cmd_buf[cmd_len] = 0;
            if (cmd_len > 0) {
                dispatch(cmd_buf);
            }
            cmd_len = 0;
            kmemset(cmd_buf, 0, sizeof(cmd_buf));
            print_prompt();

        } else if (c == '\b') {
            /* backspace */
            if (cmd_len > 0) {
                cmd_len--;
                vga_putchar('\b');
            }
        } else if (cmd_len < SHELL_BUF_SIZE - 1) {
            cmd_buf[cmd_len++] = c;
            vga_putchar(c);
        }
    }
}
