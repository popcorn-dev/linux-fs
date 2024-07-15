#include "fs.h"

static bool_t
    do_new
        (fs_char* self, u32_t count, va_list arg)                                {
            const char *name  = null_t; if (count > 0) name  = va_arg(arg, any_t);
            fs_fops    *ops   = null_t; if (count > 1) ops   = va_arg(arg, any_t);
            fs_dev     *reg   = null_t; if (count > 2) reg   = va_arg(arg, any_t);
            pp_class   *class = null_t; if (count > 3) class = va_arg(arg, any_t);

            if (pp_trait_of(class) != pp_class_t) class = &fs.class;
            if (pp_trait_of(reg)   != fs_dev_t)   reg   = &fs.chr;
            dev_t num = ida_alloc(&reg->ida, GFP_KERNEL);
            cdev_init(&self->chr, &ops->fops);

            if (cdev_add(&self->chr, reg->maj + num, 1) < 0) goto err;
            self->dev = pp_make (pp_dev) from (
                4             ,
                name          ,
                reg->maj + num,
                self          ,
                class
            );

            if (pp_trait_of(self->dev) != pp_dev_t) goto err;
            self->class = (pp_class*) pp_ref(class);
            self->ops   = (fs_fops*)  pp_ref(ops);
            self->reg   = (fs_dev *)  pp_ref(reg);
            self->num   = num;
            return true_t;
    err:    ida_simple_remove(&reg->ida, num);
            cdev_del         (&self->chr)    ;
            return false_t;
}

static bool_t
    do_clone
        (fs_char* self, fs_char* clone) {
            return false_t;
}

static void
    do_del
        (fs_char* self)                                  {
            ida_simple_remove(&self->reg->ida, self->num);
            cdev_del         (&self->chr)                ;
            pp_del (self->class);
            pp_del (self->dev);
            pp_del (self->ops);
            pp_del (self->reg);
}

static pp_obj_trait
    do_obj = pp_make_trait (
        do_new         ,
        do_clone       ,
        null_t         ,
        do_del         ,
        sizeof(fs_char),
        null_t
);

pp_obj_trait *fs_char_t = &do_obj;

EXPORT_SYMBOL(fs_char_t);