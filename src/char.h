#ifndef PO_LINUX_FS_CHAR_H
#define PO_LINUX_FS_CHAR_H

#include <core.h>
#include <collections.h>
#include <core/class.h>
#include <core/dev.h>

#include <linux/cdev.h>

struct fs_file;
struct fs_char;

extern po_obj_trait *fs_char_t;
typedef struct       fs_char  {
    po_obj          head ;
    po_class       *class;

    struct fs_fops *ops;
    struct fs_dev  *reg;
    po_dev         *dev;

    struct cdev     chr;
    dev_t           num;
}   fs_char;

bool_t fs_char_new  (fs_char*, u32_t, va_list);
bool_t fs_char_clone(fs_char*, fs_char*)      ;
void   fs_char_del  (fs_char*)                ;

#endif
