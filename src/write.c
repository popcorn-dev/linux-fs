#include "write.h"
#include "use.h"

po_obj_trait fs_write_trait = po_make_trait (
    fs_write_new    ,
    fs_write_clone  ,
    null_t          ,
    fs_write_del    ,
    sizeof(fs_write),
    null_t
);

po_obj_trait *fs_write_t = &fs_write_trait;

bool_t
    fs_write_new
        (fs_write* self, u32_t count, va_list arg)                       {
            fs_use *use = null_t; if (count > 0) use = va_arg(arg, any_t);
            u8_t   *buf = null_t; if (count > 1) buf = va_arg(arg, any_t);
            u64_t   len = 0     ; if (count > 2) len = va_arg(arg, u64_t);

            if (!po_make_at(&self->buf, po_ua) from (2, buf, len)) return false_t;
            if (po_trait_of(use) != fs_use_t) goto err;
            if (!buf)                         goto err;
            if (!len)                         goto err;
            self->stat = po_fut_pend;

            self->use = (fs_use*) po_ref (use);
            self->ret  = 0;
            return true_t;
    err:    po_del (&self->buf);
            return false_t;
}

bool_t
    fs_write_clone
        (fs_write* self, fs_write* clone) {
            return false_t;
}

void
    fs_write_del
        (fs_write* self)      {
            po_del(&self->buf);
            po_del (self->use);
}

po_ua*
    fs_write_buf
        (fs_write* self)                                       {
            if (po_trait_of(self) != fs_write_t) return null_t;
            return &self->buf;
}

bool_t
    fs_write_ready
        (fs_write* self, u64_t len)                            {
            if (po_trait_of(self) != fs_write_t) return false_t;
            if (self->stat != po_fut_pend)       return false_t;
            self->ret = len;

            self->stat = po_fut_ready;
            return true_t;
}

bool_t
    fs_write_err
        (fs_write* self, u64_t err)                            {
            if (po_trait_of(self) != fs_write_t) return false_t;
            if (self->stat != po_fut_pend)       return false_t;
            self->ret = err;

            self->stat = po_fut_err;
            return true_t;
}

u64_t
    fs_write_len
        (fs_write* self)                                 {
            if (po_trait_of(self) != fs_write_t) return 0;
            return po_ua_len(&self->buf);
}

static u64_t
    do_poll
        (fs_write* self)                                          {
            if (po_trait_of(self) != fs_write_t) return po_fut_err;
            return self->stat;
}

static u64_t
    do_ret
        (fs_write* self)                                          {
            if (po_trait_of(self) != fs_write_t) return po_fut_err;
            return self->ret;
}

static po_fut_ops
    do_fut = po_make_fut_ops (
    do_poll,
    do_ret
);

po_fut*
    fs_write_fut
        (fs_write* self)                                       {
            if (po_trait_of(self) != fs_write_t) return null_t;
            return po_make (po_fut) from       (
                2      ,
                &do_fut,
                self
            );
}

#include <linux/module.h>
MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_write_to);
EXPORT_SYMBOL(fs_write_len);
EXPORT_SYMBOL(fs_write_ready);
EXPORT_SYMBOL(fs_write_err);
EXPORT_SYMBOL(fs_write_buf);
EXPORT_SYMBOL(fs_write_fut);
EXPORT_SYMBOL(fs_write_t);