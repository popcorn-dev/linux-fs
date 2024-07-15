#include "in.h"
#include "file.h"

#include <linux/errno.h>

static bool_t
    do_new
        (fs_in* self, u32_t count, va_list arg)                             {
            fs_file *file = null_t; if (count > 0) file = va_arg(arg, any_t);
            u8_t    *buf  = null_t; if (count > 1) buf  = va_arg(arg, any_t);
            u64_t    len  = 0     ; if (count > 2) len  = va_arg(arg, u64_t);
            pp_wait *wait = null_t; if (count > 3) wait = va_arg(arg, any_t);

            if (!pp_make_at(&self->buf, pp_ua) from (2, buf, len)) return false_t;
            if (pp_trait_of(file) != fs_file_t) goto err;
            if (!buf)                           goto err;
            if (!len)                           goto err;

            self->file = (fs_file*) pp_ref(file);
            self->wait = (pp_wait*) pp_ref(wait);
            self->ret  = 0;
            return true_t;
    err:    pp_del (&self->buf);
            return false_t;
}

static bool_t
    do_clone
        (fs_in* self, fs_in* clone) {
            return false_t;
}

static void
    do_del
        (fs_in* self)         {
            pp_del(&self->buf);
            pp_del(self->file);
            pp_del(self->wait);
}

static pp_obj_trait
    do_obj = pp_make_trait (
        do_new       ,
        do_clone     ,
        null_t       ,
        do_del       ,
        sizeof(fs_in),
        null_t
);

pp_obj_trait* fs_in_t = &do_obj;

bool_t
    fs_in_err
        (fs_in* self, i64_t err)                            {
            if (pp_trait_of(self) != fs_in_t) return false_t;
            if (err > 0)                      return false_t;
            self->ret = err;

            pp_wake(self->wait, true_t);
            return true_t;
}

bool_t
    fs_in_ready
        (fs_in* self, void* buf, i64_t len)                 {
            if (pp_trait_of(self) != fs_in_t) return false_t;
            if (len < 0)                      goto err;
            if (!buf)                         goto err;

            self->ret = (i64_t) pp_ua_copy_from (&self->buf,buf,len);
            pp_wake (self->wait, true_t);
            return true_t;
    err:    fs_in_err(self, -EINVAL);
            return false_t;
}

u64_t
    fs_in_len
        (fs_in* self)                                 {
            if (pp_trait_of(self) != fs_in_t) return 0;
            return pp_ua_len(&self->buf);
}

#include <linux/module.h>

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_in_err);
EXPORT_SYMBOL(fs_in_ready);
EXPORT_SYMBOL(fs_in_len);
EXPORT_SYMBOL(fs_in_t);