#ifndef PO_LINUX_FS_IN_H
#define PO_LINUX_FS_IN_H

#include <core.h>
#include <collections.h>

#include <core/wait.h>
#include <core/ua.h>

struct fs_file;
extern po_obj_trait *fs_in_t;
typedef struct       fs_in  {
    po_obj          head;
    struct fs_file *file;
    po_wait        *wait;
    po_ua           buf;
    i64_t           ret;
}   fs_in;

bool_t  fs_in_new  (fs_in*, u32_t, va_list);
bool_t  fs_in_clone(fs_in*, fs_in*)        ;
void    fs_in_del  (fs_in*)                ;

bool_t  fs_in_ready(fs_in*, void*, i64_t);
bool_t  fs_in_err  (fs_in*, i64_t);
u64_t   fs_in_len  (fs_in*);

#endif