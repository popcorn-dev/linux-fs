#include "file.h"

po_obj_trait fs_file_trait = po_make_trait (
    fs_file_new    ,
    fs_file_clone  ,
    null_t        ,
    fs_file_del    ,
    sizeof(fs_file),
    null_t
);

po_obj_trait *fs_file_t = &fs_file_trait;

bool_t
    fs_file_new
        (fs_file* self, u32_t count, va_list arg) {
            self->file = null_t;
            self->ops  = null_t;
            self->use  = null_t;
            return true_t;
}

bool_t
    fs_file_clone
        (fs_file* self, fs_file* clone) {
            return false_t;
}

void
    fs_file_del
        (fs_file* self)      {
            po_del(self->ops);
            po_del(self->use);
}

#include <linux/module.h>
MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_file_t);