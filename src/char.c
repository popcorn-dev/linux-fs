#include "fs.h"

po_obj_trait fs_char_trait = po_make_trait (
    fs_char_new    ,
    fs_char_clone  ,
    null_t         ,
    fs_char_del    ,
    sizeof(fs_char),
    null_t
);

po_obj_trait *fs_char_t = &fs_char_trait;

bool_t
    fs_char_new
        (fs_char* self, u32_t count, va_list arg)                                {
            const char *name  = null_t; if (count > 0) name  = va_arg(arg, any_t);
            fs_fops    *ops   = null_t; if (count > 1) ops   = va_arg(arg, any_t);
            fs_dev     *reg   = null_t; if (count > 2) reg   = va_arg(arg, any_t);
            po_class   *class = null_t; if (count > 3) class = va_arg(arg, any_t);

            if (po_trait_of(class) != po_class_t) class = &fs.class;
            if (po_trait_of(reg)   != fs_dev_t)   reg   = &fs.chr;
            dev_t num = ida_alloc(&reg->ida, GFP_KERNEL);
            cdev_init(&self->chr, &ops->fops);

            if (cdev_add(&self->chr, reg->maj + num, 1) < 0) goto err;
            self->dev = po_make (po_dev) from (
                4             ,
                name          ,
                reg->maj + num,
                self          ,
                class
            );

            if (po_trait_of(self->dev) != po_dev_t) goto err;
            self->class = (po_class*) po_ref(class);
            self->ops   = (fs_fops*)  po_ref(ops);
            self->reg   = (fs_dev *)  po_ref(reg);
            self->num   = num;
            return true_t;
    err:    ida_simple_remove(&reg->ida, num);
            cdev_del         (&self->chr)    ;
            return false_t;
}

bool_t
    fs_char_clone
        (fs_char* self, fs_char* clone) {
            return false_t;
}

void
    fs_char_del
        (fs_char* self)                                  {
            ida_simple_remove(&self->reg->ida, self->num);
            cdev_del         (&self->chr)                ;
            po_del (self->class);
            po_del (self->dev);
            po_del (self->ops);
            po_del (self->reg);
}

EXPORT_SYMBOL(fs_char_t);