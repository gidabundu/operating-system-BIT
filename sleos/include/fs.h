/* fs.h — in-memory file system interface */
#ifndef FS_H
#define FS_H
#include "types.h"

#define FS_MAX_FILES    64
#define FS_MAX_NAME     32
#define FS_MAX_DATA     512

typedef enum { FS_FILE = 0, FS_DIR } fs_type_t;

typedef struct {
    uint8_t     used;
    fs_type_t   type;
    char        name[FS_MAX_NAME];
    uint32_t    size;
    char        data[FS_MAX_DATA];
} inode_t;

void fs_init(void);
int  fs_create(const char *name, fs_type_t type);
int  fs_write(const char *name, const char *data, uint32_t len);
int  fs_read(const char *name, char *buf, uint32_t buflen);
int  fs_delete(const char *name);
void fs_ls(void);

#endif
