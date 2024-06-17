kernel_source       (
    popcorn-linux-fs
        char.h
        char.c

        cdev.h
        cdev.c

    char.h

    ioctl.h
    ioctl.c

    write.h
    write.c

    read.h
    read.c

    file.h
    file.c

    use.h
    use.c
)

target_include_directories(popcorn-linux-fs PRIVATE ../../linux-core/include)