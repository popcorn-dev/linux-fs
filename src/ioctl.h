#ifndef __FS_IOCTL_H__
#define __FS_IOCTL_H__

#include <core.h>
#include <collections.h>
#include <core/ua.h>

struct fs_use;

extern po_obj_trait *fs_ioctl_t;
typedef struct       fs_ioctl  {
    po_obj         head;
    u64_t          stat;
    u64_t          code;
    struct fs_use *use;
    any_t          arg;
    u64_t          ret;
}   fs_ioctl;

bool_t  fs_ioctl_new  (fs_ioctl*, u32_t, va_list);
bool_t  fs_ioctl_clone(fs_ioctl*, fs_ioctl*)     ;
void    fs_ioctl_del  (fs_ioctl*);

void    fs_ioctl_ready(fs_ioctl*, u64_t);
u64_t   fs_ioctl_code (fs_ioctl*);
po_fut* fs_ioctl_fut  (fs_ioctl*);

po_ua*  fs_ioctl_arg  (fs_ioctl*)       ;
po_ua*  fs_ioctl_ua   (fs_ioctl*, u64_t);

#endif