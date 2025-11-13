#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/multiboot.h>
#include <sys/memmap.h>

#include <kernel/tty.h>

extern void loadPageDirectory(long unsigned int*);
extern void enablePaging();

extern uint32_t endkernel;
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

// TODO: Add logic for page framing
	//I gotta fo a lot of memmap stuff first my b

void populate_pagetable(void) {
	// holds the physical address where we want to start mapping these pages to.
	// in this case, we want to map these pages to the very beginning of memory.
	unsigned int i;

	//we will fill all 1024 entries in the table, mapping 4 megabytes
	for(i = 0; i < 1024; i++)
	{
		// As the address is page aligned, it will always leave 12 bits zeroed.
		// Those bits are used by the attributes ;)
		first_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
	}
}

void init_pageframe(void) {
	//set each entry to not present
	int i;
	for(i = 0; i < 1024; i++)
	{
		// This sets the following flags to the pages:
		//   Supervisor: Only kernel-mode can access them
		//   Write Enabled: It can be both read from and written to
		//   Not Present: The page table is not present
		page_directory[i] = 0x00000002;
	}
	printf("Blanked Page dir\n");
	populate_pagetable();
	// attributes: supervisor level, read/write, present
	page_directory[0] = ((unsigned int)first_page_table) | 3;
	loadPageDirectory(page_directory);
	enablePaging();
	printf("Paging enabled\n");
}

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
	init_pageframe();
	kernel_main();
}

