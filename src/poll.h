#ifndef LINUX_FS_POLL_H
#define LINUX_FS_POLL_H

#include <core.h>
#include <collections.h>

#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>

struct fs_file;

extern pp_obj_trait *fs_poll_t;
typedef struct       fs_poll  {
    pp_obj            head;
    struct fs_file   *file;
    wait_queue_head_t poll;
    u64_t             ret;
}   fs_poll;

void   fs_mask_out_norm(fs_poll*);
void   fs_mask_out     (fs_poll*);

void   fs_mask_in_norm (fs_poll*);
void   fs_mask_in      (fs_poll*);

void   fs_poll_ready   (fs_poll*);

#endif
