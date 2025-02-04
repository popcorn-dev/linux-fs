#include "fs.h"

struct fs fs;

static int
    mod_init(void)                                                                       {
        if (!pp_make_at(&fs.class, pp_class) from (1, "popcorn_fs"))            return -1;
        if (!pp_make_at(&fs.chr  , fs_dev)   from (2, "popcorn_fs", fs_char_t)) return -1;
        return 0;
}

static void
    mod_exit(void)        {
        pp_del (&fs.class);
        pp_del (&fs.chr);
}

MODULE_LICENSE("GPL");
module_init(mod_init)
module_exit(mod_exit)