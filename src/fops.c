#include "fops.h"
#include "file.h"

#include "poll.h"
#include "ctl.h"
#include "out.h"
#include "in.h"

#include <linux/poll.h>

static int
    do_open
        (struct inode* inode, struct file* file)                   {
            fs_fops *self = container_of(file->f_op, fs_fops, fops);

            if (po_trait_of(self) != fs_fops_t) return -EINVAL;
            if (!self->use)                     return -EINVAL;
            if (!self->fdo)                     return -EINVAL;
            fs_file* ret = po_make(fs_file) from (0);

            if (po_trait_of(ret) != fs_file_t) return -ENOMEM;
            ret->ops  = (fs_fops*) po_ref(self);
            ret->file = file;
            ret->use  = po_obj_new (
                null_t   ,
                self->use,
                2        ,
                self     ,
                ret
            );

            if (po_trait_of(ret->use) != self->use) goto err;
            file->private_data = ret;
            return 0;
    err:    po_del(ret->use);
            po_del(ret)     ;
            return -EINVAL;
}

static int
    do_close
        (struct inode* inode, struct file* file)                   {
            fs_fops *self = container_of(file->f_op, fs_fops, fops);

            if (po_trait_of(self) != fs_fops_t) return -EINVAL;
            if (!self->use)                     return -EINVAL;
            if (!self->fdo)                     return -EINVAL;
            fs_file *ret = file->private_data;

            if (po_trait_of(ret) != fs_file_t) return -EINVAL;
            po_del (ret->use);
            po_del (ret);
            return 0;
}

static ssize_t
    do_in
        (struct file* file, __user char* buf, size_t len, loff_t* off) {
            if (file->f_flags & O_WRONLY) return -EACCES;
            po_wait *wait = null_t;
            fs_file *use  = null_t;
            fs_fops *ops  = null_t;
            i64_t    ret  = 0;

            use = file->private_data; if (po_trait_of(use) != fs_file_t) return -EINVAL;
            ops = use->ops;           if (po_trait_of(ops) != fs_fops_t) return -EINVAL;
            if (!ops->fdo->in) return -EACCES;

            if (!(file->f_flags & O_NONBLOCK)) wait = po_make (po_wait) from (0);
            fs_in *in  = po_make (fs_in) from (
                3  ,
                use,
                buf,
                len
            );

            if (po_trait_of(wait) != po_wait_t) goto err;
            if (po_trait_of(in)   != fs_in_t)   goto err;
            ops->fdo->in (use->use, in, wait);

            po_wait_on(wait, true_t);
            ret = in->ret;
            po_del (in);

            return  ret;
    err:    po_del  (wait);
            po_del  (in)  ;
            return -EINVAL;
}

static ssize_t
    do_out
        (struct file* file, __user char* buf, size_t len, loff_t* off) {
            if (file->f_flags == O_RDONLY) return -EACCES;
            po_wait *wait = null_t;
            fs_file *use  = null_t;
            fs_fops *ops  = null_t;
            i64_t    ret  = 0;

            use = file->private_data; if (po_trait_of(use) != fs_file_t) return -EINVAL;
            ops = use->ops;           if (po_trait_of(ops) != fs_fops_t) return -EINVAL;
            if (!ops->fdo->out) return -EACCES;

            if (!(file->f_flags & O_NONBLOCK)) wait = po_make (po_wait) from (0);
            fs_out *out  = po_make (fs_out) from (
                3  ,
                use,
                buf,
                len
            );

            if (po_trait_of(wait) != po_wait_t) goto err;
            if (po_trait_of(out)  != fs_in_t)   goto err;
            ops->fdo->out (use->use, out, wait);

            po_wait_on(wait, true_t);
            ret = out->ret;
            po_del (wait);
            po_del (out);

            return  ret;
    err:    po_del  (wait);
            po_del  (out) ;
            return -EINVAL;
}

static long
    do_ctl
        (struct file* file, unsigned int code, unsigned long arg) {
                po_wait *wait = null_t;
                fs_file *use  = null_t;
                fs_fops *ops  = null_t;
                i64_t    ret  = 0;

                use = file->private_data; if (po_trait_of(use) != fs_file_t) return -EINVAL;
                ops = use->ops;           if (po_trait_of(ops) != fs_fops_t) return -EINVAL;
                if (!ops->fdo->ctl) return -EACCES;

                if (!(file->f_flags & O_NONBLOCK)) wait = po_make (po_wait) from (0);
                fs_ctl *ctl  = po_make (fs_ctl) from (
                    3   ,
                    use ,
                    code,
                    arg
                );

                if (po_trait_of(wait) != po_wait_t) goto err;
                if (po_trait_of(ctl)  != fs_ctl_t)  goto err;
                ops->fdo->ctl (use->use, ctl, wait);

                po_wait_on(wait, true_t);
                ret = ctl->ret;
                po_del (wait);
                po_del (ctl);

                return  ret;
        err:    po_del  (wait);
                po_del  (ctl) ;
                return -EINVAL;
}

static __poll_t
    do_poll
        (struct file* file, struct poll_table_struct* table) {
            fs_poll *poll = null_t;
            fs_fops *ops  = null_t;
            fs_file *use  = null_t;
            u64_t    ret;

            use  = file->private_data; if (po_trait_of(use) != fs_file_t) return -EINVAL;
            ops  = use->ops;           if (po_trait_of(ops) != fs_fops_t) return -EINVAL;
            poll = po_make (fs_poll) from (1, use);

            if (po_trait_of(poll) != fs_poll_t) goto err;
            if (!ops->fdo->poll)                goto err;
            ops->fdo->poll (use, poll);

            poll_wait(file, &poll->poll, table);
            ret = poll->ret;
            po_del (poll);
            return ret;
    err:    po_del  (poll);
            return -EINVAL;
}

static struct file_operations do_ops   = {
        .open           = do_open ,
        .release        = do_close,
        .read           = do_in   ,
        .write          = do_out  ,
        .unlocked_ioctl = do_ctl  ,
        .compat_ioctl   = do_ctl  ,
        .poll           = do_poll
};

po_obj_trait fs_fops_trait = po_make_trait (
    fs_fops_new    ,
    fs_fops_clone  ,
    null_t              ,
    fs_fops_del    ,
    sizeof(fs_fops),
    null_t
);

po_obj_trait *fs_fops_t = &fs_fops_trait;

bool_t
    fs_fops_new
        (fs_fops* self, u32_t count, va_list arg)                              {
            po_obj_trait *use = null_t; if (count > 1) use = va_arg(arg, any_t);
            fs_fdo       *fdo = null_t; if (count > 0) fdo = va_arg(arg, any_t);
            if (!fdo) return false_t;

            self->fops = do_ops;
            self->use  = use;
            self->fdo  = fdo;
            return true_t;
}

bool_t
    fs_fops_clone
        (fs_fops* self, fs_fops* clone) {
            self->fops = clone->fops;
            self->use  = clone->use;
            self->fdo  = clone->fdo;
            return true_t;
}

void
    fs_fops_del
        (fs_fops* par) {
}

#include <linux/module.h>

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_fops_t);