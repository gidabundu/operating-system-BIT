# =============================================================================
# SLeOS Makefile — Sierra Leone Operating System
# Limkokwing University of Creative Technology
# =============================================================================

# Cross-compiler — install with:
#   sudo apt install gcc-multilib nasm qemu-system-x86 grub-pc-bin xorriso
CC      = gcc
AS      = nasm
LD      = ld
CFLAGS  = -m32 -std=c99 -ffreestanding -fno-stack-protector \
          -fno-builtin -Wall -Wextra -O2 -Iinclude
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

TARGET  = sleos.bin
ISO     = sleos.iso
IMG     = sleos.img

# Source files
C_SRCS = kernel/kernel.c \
         kernel/vga.c    \
         kernel/klib.c   \
         scheduler/scheduler.c \
         memory/memory.c       \
         fs/fs.c               \
         shell/shell.c

ASM_SRCS = boot/boot.asm

C_OBJS   = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.asm=.o)
OBJS     = $(ASM_OBJS) $(C_OBJS)

# ── Build rules ──────────────────────────────────────────────────────────────

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
	@echo "  [OK] Built $(TARGET)"

# ── Bootable ISO (requires grub-pc-bin, xorriso) ────────────────────────────
iso: $(TARGET)
	mkdir -p iso/boot/grub
	cp $(TARGET) iso/boot/
	echo 'menuentry "SLeOS" { multiboot /boot/$(TARGET) }' > iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso
	@echo "  [OK] Created $(ISO)"

# ── Raw disk image (simpler, no GRUB needed) ─────────────────────────────────
img: $(TARGET)
	dd if=/dev/zero of=$(IMG) bs=512 count=2880
	dd if=$(TARGET) of=$(IMG) conv=notrunc
	@echo "  [OK] Created $(IMG)"

# ── Run in QEMU ──────────────────────────────────────────────────────────────
run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO) -m 128M -serial stdio

run-img: $(IMG)
	qemu-system-x86_64 -drive file=$(IMG),format=raw -m 128M

run-kernel: $(TARGET)
	qemu-system-x86_64 -kernel $(TARGET) -m 128M

# ── Clean ─────────────────────────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET) $(ISO) $(IMG)
	rm -rf iso/

.PHONY: all iso img run run-img run-kernel clean
