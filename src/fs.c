#include "fs.h"

static bool_t
    do_new
        (struct fs* self, u32_t count, va_list arg)                                                  {
            if (!po_make_at(&self->class, po_class) from (1, "popcorn_fs"))            return false_t;
            if (!po_make_at(&self->chr  , fs_dev)   from (2, "popcorn_fs", fs_char_t)) return false_t;
            return true_t;
}

static bool_t
    do_clone
        (struct fs* self, struct fs* clone) {
            return false_t;
}

static void
    do_del
        (struct fs* self)        {
            po_del (&self->class);
            po_del (&self->chr)  ;
}

po_obj_trait fs_trait = po_make_trait (
    do_new           ,
    do_clone         ,
    null_t           ,
    do_del           ,
    sizeof(struct fs),
    null_t
);

po_obj_trait *fs_t = &fs_trait;