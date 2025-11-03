#include <sys/sysinfo.h>
#include <sys/memmap.h>

void parse_multiboot_info(void* multiboot_info, sysinfo_t* info) {
    //tags header
    info->tags_header = *(multiboot_tags_header_t*)multiboot_info;
    //tag offset address
    printf("Searching multiboot information structure...\n");

    struct multiboot_tag* tag;

    for (tag = (struct multiboot_tag*)((size_t)multiboot_info + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~ 7))) {
            char tsize[33];
            char ttype[33];
            itoa(tag->size, tsize, 10);
            itoa(tag->type, ttype, 10);
            printf("Found tag of size: ");
            printf(tsize);
            printf(" type: ");
            printf(ttype);

            
        }
}