#include "ctl.h"
#include "file.h"

#include <linux/uaccess.h>

static bool_t
    do_new
        (fs_ctl* self, u32_t count, va_list par)                            {
            pp_wait *wait = null_t; if (count > 3) wait = va_arg(par, any_t);
            fs_file *file = null_t; if (count > 0) file = va_arg(par, any_t);
            u64_t    code = 0     ; if (count > 1) code = va_arg(par, u64_t);
            any_t    arg  = 0     ; if (count > 2) arg  = va_arg(par, any_t);
            if (pp_trait_of(file) != fs_file_t) return false_t;

            self->wait = (pp_wait*) pp_ref(wait);
            self->use  = (fs_file*) pp_ref(file);
            self->code = code;
            self->arg  = arg ;
            return true_t;
}

static bool_t
    do_clone
        (fs_ctl* self, fs_ctl* clone) {
            return false_t;
}

static void
    do_del
        (fs_ctl* self)         {
            pp_del (self->wait);
            pp_del (self->use);
}


static pp_obj_trait
    do_obj = pp_make_trait (
        do_new        ,
        do_clone      ,
        null_t        ,
        do_del        ,
        sizeof(fs_ctl),
        null_t
);

pp_obj_trait* fs_ctl_t = &do_obj;

void
    fs_ctl_ready
        (fs_ctl* self, i64_t ret)                    {
            if (pp_trait_of(self) != fs_ctl_t) return;
            self->ret = ret;

            pp_wake(self->wait, true_t);
}

any_t fs_ctl_arg  (fs_ctl* self) { if (pp_trait_of(self) != fs_ctl_t) return null_t; return self->arg ; }
u64_t fs_ctl_code (fs_ctl* self) { if (pp_trait_of(self) != fs_ctl_t) return -1;     return self->code; }

any_t
    fs_ctl_arg_ka
        (fs_ctl* self, u64_t len, pp_mem* mem)                   {
            if (pp_trait_of(mem)  != pp_mem_t) mem = pp_get_mem();
            if (pp_trait_of(mem)  != pp_mem_t) return null_t;
            if (pp_trait_of(self) != fs_ctl_t) return null_t;
            if (!len)                          return null_t;
            any_t ret = pp_mem_use(mem, null_t, len);

            if (!ret) return null_t;
            copy_from_user         (
                ret      ,
                self->arg,
                len
            );

            return ret;

}

pp_ua*
    fs_ctl_arg_ua
        (fs_ctl* self, u64_t len)                           {
            if (pp_trait_of(self) != fs_ctl_t) return null_t;
            if (!len)                          return null_t;
            return pp_make (pp_ua) from (
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