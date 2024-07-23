#ifndef LINUX_FS_FILE_H
#define LINUX_FS_FILE_H

#include <core.h>

extern pp_obj_trait *fs_file_t;
typedef struct       fs_file { u8_t file[128]; } fs_file;

#endif
