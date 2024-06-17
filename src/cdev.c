#include "cdev.h"
#include "char.h"

#include "file.h"
#include "use.h"

#include "../../../lib/popcorn/linux/src/dev/class.h"

po_obj_trait po_chr_trait = po_make_trait (
    po_chr_new    ,
    po_chr_clone  ,
    null_t        ,
    po_chr_del    ,
    sizeof(po_chr),
    null_t
);

po_obj_trait *po_chr_t = &po_chr_trait;

bool_t
    po_chr_new
        (po_chr* par_dev, u32_t par_count, va_list par)                                        {
            po_str       *name  = null_t; if (par_count > 0) name  = va_arg(par, po_str*)      ;
            po_class     *class = null_t; if (par_count > 1) class = va_arg(par, po_class*)    ;
            po_file_type *file  = null_t; if (par_count > 2) file  = va_arg(par, po_file_type*);
            po_chr_type  *type  = null_t; if (par_count > 3) type  = va_arg(par, po_chr_type*) ;
            dev_t         num   = -1    ; if (par_count > 4) num   = va_arg(par, dev_t)        ;
            if (po_trait_of(type)  != po_chr_type_t)  return false_t;
            if (po_trait_of(file)  != po_file_type_t) return false_t;
            if (po_trait_of(class) != po_class_t)     return false_t;
            if (po_trait_of(name)  != po_str_t)       return false_t;

            if (num == -1)      num = ida_simple_get(&type->ida, 0  , 0      , GFP_KERNEL);
            else                num = ida_simple_get(&type->ida, num, num + 1, GFP_KERNEL);
            if (num == -ENOSPC) return false_t;

            cdev_init   (&par_dev->chr, &file->type);
            if (cdev_add(&par_dev->chr, type->maj + num, 1) < 0) return false_t;
            par_dev->dev = device_create                                       (
                &class->class  ,
                null_t         ,
                type->maj + num,
                par_dev        ,
                po_str_ptr(name)
            );

            par_dev->class = (po_class*)     po_ref(class);
            par_dev->file  = (po_file_type*) po_ref(file) ;
            par_dev->type  = (po_chr_type*)  po_ref(type) ;
            par_dev->num   = num;
            return true_t;
}

bool_t
    po_chr_clone
        (po_chr* par, po_chr* par_clone) {
            return false_t;
}

void
    po_chr_del
        (po_chr* par)                                                       {
            device_destroy   (&par->class->class, par->type->maj + par->num);
            ida_simple_remove(&par->type->ida   , par->num)                 ;
            cdev_del         (&par->chr)                                    ;
            po_del (par->class);
            po_del (par->type) ;
            po_del (par->file) ;
}

EXPORT_SYMBOL(po_chr_t);