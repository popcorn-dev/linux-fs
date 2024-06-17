#include "use.h"

po_obj_trait fs_use_trait = po_make_trait (
    fs_use_new    ,
    fs_use_clone  ,
    null_t        ,
    fs_use_del    ,
    sizeof(fs_use),
    null_t
);

po_obj_trait *fs_use_t = &fs_use_trait;

bool_t
    fs_use_new
        (fs_use* self, u32_t count, va_list arg) {
            self->file = null_t;
            self->use  = null_t;
            self->obj  = null_t;
            return true_t;
}

bool_t
    fs_use_clone
        (fs_use* self, fs_use* clone) {
            return false_t;
}

void
    fs_use_del
        (fs_use* self)        {
            po_del(self->file);
            po_del(self->obj) ;
}

#include <linux/module.h>
MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_use_t);