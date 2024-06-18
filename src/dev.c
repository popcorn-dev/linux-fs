#include "dev.h"
#include "char.h"

#include "fops.h"

po_obj_trait fs_dev_trait = po_make_trait (
    fs_dev_new    ,
    fs_dev_clone  ,
    null_t        ,
    fs_dev_del    ,
    sizeof(fs_dev),
    null_t
);

po_obj_trait *fs_dev_t = &fs_dev_trait;

bool_t
    fs_dev_new
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

bool_t
    fs_dev_clone
        (fs_dev* par, fs_dev* par_clone)  {
            return false_t;
}

void
    fs_dev_del
        (fs_dev* self)                                                                {
            if (self->dev == fs_char_t) unregister_chrdev_region(self->maj, self->num);
            ida_destroy(&self->ida);
}

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_dev_t);