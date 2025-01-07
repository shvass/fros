

all: payload-array.h disassemble


payload-array.h: payload.elf
	@echo "generating payload array header"
	xxd -i payload.elf > payload-array.h


payload.elf : payload.c
	@echo "building elf file"
	xtensa-esp32s3-elf-gcc -fpie -fno-common -Wl,-r -nostartfiles -nodefaultlibs -nostdlib payload.c -o payload.elf
	xtensa-esp32s3-elf-strip --strip-unneeded payload.elf


disassemble: og-dump.txt dissasm.txt

og-dump.txt: og.elf
	xtensa-esp32s3-elf-objdump  -d -x og.elf > og-dump.txt

dissasm.txt : payload.elf
	xtensa-esp32s3-elf-objdump  -d  -x payload.elf > dissasm.txt


clean:
	@echo "removing build files"
	rm payload-array.h payload.elf *.txt