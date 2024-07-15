#ifndef LINUX_FS_OUT_H
#define LINUX_FS_OUT_H

#include <core.h>
#include <collections.h>

#include <core/wait.h>
#include <core/ua.h>

struct fs_file;

extern pp_obj_trait *fs_out_t;
typedef struct       fs_out  {
    pp_obj          head;
    struct fs_file *file;
    pp_wait        *wait;
    pp_ua           buf;
    i64_t           ret;
}   fs_out;

bool_t  fs_out_ready(fs_out*, void*, i64_t);
bool_t  fs_out_err  (fs_out*, i64_t);
u64_t   fs_out_len  (fs_out*);

#endif