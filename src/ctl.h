#ifndef LINUX_FS_CTL_H
#define LINUX_FS_CTL_H

#include <core.h>
#include <collections.h>

#include <core/wait.h>
#include <core/ua.h>

extern pp_obj_trait *fs_ctl_t;
typedef struct       fs_ctl  {
    pp_obj          head;
    pp_wait        *wait;
    u64_t           code;
    struct fs_file *use;
    any_t           arg;
    i64_t           ret;
}   fs_ctl;

void    fs_ctl_ready  (fs_ctl*, i64_t);
u64_t   fs_ctl_code   (fs_ctl*);
any_t   fs_ctl_arg    (fs_ctl*);

#endif