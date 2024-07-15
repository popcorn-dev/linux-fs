#include "file.h"

static bool_t
    do_new
        (fs_file* self, u32_t count, va_list arg) {
            self->file = null_t;
            self->ops  = null_t;
            self->use  = null_t;
            return true_t;
}

static bool_t
    do_clone
        (fs_file* self, fs_file* clone) {
            return false_t;
}

static void
    do_del
        (fs_file* self)      {
            pp_del(self->ops);
            pp_del(self->use);
}

static pp_obj_trait
    do_obj = pp_make_trait (
        do_new         ,
        do_clone       ,
        null_t         ,
        do_del         ,
        sizeof(fs_file),
        null_t
);

pp_obj_trait *fs_file_t = &do_obj;

#include <linux/module.h>
MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_file_t);