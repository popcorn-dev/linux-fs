#ifndef LINUX_FS_FILE_H
#define LINUX_FS_FILE_H

#include <core.h>
#include <collections.h>

#include <core/wait.h>

struct fs_fops;

extern pp_obj_trait *fs_file_t;
typedef struct       fs_file  {
    pp_obj          head;
    struct file    *file;
    struct fs_fops *ops;
    u64_t           off;
    pp_obj         *use;
}   fs_file;

#endif