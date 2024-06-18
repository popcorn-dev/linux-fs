#ifndef PO_LINUX_FS_DEV_H
#define PO_LINUX_FS_DEV_H

#include "core.h"
#include "collections.h"

#include <linux/cdev.h>
#include <linux/blkdev.h>
#include <linux/fs.h>

extern po_obj_trait *fs_dev_t;
typedef struct       fs_dev  {
    po_obj     head;
    struct ida ida;
    any_t      dev;
    dev_t      maj;
    u64_t      num;
}   fs_dev;

bool_t fs_dev_new  (fs_dev*, u32_t, va_list);
bool_t fs_dev_clone(fs_dev*, fs_dev*)       ;
void   fs_dev_del  (fs_dev*)                ;

#endif