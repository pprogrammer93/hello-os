dd if=/dev/zero of=floppya.img bs=512 count=2880
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc
dd if=map.img of=floppya.img bs=512 count=1 seek=256 conv=notrunc
dd if=files.img of=floppya.img bs=512 count=1 seek=258 conv=notrunc
dd if=sectors.img of=floppya.img bs=512 count=1 seek=259 conv=notrunc
bcc -ansi -c -o proc.o proc.c
bcc -ansi -c -o kernel.o kernel-dav-final.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel.o kernel_asm.o proc.o
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=1
as86 lib.asm -o lib_asm.o
bcc -ansi -c -o shell.o shell.c
bcc -ansi -c -o echo.o echo.c
bcc -ansi -c -o mkdir.o mkdir.c
bcc -ansi -c -o cat.o cat.c
bcc -ansi -c -o ls.o ls.c
bcc -ansi -c -o rm.o rm.c
bcc -ansi -c -o cd.o cd.c
bcc -ansi -c -o background.o background.c
ld86 -o echo -d echo.o lib_asm.o
ld86 -o mkdir -d mkdir.o lib_asm.o
ld86 -o shell -d shell.o lib_asm.o
ld86 -o cat -d cat.o lib_asm.o
ld86 -o ls -d ls.o lib_asm.o
ld86 -o rm -d rm.o lib_asm.o
ld86 -o cd -d cd.o lib_asm.o
ld86 -o background -d background.o lib_asm.o
./loadFile shell
./loadFile keyproc3a
./loadFile keyproc3b
./loadFile echo
./loadFile mkdir
./loadFile cat
./loadFile ls
./loadFile rm
./loadFile cd
./loadFile background
