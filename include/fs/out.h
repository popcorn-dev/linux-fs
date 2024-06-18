#ifndef PO_LINUX_FS_OUT_H
#define PO_LINUX_FS_OUT_H

#include <core.h>

extern po_obj_trait *fs_out_t;
typedef struct       fs_out { u8_t out[256]; } fs_out;

bool_t  fs_out_ready(fs_out*, void*, i64_t);
bool_t  fs_out_err  (fs_out*, i64_t);
u64_t   fs_out_len  (fs_out*);

#endif
