#include <stdint.h>
#include <stddef.h>
#include <sys/sysinfo.h>

/*  Incredibly primitive while I'm learning
    Lets see how much this changes lol */


memmap_entry_t create_mmap_entry(uint32_t start, uint32_t size, uint32_t flags);

memory_map_t* generate_memmap(void* multiboot_info, sysinfo_t* sysinfo);

uint32_t sort_memmap(memory_map_t* mmap);

void condense_memmap(memory_map_t* mmap);