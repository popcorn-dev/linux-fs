#ifndef PO_LINUX_FS_POLL_H
#define PO_LINUX_FS_POLL_H

#include <core.h>
#include <collections.h>

#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>

struct fs_file;

extern po_obj_trait *fs_poll_t;
typedef struct       fs_poll  {
    po_obj            head;
    struct fs_file   *file;
    wait_queue_head_t poll;
    u64_t             ret;
}   fs_poll;

bool_t fs_poll_new     (fs_poll*, u32_t, va_list);
bool_t fs_poll_clone   (fs_poll*, fs_poll*)      ;
void   fs_poll_del     (fs_poll*)                ;

void   fs_mask_out_norm(fs_poll*);
void   fs_mask_out     (fs_poll*);

void   fs_mask_in_norm (fs_poll*);
void   fs_mask_in      (fs_poll*);

void   fs_poll_ready   (fs_poll*);

#endif
