#ifndef _BOOTINFO_H
#define _BOOTINFO_H

#include <stdint.h>

//memory map entry types (matches EFI)
#define MMAP_USABLE             7   //EfiConventionalMemory
#define MMAP_RESERVED           0   //EfiReservedMemoryType
#define MMAP_ACPI_RECLAIMABLE   9   //EfiACPIReclaimMemory
#define MMAP_ACPI_NVS           10  //EfiACPIMemoryNVS
#define MMAP_BAD                8   //EfiUnusableMemory
#define MMAP_BOOTLOADER_RECLAIM 2   //EfiLoaderData - can be reclaimed

//memory map entry
typedef struct {
    uint32_t type;
    uint64_t phys_start;
    uint64_t virt_start;
    uint64_t num_pages;
    uint64_t attributes;
} __attribute__((packed)) MemoryMapEntry;

//boot info passed from bootloader to kernel
typedef struct {
    //magic number for validation
    uint64_t magic;
    #define BOOTINFO_MAGIC 0x44454C424F4F5421ULL  //"DELBOOT!"
    
    //framebuffer info
    uint64_t fb_base;
    uint32_t fb_width;
    uint32_t fb_height;
    uint32_t fb_pitch;      //bytes per scanline
    uint32_t fb_bpp;        //bits per pixel
    
    //memory map
    uint64_t mmap_entries;
    uint64_t mmap_entry_size;
    MemoryMapEntry *mmap;   //pointer to memory map array
    
    //total usable memory
    uint64_t total_memory;
    
} __attribute__((packed)) BootInfo;

#endif
