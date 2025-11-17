#pragma once
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct page_allocator_info {
    uint32_t page_to; //address that needs to be paged in
    uint32_t num_pages; //num of pages to that address
    uint32_t bitmap_size;
    uint32_t bitmap_addr;
};
typedef struct page_allocator_info page_allocator_info_t;


/* Memory Map */
struct memmap_entry {
    uint32_t start;
    uint32_t size;
    uint8_t flags;
};
typedef struct memmap_entry memmap_entry_t;

struct memmap {
    memmap_entry_t entries[4096];
    uint32_t index;
};
typedef struct memmap memory_map_t;

struct multiboot_tags_header {
    uint32_t total_size;
    uint32_t reserved;
};
typedef struct multiboot_tags_header multiboot_tags_header_t;


/* System Info */
struct sysinfo{
    multiboot_tags_header_t tags_header;
    struct multiboot_tag_mmap* grub_memmap;
    memory_map_t* memmap;

    uint32_t mem_lower;
    uint32_t mem_upper;

    char* cmdline;
    char* boot_loader_name;

    page_allocator_info_t page_allocator_info;
};
typedef struct sysinfo sysinfo_t;

sysinfo_t get_sysinfo(void* multiboot_info);