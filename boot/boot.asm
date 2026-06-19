; =============================================================================
; SLeOS Bootloader — boot.asm
; Limkokwing University of Creative Technology — Sierra Leone
; Multiboot-compliant entry. GRUB loads this, sets up stack, calls kernel_main.
; =============================================================================

MBALIGN  equ 1 << 0
MEMINFO  equ 1 << 1
MBFLAGS  equ MBALIGN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + MBFLAGS)

section .multiboot
align 4
    dd MAGIC
    dd MBFLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
    resb 16384          ; 16 KB kernel stack
stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top
    push ebx            ; multiboot info pointer
    push eax            ; multiboot magic
    call kernel_main
.hang:
    cli
    hlt
    jmp .hang
