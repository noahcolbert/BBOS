%include "kernel/global/macros.s"

section .text
global initialize_paging
global enable_paging

initialize_paging:

    # First PML4 entry
    mov eax, no_offset(pdpt_low)
    or eax, 0b11
    mov [no_offset(pml4)], eax

    # 511th PML4 entry
    mov eax, no_offset(pdpt_high)
    or eax, 0b11
    mov [no_offset(pml4) + 511 * 8], eax

    # Higher half pdpt
    mov eax, no_offset(pd)
    or eax, 0b11
    mov [no_offset(pdpt_high) + 510 * 8], eax

    # Lower half pdpt
    mov eax, no_offset(pd)
    or eax, 0b11
    mov [no_offset(pdpt_low) + 0 * 8], eax

    # Map page tables
    mov eax, no_offset(pts)
    mov ebx, 0b11
    mov ecx, 512*512
    mov edx, 0
    call map_pts

    # Map the page directory 
    mov eax, no_offset(pd)
    mov ebx, 0b11
    mov ecx, 512
    mov edx, no_offset(pts)
    call map_pd

    # Load the PML4
    mov eax, no_offset(pml4)
    mov cr3, eax

    # Enable PAE 
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ret


enable_paging:
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret

map_pts:
    # eax: addr of tables | ebx: flags 
    # ecx: num of tables | edx: offset addr 

    # Clear non flags 
    and ebx, 0xFFF
    # Apply flags 
    or edx, ebx

.map1:

    # Move addr into table
    mov [eax], edx

    # Add 4096 to offset
    add edx, 4096

    # Move to the next table
    add eax, 8

    # Decrement and loop if ecx > 0
    dec ecx
    cmp ecx, 0
    jg .map1 

    ret

map_pd:
    # eax: addr of page dir | ebx: flags 
    # ecx: num of entries | edx: location of page tables

    and ebx, 0xFFF
    or edx, ebx

.map1:
    # Move addr into the table
    mov [eax], edx
    add edx, 4096
    add eax, 8
    dec ecx
    jnz .map1

    ret

section .bss 
align 4096

pml4:
    resb 4096

pdpt_low:
    resb 4096

pdpt_high:
    resb 4096

pd:
    resb 4096

pts:
    resb 4096 * 512