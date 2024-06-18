#ifndef PO_LINUX_FS_FOPS_H
#define PO_LINUX_FS_FOPS_H

#include <core.h>
#include <collections.h>

#include <core/wait.h>
#include <core/ua.h>
#include <linux/fs.h>

struct fs_poll;
struct fs_ctl;
struct fs_out;
struct fs_in;

typedef struct fs_fdo                     {
    void (*poll)(po_obj*, struct fs_poll*);
    void (*ctl) (po_obj*, struct fs_ctl *);
    void (*out) (po_obj*, struct fs_out *);
    void (*in)  (po_obj*, struct fs_in  *);
}   fs_fdo;

#define fs_make_fdo(do_ctl, do_out, do_in, do_poll)        {\
    .ctl  = ((void (*)(po_obj*, struct fs_ctl*)) (do_ctl)) ,\
    .out  = ((void (*)(po_obj*, struct fs_out*)) (do_out)) ,\
    .in   = ((void (*)(po_obj*, struct fs_in*))  (do_in))  ,\
    .poll = ((void (*)(po_obj*, struct fs_poll*))(do_poll)) \
}

extern po_obj_trait *fs_fops_t;
typedef struct       fs_fops   {
    po_obj                 head;
    struct file_operations fops;
    po_obj_trait          *use;
    fs_fdo                *fdo;
}   fs_fops;

bool_t fs_fops_new  (fs_fops*, u32_t, va_list);
bool_t fs_fops_clone(fs_fops*, fs_fops*)      ;
void   fs_fops_del  (fs_fops*)                ;

#endif
