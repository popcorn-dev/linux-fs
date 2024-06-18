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
            po_class   *class = null_t; if (count > 3) class = va_arg(arg, any_t);
            const char *name  = null_t; if (count > 0) name  = va_arg(arg, any_t);
            fs_fops    *ops   = null_t; if (count > 1) ops   = va_arg(arg, any_t);
            fs_dev     *reg   = null_t; if (count > 2) reg   = va_arg(arg, any_t);
            if (po_trait_of(&fs) != fs_t) return false_t;

            if (po_trait_of(class) != po_class_t) class = &fs.class;
            if (po_trait_of(reg)   != fs_dev_t)   reg   = &fs.chr;
            dev_t num = ida_alloc(&reg->ida, GFP_KERNEL);
            cdev_init(&self->chr, ops->fdo);

            if (cdev_add(&self->chr, reg->maj + num, 1) < 0) return false_t;
            self->dev = device_create (
                &class->class ,
                null_t        ,
                reg->maj + num,
                self          ,
                name
            );

            self->class = (po_class*) po_ref(class);
            self->ops   = (fs_fops*)  po_ref(ops);
            self->reg   = (fs_dev *)  po_ref(reg);
            self->num   = num;
            return true_t;
}

bool_t
    fs_char_clone
        (fs_char* self, fs_char* clone) {
            return false_t;
}

void
    fs_char_del
        (fs_char* self)                                                       {
            device_destroy   (&self->class->class, self->reg->maj + self->num);
            ida_simple_remove(&self->reg->ida    , self->num)                 ;
            cdev_del         (&self->chr)                                     ;
            po_del (self->class);
            po_del (self->ops);
            po_del (self->reg);
}

EXPORT_SYMBOL(fs_char_t);