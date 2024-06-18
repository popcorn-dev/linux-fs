#ifndef PO_LINUX_FS_FOPS_H
#define PO_LINUX_FS_FOPS_H

#include <core.h>
#include <core/wait.h>

struct fs_poll;
struct fs_ctl;
struct fs_out;
struct fs_in;

typedef struct fs_fdo                               {
    void (*ctl) (po_obj*, struct fs_ctl *, po_wait*);
    void (*out) (po_obj*, struct fs_out *, po_wait*);
    void (*in)  (po_obj*, struct fs_in  *, po_wait*);
    void (*poll)(po_obj*, struct fs_poll*);
}   fs_fdo;

#define fs_make_fdo(do_ctl, do_out, do_in, do_poll)        {\
    .ctl  = ((void (*)(po_obj*, struct fs_out*)) (do_ctl)) ,\
    .out  = ((void (*)(po_obj*, struct fs_out*)) (do_out)) ,\
    .in   = ((void (*)(po_obj*, struct fs_in*))  (do_in))  ,\
    .poll = ((void (*)(po_obj*, struct fs_poll*))(do_poll)) \
}

extern po_obj_trait *fs_fops_t;
typedef struct       fs_fops { u8_t fops[512]; } fs_fops;

#endif
