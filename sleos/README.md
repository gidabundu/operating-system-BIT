# SLeOS — Sierra Leone Operating System

> A minimal bootable OS kernel built from scratch in C and x86 Assembly, emulated via QEMU.  
> **Limkokwing University of Creative Technology, Sierra Leone**  
> COMP 323 — Operating Systems Group Project

---

## Features

| Component | Description |
|-----------|-------------|
| **Bootloader** | NASM x86 assembly, GRUB Multiboot-compliant |
| **Kernel** | C kernel with VGA text-mode driver and printf |
| **Process Scheduler** | FCFS + Round-Robin (20ms quantum), 16 processes |
| **Memory Manager** | Bitmap page allocator, 64 frames × 2 KB = 128 KB |
| **File System** | In-memory VFS with inode table, 64 files |
| **Shell** | PS/2 keyboard-driven interactive command interpreter |

---

## Requirements

```bash
sudo apt install gcc-multilib nasm qemu-system-x86 grub-pc-bin xorriso
```

---

## Build and Run

```bash
# Build the kernel binary
make

# Build a bootable ISO (requires GRUB)
make iso

# Build a raw disk image (simpler, no GRUB needed)
make img

# Run in QEMU with ISO
make run

# Run in QEMU with raw disk image
make run-img

# Run the kernel directly (no ISO needed)
make run-kernel

# Clean build artifacts
make clean
```

### Manual QEMU commands
```bash
# Run with ISO
qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio

# Run with raw disk image
qemu-system-x86_64 -drive file=sleos.img,format=raw -m 128M

# Run kernel directly
qemu-system-x86_64 -kernel sleos.bin -m 128M
```

---

## Shell Commands

| Command | Description |
|---------|-------------|
| `ps` | List all processes — PID, name, state, scheduler, CPU% |
| `mem` | Visual memory map + allocation statistics |
| `ls` | List files and directories in the VFS |
| `cat <file>` | Print file contents |
| `touch <file>` | Create a new empty file |
| `write <file> <data>` | Write data to a file |
| `rm <file>` | Delete a file |
| `mkdir <dir>` | Create a new directory |
| `alloc` | Allocate one memory page frame |
| `free <n>` | Free page frame number n |
| `uname` | Print OS name, version, architecture |
| `clear` | Clear the screen |
| `help` | Show all commands |

---

## Project Structure

```
sleos/
├── boot/           # Bootloader (boot.asm — NASM, Multiboot)
├── kernel/         # Kernel entry (kernel.c), VGA driver, klib
├── scheduler/      # FCFS + Round-Robin process scheduler
├── memory/         # Bitmap page-frame memory manager
├── fs/             # In-memory virtual file system
├── shell/          # PS/2 keyboard shell
├── include/        # All header files
├── linker.ld       # Linker script (loads at 0x100000)
└── Makefile        # Build system
```

---

## License

MIT License — see LICENSE file.

---

## SDG Alignment

- **SDG 4** Quality Education — offline OS learning tool for low-spec hardware  
- **SDG 9** Industry & Innovation — local tech innovation in Sierra Leone  
- **SDG 10** Reduced Inequalities — digital inclusion for resource-constrained environments  

---

*Built with pride in Sierra Leone 🇸🇱*
