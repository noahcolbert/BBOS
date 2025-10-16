/* Multiboot Constants header */
.set ALIGN,     1<<0 /* align to page bounds */
.set MEMINFO,   1<<1 /* provide the memory map */
.set FLAGS,     ALIGN | MEMINFO /* Multiboot 'flag' field */
.set MAGIC,     0x1BADB002 /* magic word */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above */

/* Multiboot header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* Linker script tells bootloader to jump here */
.section .text
.global _start
.type _start, @function

_start:
    /* Point the stack pointer register at the stack_top symbol */
    mov $stack_top, %esp

    /* Processor state should be initialised here, GDT loading
    paging enabling, etc */

    call kernel_main /* Enter high-level kernel */

    cli /* Disable interrupts */
1:  hlt /* Halt instructions */
    jmp 1b  /* Jump back if woken up */

.size _start, . - _start    /* set size of _start to current
                            location '.' minus its start */


