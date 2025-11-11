#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/multiboot.h>

void show_memmap(multiboot_info_t* mbd) {
    uint32_t i;
	for ( i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
		if (mmmt->type == 1){
			printf("Start Addr: 0x%x%x | Length 0x%x%x | Size: %d | Type: AVAIL\n", mmmt->addr_low, mmmt->addr_high, mmmt->len_low, mmmt->len_high, mmmt->size);
		} else {
			printf("Start Addr: 0x%x%x | Length 0x%x%x | Size: %d | Type: RESERV\n", mmmt->addr_low, mmmt->addr_high, mmmt->len_low, mmmt->len_high, mmmt->size);
		}
		//printf("Cycling mmap entries");
		if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
			// TODO: Add smth here idk
			//printf("amongus\n");
			continue;
		}
	}
}
