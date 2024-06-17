#ifndef __PO_FS_CHR_CHR_TYPE_H__
#define __PO_FS_CHR_CHR_TYPE_H__

#include "core.h"
#include "collections.h"
#include <linux/cdev.h>
#include <linux/fs.h>

struct po_file_type;
extern po_obj_trait *fs_char_t;
typedef struct       fs_char  {
    po_obj     head;
    struct ida ida;
    dev_t      maj;
    u64_t      num;
}   fs_char;

bool_t fs_char_new  (fs_char*, u32_t, va_list);
bool_t fs_char_clone(fs_char*, fs_char*)      ;
void   fs_char_del  (fs_char*)                ;

#endif