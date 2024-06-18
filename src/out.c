#include "out.h"
#include "file.h"

po_obj_trait fs_out_trait = po_make_trait (
    fs_out_new    ,
    fs_out_clone  ,
    null_t          ,
    fs_out_del    ,
    sizeof(fs_out),
    null_t
);

po_obj_trait *fs_out_t = &fs_out_trait;

bool_t
    fs_out_new
        (fs_out* self, u32_t count, va_list arg)                            {
            fs_file *file = null_t; if (count > 0) file = va_arg(arg, any_t);
            u8_t    *buf  = null_t; if (count > 1) buf  = va_arg(arg, any_t);
            u64_t    len  = 0     ; if (count > 2) len  = va_arg(arg, u64_t);
            po_wait *wait = null_t; if (count > 3) wait = va_arg(arg, any_t);

            if (!po_make_at(&self->buf, po_ua) from (2, buf, len)) return false_t;
            if (po_trait_of(file) != fs_file_t) goto err;
            if (!buf)                           goto err;
            if (!len)                           goto err;

            self->wait = (po_wait*) po_ref (wait);
            self->file = (fs_file*) po_ref (file);
            self->ret  = 0;
            return true_t;
    err:    po_del (&self->buf);
            return false_t;
}

bool_t
    fs_out_clone
        (fs_out* self, fs_out* clone) {
            return false_t;
}

void
    fs_out_del
        (fs_out* self)        {
            po_del(&self->buf);
            po_del(self->file);
            po_del(self->wait);
}

bool_t
    fs_out_ready
        (fs_out* self, void* buf, i64_t len)                 {
            if (po_trait_of(self) != fs_out_t) return false_t;
            if (len < 0)                       return false_t;
            if (!buf)                          return false_t;

            self->ret = (i64_t) po_ua_copy_to(&self->buf, buf, len);
            po_wake(self->wait, true_t);
            return true_t;
}

bool_t
    fs_out_err
        (fs_out* self, i64_t err)                            {
            if (po_trait_of(self) != fs_out_t) return false_t;
            if (err > 0)                       return false_t;
            self->ret = err;

            po_wake (self->wait, true_t);
            return true_t;
}

u64_t
    fs_out_len
        (fs_out* self)                                 {
            if (po_trait_of(self) != fs_out_t) return 0;
            return po_ua_len(&self->buf);
}

#include <linux/module.h>
MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_out_ready);
EXPORT_SYMBOL(fs_out_len);
EXPORT_SYMBOL(fs_out_err);
EXPORT_SYMBOL(fs_out_t);