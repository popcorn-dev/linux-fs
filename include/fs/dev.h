#ifndef PO_LINUX_FS_DEV_H
#define PO_LINUX_FS_DEV_H

#include <core.h>

extern po_obj_trait *fs_dev_t;
typedef struct       fs_dev { u8_t dev[128]; } fs_dev;

#endif
