#ifndef PO_LINUX_FS_H
#define PO_LINUX_FS_H

#include <core.h>
#include <core/class.h>

#include "char.h"
#include "ctl.h"
#include "dev.h"
#include "file.h"
#include "fops.h"
#include "in.h"
#include "out.h"
#include "poll.h"

struct fs         {
    po_class class;
    fs_dev   chr;
    fs_dev   blk;
};

extern struct fs fs;

#endif
