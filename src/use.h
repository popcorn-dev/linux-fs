#ifndef __PO_FS_FILE_H__
#define __PO_FS_FILE_H__

#include <core.h>
#include <collections.h>

struct fs_file;

extern po_obj_trait *fs_use_t;
typedef struct       fs_use  {
    po_obj          head;
    struct fs_file *file;
    struct file    *use;
    u64_t           off;
    po_obj         *obj;
}   fs_use;

bool_t fs_use_new  (fs_use*, u32_t, va_list);
bool_t fs_use_clone(fs_use*, fs_use*)       ;
void   fs_use_del  (fs_use*)                ;

#endif