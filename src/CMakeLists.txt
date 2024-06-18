add_kernel_source   (
    popcorn-linux-fs
    fs.h
    fs.c

    dev.h
    dev.c

    char.h
    char.c

    poll.h
    poll.c

    ctl.h
    ctl.c

    out.h
    out.c

    in.h
    in.c

    fops.h
    fops.c

    file.h
    file.c
)

inc (popcorn-linux-fs-include
    ../include/fs.h
    ../include/fs/char.h
    ../include/fs/ctl.h
    ../include/fs/dev.h
    ../include/fs/file.h
    ../include/fs/fops.h
    ../include/fs/in.h
    ../include/fs/out.h
    ../include/fs/poll.h
)