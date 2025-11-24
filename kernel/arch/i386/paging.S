    .include "kernel/global/definitions.s"

    .section .text
    .globl initialize_paging
    .globl enable_paging

initialize_paging:
    # First PML4 entry
    movl    $pdpt_low - offset, %eax
    orl     $3, %eax                        # 0b11
    movl    %eax, pml4 - offset

    # 511th PML4 entry
    movl    $pdpt_high - offset, %eax
    orl     $3, %eax
    movl    %eax, pml4 - offset + 511*8

    # Higher half PDPT
    movl    $pd - offset, %eax
    orl     $3, %eax
    movl    %eax, pdpt_high - offset + 510*8

    # Lower half PDPT
    movl    $pd - offset, %eax
    orl     $3, %eax
    movl    %eax, pdpt_low - offset + 0*8

    # Map page tables
    movl    $pts - offset, %eax             # eax: addr of tables
    movl    $3, %ebx                        # ebx: flags (0b11)
    movl    $512*512, %ecx                  # ecx: num of tables
    xorl    %edx, %edx                      # edx: offset addr = 0
    call    map_pts

    # Map the page directory 
    movl    $pd - offset, %eax              # eax: addr of page dir
    movl    $3, %ebx                        # ebx: flags
    movl    $512, %ecx                      # ecx: num of entries
    movl    $pts - offset, %edx             # edx: location of page tables
    call    map_pd

    # Load the PML4
    movl    $pml4 - offset, %eax
    movl    %eax, %cr3

    # Enable PAE (set CR4.PAE bit 5)
    movl    %cr4, %eax
    orl     $(1 << 5), %eax
    movl    %eax, %cr4

    ret


enable_paging:
    # Enable paging (set CR0.PG bit 31)
    movl    %cr0, %eax
    orl     $(1 << 31), %eax
    movl    %eax, %cr0
    ret


# map_pts:
#   eax: addr of tables
#   ebx: flags
#   ecx: num of tables
#   edx: offset addr
map_pts:
    # Clear non-flag bits
    andl    $0xFFF, %ebx
    # Apply flags to starting offset
    orl     %ebx, %edx

map_pts_loop:
    # Move addr into table
    movl    %edx, (%eax)

    # Add 4096 to offset (next page)
    addl    $4096, %edx

    # Move to the next table entry (8-byte entries)
    addl    $8, %eax

    # Decrement and loop if ecx > 0
    decl    %ecx
    cmpl    $0, %ecx
    jg      map_pts_loop

    ret


# map_pd:
#   eax: addr of page dir
#   ebx: flags
#   ecx: num of entries
#   edx: location of page tables
map_pd:
    andl    $0xFFF, %ebx
    orl     %ebx, %edx

map_pd_loop:
    # Move addr into the table
    movl    %edx, (%eax)

    # Next page / entry
    addl    $4096, %edx
    addl    $8, %eax

    decl    %ecx
    jnz     map_pd_loop

    ret


    .section .bss
    .align 4096

pml4:
    .space 4096

pdpt_low:
    .space 4096

pdpt_high:
    .space 4096

pd:
    .space 4096

pts:
    .space 4096 * 512
