#ifndef PO_LINUX_FS_FILE_H
#define PO_LINUX_FS_FILE_H

#include <core.h>
#include <collections.h>

#include <core/wait.h>

struct fs_fops;

extern po_obj_trait *fs_file_t;
typedef struct       fs_file  {
    po_obj          head;
    struct file    *file;
    struct fs_fops *ops;
    u64_t           off;
    po_obj         *use;
}   fs_file;

bool_t fs_file_new  (fs_file*, u32_t, va_list);
bool_t fs_file_clone(fs_file*, fs_file*)      ;
void   fs_file_del  (fs_file*)                ;

#endif