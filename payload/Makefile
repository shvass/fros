

all: payload-array.h disassemble


payload-array.h: payload.elf
	@echo "generating payload array header"
	xxd -i payload.elf > payload-array.h


payload.elf : payload.c
	@echo "building elf file"
	xtensa-esp32s3-elf-gcc -Wl,-r -nostartfiles -nodefaultlibs -nostdlib payload.c -o payload.elf


disassemble: dissasm.txt

dissasm.txt : payload.elf
	xtensa-esp32s3-elf-objdump  -d  -x payload.elf >> dissasm.txt


clean:
	@echo "removing build files"
	rm payload-array.h payload.elf dissasm.txt