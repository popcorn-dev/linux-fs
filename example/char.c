#include <core.h>
#include <fs.h>

#include <linux/module.h>

static void
    do_read
        (po_obj* self, fs_in* in)                   {
            if (po_trait_of(in) != fs_in_t) goto err;
            fs_in_ready(in, "Hello World", 11);
            return;
    err:    fs_in_err  (in, -EINVAL);
}

static void
    do_write
        (po_obj* self, fs_out* out)                   {
            if (po_trait_of(out) != fs_out_t) goto err;
            u8_t *buf = po_new (char[128]);

            fs_out_ready(out, buf, fs_out_len(out));
            printk ("%s\n", buf);
            po_drop(buf);
            return;
    err:    fs_out_err(out, -EINVAL);
}

static fs_fdo fdo = fs_make_fdo (
    null_t  ,
    do_write,
    do_read ,
    null_t
);

fs_fops *ops;
fs_char *dev;

static int
    mod_init (void)                                    {
        ops = po_make (fs_fops) from (1, &fdo);
        dev = po_make (fs_char) from (2, "example", ops);
        return 0;
}

static void
    mod_exit (void) {
        po_del (ops);
        po_del (dev);
}

module_init(mod_init)
module_exit(mod_exit)

MODULE_LICENSE("GPL");