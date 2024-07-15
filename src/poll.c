#include "poll.h"
#include "file.h"

static bool_t
    do_new
        (fs_poll* self, u32_t count, va_list arg)                           {
            fs_file *file = null_t; if (count > 0) file = va_arg(arg, any_t);
            if (pp_trait_of(file) != fs_file_t) return false_t;
            init_waitqueue_head(&self->poll);

            self->file = (fs_file*) pp_ref(file);
            self->ret  = 0;
            return true_t;
}

static bool_t
    do_clone
        (fs_poll* self, fs_poll* clone) {
            return false_t;
}

static void
    do_del
        (fs_poll* self)        {
            pp_del (self->file);
}

static pp_obj_trait
    do_obj = pp_make_trait (
        do_new         ,
        do_clone       ,
        null_t         ,
        do_del         ,
        sizeof(fs_poll),
        null_t
);

pp_obj_trait *fs_poll_t = &do_obj;

void   fs_mask_out_norm(fs_poll* self) { if (pp_trait_of(self) != fs_poll_t) return; self->ret |= POLLOUT; }
void   fs_mask_out     (fs_poll* self) { if (pp_trait_of(self) != fs_poll_t) return; self->ret |= POLLIN ; }

void   fs_mask_in_norm (fs_poll* self) { if (pp_trait_of(self) != fs_poll_t) return; self->ret |= POLLRDNORM; }
void   fs_mask_in      (fs_poll* self) { if (pp_trait_of(self) != fs_poll_t) return; self->ret |= POLLWRNORM; }

void
    fs_poll_ready
        (fs_poll* self)                               {
            if (pp_trait_of(self) != fs_poll_t) return;
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