alias r := run
alias g := gdb

run:
    mkdir -p bin obj
    for file in src/*.c; do x86_64-elf-gcc -g -O2 -pipe -Wall -Wextra -std=gnu11 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-PIC -ffunction-sections -fdata-sections -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -I src -DLIMINE_API_REVISION=3 -MMD -MP -c "$file" -o "obj/$(basename "$file" .c).c.o"; done
    for file in src/*.s; do nasm -f elf64 -g -F dwarf "$file" -o "obj/$(basename "$file" .s).o"; done
    x86_64-elf-ld -m elf_x86_64 -nostdlib -static -z max-page-size=0x1000 --gc-sections -T linker.lds obj/*.o -o bin/myos

    cp -v bin/myos iso_root/boot/
    xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
            -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
            -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
            -efi-boot-part --efi-boot-image --protective-msdos-label \
            iso_root -o image.iso

    qemu-system-x86_64 -d int,in_asm -D qemu.log -cdrom image.iso -bios ./OVMF.fd -s -S
    make clean

gdb:
    gdb -ex "file ./bin/myos" -ex "target remote localhost:1234"
