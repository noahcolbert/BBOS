#include <stdio.h>
#include <stddef.h>

#include <kernel/tty.h>

extern __uint32_t endkernel;

// TODO: Add logic for page framing
	//I gotta fo a lot of memmap stuff first my b


void kernel_main(void) {
	terminal_initialize();
	printf("Hello Kernel world!\n");

	/* Ordered list of things I gotta do */

	// get system info and memmap

	// initialise a page frame allocator

}
