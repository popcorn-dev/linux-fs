#ifndef LINUX_FS_DEV_H
#define LINUX_FS_DEV_H

#include "core.h"
#include "collections.h"

#include <linux/cdev.h>
#include <linux/blkdev.h>
#include <linux/fs.h>

extern pp_obj_trait *fs_dev_t;
typedef struct       fs_dev  {
    pp_obj     head;
    struct ida ida;
    any_t      dev;
    dev_t      maj;
    u64_t      num;
}   fs_dev;

#endif