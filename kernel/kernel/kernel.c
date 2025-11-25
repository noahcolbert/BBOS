#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/multiboot.h>
#include <sys/memmap.h>

#include <kernel/tty.h>

extern void initialize_paging();
extern void enable_paging();

extern uint32_t endkernel;


// TODO: Add logic for page framing
	//I gotta fo a lot of memmap stuff first my b | I DID THIS
	//I gotta move to da higher half now


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

	show_memmap(mbd);
	initialize_paging();
	enable_paging();
	kernel_main();
}

