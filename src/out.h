#ifndef __PO_FS_OUT_H__
#define __PO_FS_OUT_H__

#include <core.h>
#include <collections.h>

#include <core/wait.h>
#include <core/ua.h>

struct fs_file;

extern po_obj_trait *fs_out_t;
typedef struct       fs_out  {
    po_obj          head;
    struct fs_file *file;
    po_wait        *wait;
    po_ua           buf;
    i64_t           ret;
}   fs_out;

bool_t  fs_out_new  (fs_out*, u32_t, va_list);
bool_t  fs_out_clone(fs_out*, fs_out*)       ;
void    fs_out_del  (fs_out*)                ;

bool_t  fs_out_ready(fs_out*, void*, i64_t);
bool_t  fs_out_err  (fs_out*, i64_t);
u64_t   fs_out_len  (fs_out*);

#endif