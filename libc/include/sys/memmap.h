#include <stdint.h>
#include <stddef.h>
#include <sys/sysinfo.h>
#include <sys/multiboot.h>

void show_memmap(multiboot_info_t* mbd);