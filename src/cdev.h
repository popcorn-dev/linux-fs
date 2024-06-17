#ifndef __PO_FS_CHR_CHR_H__
#define __PO_FS_CHR_CHR_H__

#include "core.h"
#include "collections.h"
#include <linux/cdev.h>

struct po_class;
struct fs_file;
struct fs_char;

extern po_obj_trait *fs_cdev_t;
typedef struct       fs_cdev  {
    po_obj           head ;
    struct po_class *class;
    struct fs_file  *file;
    struct fs_char  *type;
    struct device   *dev;
    struct cdev      chr;
    dev_t            num;
}   fs_cdev;

bool_t po_chr_new  (fs_cdev*, u32_t, va_list);
bool_t po_chr_clone(fs_cdev*, fs_cdev*)      ;
void   po_chr_del  (fs_cdev*)                ;

#endif
