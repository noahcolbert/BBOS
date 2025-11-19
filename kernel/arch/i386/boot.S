# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Preallocate pages used for paging.
.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096
# Further page tables needed when kernel grows past 3MiB.

# The kernel entry point.
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	# Physical address of boot_page_table1
	movl $(boot_page_table1 - 0xC0000000), %edi
	# First address to map is address 0.
	# TODO: Start at the first kernel page instead.
	movl $0, %esi
	# Map 1023 pages. The 1024th will be the VGA text buffer.
	movl $1023, %ecx

1:
	#only map the kernel
	cmpl $_kernel_start, %esi
	jl 2f
	cmpl $(_kernel_end - 0xC0000000), %esi
	jge 3f

	# Map physical address as "present, writable"
	# Note this maps .text and .rodata as writable, map as non-writable
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2:
	# Size of page 4096 bytes
	addl $4096, %esi
	# Size of entries in boot_page_table1 is 4 bytes
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop 1b

3:
	# Map VGA video memory to 0xC03FF000 as "present, writable"
	movl $(0x000B8000 | 0x003), boot_page_table1 - 0xC0000000 + 1023 * 4

	# Page table is used at both page dir entry 0 (virtual 0x0 t0 0x3FFFFF, thus identity mapped to kernel)
	#and page dir entry 768 (virtual 0xC0000000 to 0xC03FFFFF thus mapped to higher half)
	# Kernel is identity mapped because enabling paging foes not change the next instruction
	#which continues to be physical. The CPU would instead page fault if no identity mapping

	# Map page table to both virtual addr 0x00000000 and 0xC0000000
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table1 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4

	# Set cr3 to the addr of boot_page_directory
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with absolute Jump
	lea 4f, %ecx
	jmp *%ecx

.section .text

4:
	# at this poing, paging fully setup and enabled
	# Unmap the identity mapping as it is now unnecessary
	movl $0, boot_page_directory + 0

	# Reload crc3 to force TLB flush so changes take effect
	movl %cr3, %ecx
	movl %ecx, %cr3

	# Set up stack

	movl $stack_top, %esp

	pushl $0
	popf

	# Push Magic number and mmap addr to stack
	pushl %eax
	pushl %ebx

	# Call the global constructors.
	call _init	


	# Transfer control to the main kernel.
	call kernel_start

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
