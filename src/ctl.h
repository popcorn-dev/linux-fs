#ifndef PO_LINUX_FS_CTL_H
#define PO_LINUX_FS_CTL_H

#include <core.h>
#include <collections.h>
#include <core/ua.h>

extern po_obj_trait *fs_ctl_t;
typedef struct       fs_ctl  {
    po_obj          head;
    u64_t           code;
    struct fs_file *use;
    any_t           arg;
    i64_t           ret;
}   fs_ctl;

bool_t  fs_ctl_new    (fs_ctl*, u32_t, va_list);
bool_t  fs_ctl_clone  (fs_ctl*, fs_ctl*)       ;
void    fs_ctl_del    (fs_ctl*)                ;

void    fs_ctl_ready  (fs_ctl*, i64_t);
u64_t   fs_ctl_code   (fs_ctl*);

any_t   fs_ctl_arg_ka (fs_ctl*, u64_t, po_mem*);
po_ua*  fs_ctl_arg_ua (fs_ctl*, u64_t);
any_t   fs_ctl_arg    (fs_ctl*);

#endif