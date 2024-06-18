#include "ctl.h"
#include "file.h"

#include <linux/uaccess.h>

po_obj_trait fs_ctl_trait = po_make_trait (
    fs_ctl_new    ,
    fs_ctl_clone  ,
    null_t        ,
    fs_ctl_del    ,
    sizeof(fs_ctl),
    null_t
);

po_obj_trait* fs_ctl_t = &fs_ctl_trait;

bool_t
    fs_ctl_new
        (fs_ctl* self, u32_t count, va_list par)                            {
            po_wait *wait = null_t; if (count > 3) wait = va_arg(par, any_t);
            fs_file *file = null_t; if (count > 0) file = va_arg(par, any_t);
            u64_t    code = 0     ; if (count > 1) code = va_arg(par, u64_t);
            any_t    arg  = 0     ; if (count > 2) arg  = va_arg(par, any_t);
            if (po_trait_of(file) != fs_file_t) return false_t;

            self->wait = (po_wait*) po_ref(wait);
            self->use  = (fs_file*) po_ref(file);
            self->code = code;
            self->arg  = arg ;
            return true_t;
}

bool_t
    fs_ctl_clone
        (fs_ctl* self, fs_ctl* clone) {
            return false_t;
}

void
    fs_ctl_del
        (fs_ctl* self)         {
            po_del (self->wait);
            po_del (self->use);
}

void
    fs_ctl_ready
        (fs_ctl* self, i64_t ret)                    {
            if (po_trait_of(self) != fs_ctl_t) return;
            self->ret = ret;

            po_wake(self->wait, true_t);
}

any_t fs_ctl_arg  (fs_ctl* self) { if (po_trait_of(self) != fs_ctl_t) return null_t; return self->arg ; }
u64_t fs_ctl_code (fs_ctl* self) { if (po_trait_of(self) != fs_ctl_t) return -1;     return self->code; }

any_t
    fs_ctl_arg_ka
        (fs_ctl* self, u64_t len, po_mem* mem)                   {
            if (po_trait_of(mem)  != po_mem_t) mem = po_get_mem();
            if (po_trait_of(mem)  != po_mem_t) return null_t;
            if (po_trait_of(self) != fs_ctl_t) return null_t;
            if (!len)                          return null_t;
            any_t ret = po_mem_use(mem, null_t, len);

            if (!ret) return null_t;
            copy_from_user         (
                ret      ,
                self->arg,
                len
            );

            return ret;

}

po_ua*
    fs_ctl_arg_ua
        (fs_ctl* self, u64_t len)                           {
            if (po_trait_of(self) != fs_ctl_t) return null_t;
            if (!len)                          return null_t;
            return po_make (po_ua) from (
                2        ,
                self->arg,
                len
            );
}

#include <linux/module.h>
EXPORT_SYMBOL(fs_ctl_ready);
EXPORT_SYMBOL(fs_ctl_code);

EXPORT_SYMBOL(fs_ctl_arg_ka);
EXPORT_SYMBOL(fs_ctl_arg_ua);
EXPORT_SYMBOL(fs_ctl_arg);

EXPORT_SYMBOL(fs_ctl_t);