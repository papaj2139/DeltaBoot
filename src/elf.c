#include "elf.h"
#include <string.h>

int elf_validate(const void *data, uint64_t size) {
    if (size < sizeof(Elf64_Ehdr)) {
        return 0;
    }
    
    const Elf64_Ehdr *ehdr = (const Elf64_Ehdr *)data;
    
    //check magic
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3) {
        return 0;
    }
    
    //must be 64-bit
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS64) {
        return 0;
    }
    
    //must be little-endian
    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        return 0;
    }
    
    //must be executable or shared object (PIE)
    if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN) {
        return 0;
    }
    
    //must be amd64
    if (ehdr->e_machine != EM_X86_64) {
        return 0;
    }
    
    //check program header table bounds
    if (ehdr->e_phoff + (ehdr->e_phnum * ehdr->e_phentsize) > size) {
        return 0;
    }
    
    return 1;
}

EFI_STATUS elf_load(
    EFI_BOOT_SERVICES *bs,
    const void *elf_data,
    uint64_t elf_size,
    uint64_t *entry_point
) {
    const Elf64_Ehdr *ehdr = (const Elf64_Ehdr *)elf_data;
    const uint8_t *base = (const uint8_t *)elf_data;
    
    *entry_point = ehdr->e_entry;
    
    //iterate program headers
    for (uint16_t i = 0; i < ehdr->e_phnum; i++) {
        const Elf64_Phdr *phdr = (const Elf64_Phdr *)(base + ehdr->e_phoff + (i * ehdr->e_phentsize));
        
        //only load PT_LOAD segments
        if (phdr->p_type != PT_LOAD) {
            continue;
        }
        
        //skip empty segments
        if (phdr->p_memsz == 0) {
            continue;
        }
        
        //validate file bounds
        if (phdr->p_offset + phdr->p_filesz > elf_size) {
            return EFI_LOAD_ERROR;
        }
        
        //calculate page-aligned allocation
        uint64_t segment_start = phdr->p_paddr;
        uint64_t segment_end = segment_start + phdr->p_memsz;
        uint64_t page_start = segment_start & ~0xFFFULL;
        uint64_t page_end = (segment_end + 0xFFF) & ~0xFFFULL;
        uint64_t num_pages = (page_end - page_start) / 4096;
        
        //allocate memory at physical address
        EFI_PHYSICAL_ADDRESS alloc_addr = page_start;
        EFI_STATUS status = bs->AllocatePages(
            AllocateAddress,
            EfiLoaderData,
            num_pages,
            &alloc_addr
        );
        
        //if exact address fails try allocating anywhere
        if (EFI_ERROR(status)) {
            status = bs->AllocatePages(
                AllocateAnyPages,
                EfiLoaderData,
                num_pages,
                &alloc_addr
            );
            if (EFI_ERROR(status)) {
                return status;
            }
            
            //adjust entry point if we had to relocate
            //this only works for the first loadable segment containing entry
            if (ehdr->e_entry >= segment_start && ehdr->e_entry < segment_end) {
                *entry_point = alloc_addr + (ehdr->e_entry - page_start);
            }
        }
        
        //calculate destination within allocated pages
        uint8_t *dest = (uint8_t *)alloc_addr + (segment_start - page_start);
        
        //zero the entire segment first (for BSS)
        memset(dest, 0, phdr->p_memsz);
        
        //copy file data
        if (phdr->p_filesz > 0) {
            memcpy(dest, base + phdr->p_offset, phdr->p_filesz);
        }
    }
    
    return EFI_SUCCESS;
}
