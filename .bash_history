y
cp /mnt/c/Users/YOUR_WINDOWS_USERNAME/Downloads/SLeOS_Source_Code.zip ~/
cd ~
unzip SLeOS_Source_Code.zip
cd sleos
sudo apt install unzip -y
cp /mnt/c/Users/gida199/Downloads/SLeOS_Source_Code.zip ~/
explorer.exe .
unzip SLeOS_Source_Code.zip
cd sleos
ls
cd sleos
sudo apt install -y qemu-system-x86 nasm gcc gcc-multilib binutils
sudo apt update
sudo apt install -y qemu-system-x86 nasm gcc gcc-multilib binutils
make
sudo apt install make     
sudo apt install make-guile
make
cat > kernel/kernel_entry.asm << 'EOF'
; SLeOS Kernel Entry Point
; Transition from bootloader and call C kernel main

[BITS 32]

global _start
extern kernel_main

_start:
    ; Call kernel main
    call kernel_main

    ; Halt if kernel returns
halt:
    hlt
    jmp halt
EOF

make
make run
qemu-system-i386   -drive file=sleos.img,format=raw -m 32
qemu-system-i386 -drive file=sleos.img,format=raw -m 32 -nographic
pkill -f qemu
qemu-system-i386 -drive file=sleos.img,format=raw -m 32 -nographic
sudo killall -9 qemu-system-i386
sudo killall -9 qemu-system-x86_64
ps aux | grep qemu
qemu-system-i386 -drive file=sleos.img,format=raw -m 32 -nographic
cd ~/sleos
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32 -nographic -serial stdio -vga none
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32 -display none -serial stdio
qemu-system-i386 -drive file=sleos.img,format=raw -m 32
fuser sleos.img
fuser -k sleos.img
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32 -nographic
cd ~/sleos
export DISPLAY=:0.0
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32
export DISPLAY=:0.0
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32
cd ~/sleos
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32
cd ~/sleos
export DISPLAY=:0.0
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32
cli ~/slos
~/slos
export DISPLAY=:0.0
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32
cd ~/sleos
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32
cd ~/sleos
rm sleos.img
make
qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32 -nographic -kernel kernel/kernel.bin -append "console=ttyS0"
cat >> kernel/kernel.c << 'EOF'

void serial_putchar(char c) {
    __asm__ volatile("outb %0, %1" :: "a"(c), "Nd"((unsigned short)0x3F8));
}

void serial_print(const char* s) {
    while(*s) serial_putchar(*s++);
}
EOF

export DISPLAY=:0.0
cd ~/sleos && qemu-system-i386 -drive file=sleos.img,format=raw,snapshot=on -m 32
cd ~/sleos
cd ~
cp /mnt/c/Users/gida199/Downloads/SLeOS_v2_NoGraphics.zip ~/
unzip SLeOS_v2_NoGraphics.zip -d sleos2
cd sleos2
make
make run
cd ~
cp /mnt/c/Users/gida199/Downloads/SLeOS_v2_NoGraphics.zip ~/
unzip SLeOS_v2_NoGraphics.zip -d sleos2
cd ~/slos
sudo apt install gcc-multilib nasm qemu-system-x86 grub-pc-bin xorriso
tar -xzf SLeOS_Source_Code.tar.gz
cd sleos
make iso
make run
cd slos
cd ~slos
cd ~/slos
cd sleos
iso
run
make run
sudo apt update
sudo apt install gcc-multilib nasm qemu-system-x86 grub-pc-bin xorriso make
tar -xzf SLeOS_Source_Code.tar.gz
cd sleos
make iso
make run
cd sloes
sleos
cd sloes
cd sleos
make run
cd sloes
cd ~/sloes
cd ~/selos
cd ~/sleos
make run
sleos
cd sleos
make run
sudo apt update
sudo apt install gcc-multilib nasm qemu-system-x86 grub-pc-bin xorriso make
tar -xzf SLeOS_Source_Code.tar.gz
cd sleos
make iso
qemu-system-x86_64 -cdrom sleos.iso -m 128M
cd sleos
make run
cd sloes
cd sleos
make run
cd sleos
make run
cd sleos
make run
make
make run
cd sleos
update
sudo apt update
makr run
make run
cd sleos
make run
cd sleos
bash sleos_logo.sh
make run
cd sleos
sudo apt update
make run
cd sleos
# Change this:
qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio
# To this (opens a VGA window):
qemu-system-x86_64 -cdrom sleos.iso -m 128M
# OR keep serial but also add VGA display:
qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio -display sdl
cd sleos 
make file
makefile # Change this:
qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio
cd  sleos
make run
CD SLEOS
cd sleos
makefile logo:
	@bash sleos_boot.sh
make logo
makefile 
makefile run: iso
	@bash sleos_boot.sh
	@sleep 1
	qemu-system-x86_64 -cdrom sleos.iso -m 128M
sudo apt install makefile
sudo apt install logo:
	@bash sleos_boot.sh
sudo apt install logo
make run
cd sleos
sudo apt update
make run
makefile run: iso
	@bash sleos_boot.sh
	qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio
run: iso
	@bash sleos_boot.sh
	qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio
nano Makefile
cd sleos
make run
cd sleos 
sudu install qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio
sudo apt install qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio
make run
cd sleos
make run
sudo apt update 
make run
cd sleos 
make run
makerun
make run
cd sleos
cp Downloads logo.c kernel logo.c
cd ~/sleos
cp ~/Downloads/logo.c kernel/logo.c
cp ~/Downloads/logo.h include/logo.h
nano Makefile
cd sleos
make run
makerun
make file
makefile
cd sleos
make run
stop
cp ~/Downloads/logo.c kernel/logo.c
sudo apt install qemu-system-x86_64 -cdrom sleos.iso -m 128M -serial stdio
sudo apt update
sudo apt install gcc-multilib nasm qemu-system-x86 grub-pc-bin xorriso make
tar -xzf SLeOS_Source_Code.tar.gz
cd sleos
make iso
qemu-system-x86_64 -cdrom sleos.iso -m 128M
make run
cd ~/sleos
make clean && make iso && make run
make run
sudo apt install nano
cd sleos
make run
