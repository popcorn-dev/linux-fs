#ifndef PO_LINUX_FS_FILE_H
#define PO_LINUX_FS_FILE_H

#include <core.h>

extern po_obj_trait *fs_file_t;
typedef struct       fs_file { u8_t file[128]; } fs_file;

#endif
