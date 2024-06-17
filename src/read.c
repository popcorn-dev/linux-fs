#include "read.h"
#include "use.h"

po_obj_trait fs_read_trait = po_make_trait (
    fs_read_new    ,
    fs_read_clone  ,
    null_t         ,
    fs_read_del    ,
    sizeof(fs_read),
    null_t
);

po_obj_trait* fs_read_t = &fs_read_trait;

bool_t
    fs_read_new
        (fs_read* self, u32_t count, va_list arg)                        {
            fs_use *use = null_t; if (count > 0) use = va_arg(arg, any_t);
            u8_t   *buf = null_t; if (count > 1) buf = va_arg(arg, any_t);
            u64_t   len = 0     ; if (count > 2) len = va_arg(arg, u64_t);

            if (!po_make_at(&self->buf, po_ua) from (2, buf, len)) return false_t;
            if (po_trait_of(use) != fs_use_t) goto err;
            if (!buf)                         goto err;
            if (!len)                         goto err;
            self->stat = po_fut_pend;

            self->use = (fs_use*) po_ref(use);
            self->ret = 0;
            return true_t;
    err:    po_del (&self->buf);
            return false_t;
}

bool_t
    fs_read_clone
        (fs_read* self, fs_read* clone) {
            return false_t;
}

void
    fs_read_del
        (fs_read* self)       {
            po_del(&self->buf);
            po_del (self->use);
}


void
    fs_read_err
        (fs_read* self, u64_t err)                    {
            if (po_trait_of(self) != fs_read_t) return;
            if (self->stat != po_fut_pend)      return;
            self->stat = po_fut_err;
            self->ret  = err;

}

void
    fs_read_ready
        (fs_read* self, u64_t len)                    {
            if (po_trait_of(self) != fs_read_t) return;
            if (self->stat != po_fut_pend)      return;
            self->ret  = len;

            self->stat = po_fut_ready;
}

u64_t
    fs_read_len
        (fs_read* self)                                 {
            if (po_trait_of(self) != fs_read_t) return 0;
            return po_ua_len(&self->buf);
}

po_ua*
    fs_read_buf
        (fs_read* self)                                 {
            if (po_trait_of(self) != fs_read_t) return 0;
            return &self->buf;
}

static u64_t
    do_poll
        (fs_read* par)                                          {
            if (po_trait_of(par) != fs_read_t) return po_fut_err;
            return par->stat;
}

static u64_t
    do_ret
        (fs_read* par)                                          {
            if (po_trait_of(par) != fs_read_t) return po_fut_err;
            return par->ret;
}

static po_fut_ops
    do_fut = po_make_fut_ops (
    do_poll,
    do_ret
);

po_fut*
    fs_read_fut
        (fs_read* self)                                      {
            if (po_trait_of(self) != fs_read_t) return null_t;
            return po_make (po_fut) from      (
                2      ,
                &do_fut,
                self
            );
}

#include <linux/module.h>

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_read_err);
EXPORT_SYMBOL(fs_read_ready);
EXPORT_SYMBOL(fs_read_len);
EXPORT_SYMBOL(fs_read_fut);
EXPORT_SYMBOL(fs_read_t);