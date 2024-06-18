#include "poll.h"
#include "file.h"

po_obj_trait fs_poll_trait = po_make_trait (
    fs_poll_new    ,
    fs_poll_clone  ,
    null_t         ,
    fs_poll_del    ,
    sizeof(fs_poll),
    null_t
);

po_obj_trait *fs_poll_t = &fs_poll_trait;

bool_t
    fs_poll_new
        (fs_poll* self, u32_t count, va_list arg)                           {
            fs_file *file = null_t; if (count > 0) file = va_arg(arg, any_t);
            if (po_trait_of(file) != fs_file_t) return false_t;
            init_waitqueue_head(&self->poll);

            self->file = (fs_file*) po_ref(file);
            self->ret  = 0;
            return true_t;
}

bool_t
    fs_poll_clone
        (fs_poll* self, fs_poll* clone) {
            return false_t;
}

void
    fs_poll_del
        (fs_poll* self)        {
            po_del (self->file);
}

void   fs_mask_out_norm(fs_poll* self) { if (po_trait_of(self) != fs_poll_t) return; self->ret |= POLLOUT; }
void   fs_mask_out     (fs_poll* self) { if (po_trait_of(self) != fs_poll_t) return; self->ret |= POLLIN ; }

void   fs_mask_in_norm (fs_poll* self) { if (po_trait_of(self) != fs_poll_t) return; self->ret |= POLLRDNORM; }
void   fs_mask_in      (fs_poll* self) { if (po_trait_of(self) != fs_poll_t) return; self->ret |= POLLWRNORM; }

void
    fs_poll_ready
        (fs_poll* self)                               {
            if (po_trait_of(self) != fs_poll_t) return;
            wake_up_interruptible(&self->poll);
}

#include <linux/module.h>

MODULE_LICENSE("GPL");

EXPORT_SYMBOL(fs_mask_out_norm);
EXPORT_SYMBOL(fs_mask_out);
EXPORT_SYMBOL(fs_mask_in_norm);
EXPORT_SYMBOL(fs_mask_in);
EXPORT_SYMBOL(fs_poll_ready);
EXPORT_SYMBOL(fs_poll_t);