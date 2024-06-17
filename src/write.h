#ifndef __PO_FS_WRITE_H__
#define __PO_FS_WRITE_H__

#include <core.h>
#include <collections.h>

#include <core/ua.h>

struct fs_use;

extern po_obj_trait *fs_write_t;
typedef struct       fs_write  {
    po_obj         head;
    u64_t          stat;
    struct fs_use *use;
    po_ua          buf;
    u64_t          ret;
}   fs_write;

bool_t  fs_write_new  (fs_write*, u32_t, va_list);
bool_t  fs_write_clone(fs_write*, fs_write*)     ;
void    fs_write_del  (fs_write*)                ;

bool_t  fs_write_ready(fs_write*, u64_t);
bool_t  fs_write_err  (fs_write*, u64_t);

po_ua*  fs_write_buf  (fs_write*);
u64_t   fs_write_len  (fs_write*);
po_fut* fs_write_fut  (fs_write*);

#endif