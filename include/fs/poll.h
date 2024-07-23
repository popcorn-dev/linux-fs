#ifndef LINUX_FS_POLL_H
#define LINUX_FS_POLL_H

#include <core.h>

extern pp_obj_trait *fs_poll_t;
typedef struct       fs_poll { u8_t poll[128]; } fs_poll;

void   fs_mask_out_norm(fs_poll*);
void   fs_mask_out     (fs_poll*);

void   fs_mask_in_norm (fs_poll*);
void   fs_mask_in      (fs_poll*);

void   fs_poll_ready   (fs_poll*);

#endif
