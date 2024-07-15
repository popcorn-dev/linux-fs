#include "dev.h"
#include "char.h"

#include "fops.h"

static bool_t
    do_new
        (fs_dev* self, u32_t count, va_list arg)                               {
            const char *name = null_t; if (count > 0) name = va_arg(arg, any_t);
            any_t       dev  = null_t; if (count > 1) dev  = va_arg(arg, any_t);
            u64_t       num  = 0;      if (count > 2) num  = va_arg(arg, u64_t);
            u64_t       maj  = 0;      if (count > 3) maj  = va_arg(arg, u64_t);

            if (maj >= shl(1, 12)) return false_t;
            if (num >= shl(1, 20)) return false_t;
            if (!name)             return false_t;
            int res = -1;

            if (dev == fs_char_t) res = alloc_chrdev_region(&self->maj, maj, num, name);
            if (res < 0)          return false_t;
            ida_init(&self->ida);
            self->num = num;
            self->dev = dev;

            return true_t;
}

static bool_t
    do_clone
        (fs_dev* self, fs_dev* clone)  {
            return false_t;
}

static void
    do_del
        (fs_dev* self)                                                                {
            if (self->dev == fs_char_t) unregister_chrdev_region(self->maj, self->num);
            ida_destroy(&self->ida);
}

static pp_obj_trait
    do_obj = pp_make_trait (
        do_new        ,
        do_clone      ,
        null_t        ,
        do_del        ,
        sizeof(fs_dev),
        null_t
);

pp_obj_trait *fs_dev_t = &do_obj;

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_dev_t);