#ifndef __PO_FS_READ_H__
#define __PO_FS_READ_H__

#include <core.h>
#include <collections.h>

#include <core/ua.h>

struct fs_use;
extern po_obj_trait *fs_read_t;
typedef struct       fs_read  {
    po_obj         head;
    u64_t          stat;
    struct fs_use *use;
    po_ua          buf;
    u64_t          ret;
}   fs_read;

bool_t  fs_read_new  (fs_read*, u32_t, va_list);
bool_t  fs_read_clone(fs_read*, fs_read*)      ;
void    fs_read_del  (fs_read*)                ;

void    fs_read_ready(fs_read*, u64_t);
void    fs_read_err  (fs_read*, u64_t);

po_fut* fs_read_fut  (fs_read*);
u64_t   fs_read_len  (fs_read*);
po_ua*  fs_read_buf  (fs_read*);

#endif