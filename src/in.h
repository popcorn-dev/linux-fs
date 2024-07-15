#ifndef LINUX_FS_IN_H
#define LINUX_FS_IN_H

#include <core.h>
#include <collections.h>

#include <core/wait.h>
#include <core/ua.h>

struct fs_file;
extern pp_obj_trait *fs_in_t;
typedef struct       fs_in  {
    pp_obj          head;
    struct fs_file *file;
    pp_wait        *wait;
    pp_ua           buf;
    i64_t           ret;
}   fs_in;

bool_t  fs_in_ready(fs_in*, void*, i64_t);
bool_t  fs_in_err  (fs_in*, i64_t);
u64_t   fs_in_len  (fs_in*);

#endif