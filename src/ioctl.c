#include "ioctl.h"
#include "use.h"

po_obj_trait fs_ioctl_trait = po_make_trait (
    fs_ioctl_new    ,
    fs_ioctl_clone  ,
    null_t            ,
    fs_ioctl_del    ,
    sizeof(fs_ioctl),
    null_t
);

po_obj_trait* fs_ioctl_t = &fs_ioctl_trait;

bool_t
    fs_ioctl_new
        (fs_ioctl* par_con, u32_t par_count, va_list par)                        {
            po_file *file = null_t; if (par_count > 0) file = va_arg(par, po_file*);
            u64_t    code = 0     ; if (par_count > 1) code = va_arg(par, u64_t)   ;
            any_t    arg  = 0     ; if (par_count > 2) arg  = va_arg(par, any_t)   ;
            if (po_trait_of(file) != po_file_t) return false_t;

            par_con->stat = po_fut_pend;
            par_con->file = file;
            par_con->code = code;
            par_con->arg  = arg ;
            return true_t;
}

bool_t
    fs_ioctl_clone
        (fs_ioctl* par, fs_ioctl* par_clone) {
            return false_t;
}

void
    fs_ioctl_del
        (fs_ioctl* par)    {
}

void
    fs_ioctl_ready
        (fs_ioctl* par, u64_t par_ret)                      {
            if (po_trait_of(par)       != fs_ioctl_t) return;
            if (po_trait_of(par->file) != po_file_t)    return;
            if (par->stat              != po_fut_pend)  return;
            par->ret  = par_ret     ;
            par->stat = po_fut_ready;
}

any_t
    fs_ioctl_arg
        (fs_ioctl* par)                                            {
            if (po_trait_of(par)       != fs_ioctl_t) return null_t;
            if (po_trait_of(par->file) != po_file_t)    return null_t;
            if (par->stat != po_fut_pend)               return null_t;
            return par->arg;
}

u64_t
    fs_ioctl_code
        (fs_ioctl* par)                                        {
            if (po_trait_of(par)       != fs_ioctl_t) return -1;
            if (po_trait_of(par->file) != po_file_t)    return -1;
            if (par->stat != po_fut_pend)               return -1;
            return par->code;
}

u64_t
    fs_ioctl_do_poll
        (fs_ioctl* par)                                                {
            if (po_trait_of(par)       != fs_ioctl_t) return po_fut_err;
            if (po_trait_of(par->file) != po_file_t)    return po_fut_err;
            return par->stat;
}

u64_t
    fs_ioctl_do_ret
        (fs_ioctl* par)                                          {
            if (po_trait_of(par)       != fs_ioctl_t) return 0ull;
            if (po_trait_of(par->file) != po_file_t)    return 0ull;
            return par->ret;
}

po_fut_ops fs_ioctl_fut_ops = po_make_fut_ops (
    fs_ioctl_do_poll,
    fs_ioctl_do_ret
);

po_fut*
    fs_ioctl_fut
        (fs_ioctl* par)                                            {
            if (po_trait_of(par)       != fs_ioctl_t) return null_t;
            if (po_trait_of(par->file) != po_file_t)    return null_t;
            return po_make (po_fut) from                             (
                2                  ,
                &fs_ioctl_fut_ops,
                par
            );
}

#include <linux/module.h>
EXPORT_SYMBOL(fs_ioctl_ready);
EXPORT_SYMBOL(fs_ioctl_code);
EXPORT_SYMBOL(fs_ioctl_arg);
EXPORT_SYMBOL(fs_ioctl_fut);
EXPORT_SYMBOL(fs_ioctl_t);