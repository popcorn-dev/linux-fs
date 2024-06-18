#ifndef PO_LINUX_FS_CTL_H
#define PO_LINUX_FS_CTL_H

#include <core.h>
#include <core/ua.h>

extern po_obj_trait *fs_ctl_t;
typedef struct       fs_ctl { u8_t ctl[128]; } fs_ctl;

void    fs_ctl_ready  (fs_ctl*, i64_t);
u64_t   fs_ctl_code   (fs_ctl*);

any_t   fs_ctl_arg_ka (fs_ctl*, u64_t, po_mem*);
po_ua*  fs_ctl_arg_ua (fs_ctl*, u64_t);
any_t   fs_ctl_arg    (fs_ctl*);

#endif
