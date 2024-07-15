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

            if (pp_trait_of(self) != fs_fops_t) return -EINVAL;
            if (!self->fdo)                     return -EINVAL;
            fs_file* ret = pp_make(fs_file) from (0);

            if (pp_trait_of(ret) != fs_file_t) return -ENOMEM;
            ret->ops  = (fs_fops*) pp_ref(self);
            ret->file = file;
            ret->use  = pp_obj_new (
                null_t   ,
                self->use,
                2        ,
                self     ,
                ret
            );

            if (pp_trait_of(ret->use) != self->use) goto err;
            file->private_data = ret;
            return 0;
    err:    pp_del(ret->use);
            pp_del(ret)     ;
            return -EINVAL;
}

static int
    do_close
        (struct inode* inode, struct file* file) {
            fs_file *self = file->private_data;

            if (pp_trait_of(self) != fs_fops_t) return -EINVAL;
            pp_del (self->use);
            pp_del (self);
            return 0;
}

static ssize_t
    do_in
        (struct file* file, __user char* buf, size_t len, loff_t* off) {
            if (file->f_flags & O_WRONLY) return -EACCES;
            pp_wait *wait = null_t;
            fs_file *use  = null_t;
            fs_fops *ops  = null_t;
            i64_t    ret  = 0;

            use = file->private_data; if (pp_trait_of(use) != fs_file_t) return -EINVAL;
            ops = use->ops;           if (pp_trait_of(ops) != fs_fops_t) return -EINVAL;
            if (!ops->fdo->in) return -EACCES;

            if (!(file->f_flags & O_NONBLOCK)) wait = pp_make (pp_wait) from (0);
            fs_in *in  = pp_make (fs_in) from (
                4  ,
                use,
                buf,
                len,
                wait
            );

            if (pp_trait_of(wait) != pp_wait_t) goto err;
            if (pp_trait_of(in)   != fs_in_t)   goto err;
            ops->fdo->in (use->use, in);

            pp_wait_on(wait, true_t);
            ret = in->ret;
            pp_del (wait);
            pp_del (in);

            return  ret;
    err:    pp_del  (wait);
            pp_del  (in)  ;
            return -EINVAL;
}

static ssize_t
    do_out
        (struct file* file, __user const char* buf, size_t len, loff_t* off) {
            if (file->f_flags == O_RDONLY) return -EACCES;
            pp_wait *wait = null_t;
            fs_file *use  = null_t;
            fs_fops *ops  = null_t;
            i64_t    ret  = 0;

            use = file->private_data; if (pp_trait_of(use) != fs_file_t) return -EINVAL;
            ops = use->ops;           if (pp_trait_of(ops) != fs_fops_t) return -EINVAL;
            if (!ops->fdo->out) return -EACCES;

            if (!(file->f_flags & O_NONBLOCK)) wait = pp_make (pp_wait) from (0);
            fs_out *out  = pp_make (fs_out) from (
                4  ,
                use,
                buf,
                len,
                wait
            );

            if (pp_trait_of(wait) != pp_wait_t) goto err;
            if (pp_trait_of(out)  != fs_out_t)  goto err;
            ops->fdo->out (use->use, out);

            pp_wait_on(wait, true_t);
            ret = out->ret;
            pp_del (wait);
            pp_del (out);

            return  ret;
    err:    pp_del  (wait);
            pp_del  (out) ;
            return -EINVAL;
}

static long
    do_ctl
        (struct file* file, unsigned int code, unsigned long arg) {
            pp_wait *wait = null_t;
            fs_file *use  = null_t;
            fs_fops *ops  = null_t;
            i64_t    ret  = 0;

            use = file->private_data; if (pp_trait_of(use) != fs_file_t) return -EINVAL;
            ops = use->ops;           if (pp_trait_of(ops) != fs_fops_t) return -EINVAL;
            if (!ops->fdo->ctl) return -EACCES;

            if (!(file->f_flags & O_NONBLOCK)) wait = pp_make (pp_wait) from (0);
            fs_ctl *ctl  = pp_make (fs_ctl) from (
                4   ,
                use ,
                code,
                arg ,
                wait
            );

            if (pp_trait_of(wait) != pp_wait_t) goto err;
            if (pp_trait_of(ctl)  != fs_ctl_t)  goto err;
            ops->fdo->ctl (use->use, ctl);

            pp_wait_on(wait, true_t);
            ret = ctl->ret;
            pp_del (wait);
            pp_del (ctl);

            return  ret;
    err:    pp_del  (wait);
            pp_del  (ctl) ;
            return -EINVAL;
}

static __poll_t
    do_poll
        (struct file* file, struct poll_table_struct* table) {
            fs_poll *poll = null_t;
            fs_fops *ops  = null_t;
            fs_file *use  = null_t;
            u64_t    ret;

            use  = file->private_data; if (pp_trait_of(use) != fs_file_t) return -EINVAL;
            ops  = use->ops;           if (pp_trait_of(ops) != fs_fops_t) return -EINVAL;
            poll = pp_make (fs_poll) from (1, use);

            if (pp_trait_of(poll) != fs_poll_t) goto err;
            if (!ops->fdo->poll)                goto err;
            ops->fdo->poll (use->use, poll);

            poll_wait(file, &poll->poll, table);
            ret = poll->ret;
            pp_del (poll);
            return ret;
    err:    pp_del  (poll);
            return -EINVAL;
}

static struct file_operations do_ops = {
    .open           = do_open ,
    .release        = do_close,
    .read           = do_in   ,
    .write          = do_out  ,
    .unlocked_ioctl = do_ctl  ,
    .compat_ioctl   = do_ctl  ,
    .poll           = do_poll
};

static bool_t
    do_new
        (fs_fops* self, u32_t count, va_list arg)                              {
            fs_fdo       *fdo = null_t; if (count > 0) fdo = va_arg(arg, any_t);
            pp_obj_trait *use = null_t; if (count > 1) use = va_arg(arg, any_t);
            if (!fdo) return false_t;

            self->fops = do_ops;
            self->use  = use;
            self->fdo  = fdo;
            return true_t;
}

static bool_t
    do_clone
        (fs_fops* self, fs_fops* clone) {
            self->fops = clone->fops;
            self->use  = clone->use;
            self->fdo  = clone->fdo;
            return true_t;
}

static void
    do_del
        (fs_fops* par) {
}

static pp_obj_trait
    do_obj = pp_make_trait (
        do_new         ,
        do_clone       ,
        null_t         ,
        do_del         ,
        sizeof(fs_fops),
        null_t
);

pp_obj_trait *fs_fops_t = &do_obj;

#include <linux/module.h>

MODULE_LICENSE("GPL");
EXPORT_SYMBOL(fs_fops_t);