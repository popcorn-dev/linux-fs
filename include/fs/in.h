#ifndef LINUX_FS_IN_H
#define LINUX_FS_IN_H

#include <core.h>

extern pp_obj_trait *fs_in_t;
typedef struct       fs_in { u8_t in[256]; } fs_in;

bool_t fs_in_ready(fs_in*, void*, i64_t);
bool_t fs_in_err  (fs_in*, i64_t);
u64_t  fs_in_len  (fs_in*);

#endif
