#ifndef _FILE_H
#define _FILE_H

#include <stdint.h>
#include "efi.h"

//load file from boot device
EFI_STATUS file_load(
    EFI_HANDLE image_handle,
    EFI_BOOT_SERVICES *bs,
    CHAR16 *path, 
    void **data, 
    uint64_t *size
);

#endif
