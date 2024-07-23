#ifndef LINUX_FS_DEV_H
#define LINUX_FS_DEV_H

#include <core.h>

extern pp_obj_trait *fs_dev_t;
typedef struct       fs_dev { u8_t dev[128]; } fs_dev;

#endif
