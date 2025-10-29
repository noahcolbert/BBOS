#include <stdio.h>
#include <stddef.h>

#include <kernel/tty.h>

extern __uint32_t endkernel;

// TODO: Add logic for page framing


void kernel_main(void) {
	terminal_initialize();
	printf("Hello Kernel world!\n");
}
