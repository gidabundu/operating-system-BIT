/* fs.c — in-memory file system (VFS) | SLeOS */
#include "../include/fs.h"
#include "../include/kernel.h"
#include "../include/vga.h"
#include "../include/types.h"

static inode_t inode_table[FS_MAX_FILES];

void fs_init(void) {
    kmemset(inode_table, 0, sizeof(inode_table));
}

static inode_t *fs_find(const char *name) {
    for (int i = 0; i < FS_MAX_FILES; i++)
        if (inode_table[i].used && kstrcmp(inode_table[i].name, name) == 0)
            return &inode_table[i];
    return NULL;
}

int fs_create(const char *name, fs_type_t type) {
    if (fs_find(name)) return -1;  /* already exists */
    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (!inode_table[i].used) {
            kmemset(&inode_table[i], 0, sizeof(inode_t));
            inode_table[i].used = 1;
            inode_table[i].type = type;
            kstrcpy(inode_table[i].name, name);
            inode_table[i].size = 0;
            return 0;
        }
    }
    return -2;  /* table full */
}

int fs_write(const char *name, const char *data, uint32_t len) {
    inode_t *node = fs_find(name);
    if (!node || node->type != FS_FILE) return -1;
    if (len >= FS_MAX_DATA) len = FS_MAX_DATA - 1;
    kmemcpy(node->data, data, len);
    node->data[len] = 0;
    node->size = len;
    return 0;
}

int fs_read(const char *name, char *buf, uint32_t buflen) {
    inode_t *node = fs_find(name);
    if (!node || node->type != FS_FILE) return -1;
    uint32_t n = node->size < buflen-1 ? node->size : buflen-1;
    kmemcpy(buf, node->data, n);
    buf[n] = 0;
    return (int)n;
}

int fs_delete(const char *name) {
    inode_t *node = fs_find(name);
    if (!node) return -1;
    kmemset(node, 0, sizeof(inode_t));
    return 0;
}

void fs_ls(void) {
    int count = 0;
    vga_set_color(VGA_CYAN, VGA_BLACK);
    kernel_printf("  NAME                TYPE   SIZE\n");
    vga_set_color(VGA_DARK_GREY, VGA_BLACK);
    kernel_printf("  ------------------  -----  ----\n");
    for (int i = 0; i < FS_MAX_FILES; i++) {
        if (!inode_table[i].used) continue;
        if (inode_table[i].type == FS_DIR) {
            vga_set_color(VGA_LIGHT_BLUE, VGA_BLACK);
            kernel_printf("  %-18s  dir    -\n", inode_table[i].name);
        } else {
            vga_set_color(VGA_LIGHT_GREEN, VGA_BLACK);
            kernel_printf("  %-18s  file   %u B\n",
                          inode_table[i].name, inode_table[i].size);
        }
        count++;
    }
    if (!count) {
        vga_set_color(VGA_DARK_GREY, VGA_BLACK);
        kernel_printf("  (empty)\n");
    }
    vga_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    kernel_printf("  %d item(s)\n", count);
}
