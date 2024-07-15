#ifndef LINUX_FS_CHAR_H
#define LINUX_FS_CHAR_H

#include <core.h>
#include <collections.h>
#include <core/class.h>
#include <core/dev.h>

#include <linux/cdev.h>

struct fs_file;
struct fs_char;

extern pp_obj_trait *fs_char_t;
typedef struct       fs_char  {
    pp_obj          head ;
    pp_class       *class;

    struct fs_fops *ops;
    struct fs_dev  *reg;
    pp_dev         *dev;

    struct cdev     chr;
    dev_t           num;
}   fs_char;

#endif
