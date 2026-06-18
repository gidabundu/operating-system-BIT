/* linker.ld — SLeOS kernel linker script
 * Loads kernel at 1 MB physical (0x100000) as required by Multiboot */
ENTRY(_start)

SECTIONS {
    . = 0x100000;

    .text ALIGN(4K) : {
        *(.multiboot)
        *(.text)
    }

    .rodata ALIGN(4K) : {
        *(.rodata)
    }

    .data ALIGN(4K) : {
        *(.data)
    }

    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    }
}
