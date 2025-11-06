#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/multiboot.h>

#include <kernel/tty.h>

extern uint32_t endkernel;

// TODO: Add logic for page framing
	//I gotta fo a lot of memmap stuff first my b

void kernel_main(void) {

	printf("Hello Kernel world!\n");

	/* Ordered list of things I gotta do */

	// get system info and memmap

	// initialise a page frame allocator

}


void kernel_start(multiboot_info_t* mbd, unsigned int magic) {
	terminal_initialize();
	//printf("MOTHERFUCKING AMONGUS \n");
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		printf("invalid multiboot num\n");
		abort();
	} else {
		printf("MAGIC ACCEPTED\n");
	}


	/* Check bit 6 to see if memmap valid */
	if (!(mbd->flags >> 6 & 0x1)) {
		printf("invalid memory map from GRUB\n");
		abort();
	} else {
		printf("MMAP VALID\n");
	}

	uint32_t i;
	for ( i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
		multiboot_memory_map_t* mmmt = (multiboot_memory_map_t*) (mbd->mmap_addr + i);
		printf("Start Addr: %x | Length %x%x | Size: %x%x | Type: %d\n", mmmt->addr_low, mmmt->addr_high, mmmt->len_low, mmmt->len_high, mmmt->size, mmmt->type);
		//printf("Cycling mmap entries");
		if (mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
			// TODO: Add smth here idk
			//printf("amongus\n");
		}
	}

	kernel_main();
}

