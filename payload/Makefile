

all: payload-array.h disassemble

C := xtensa-esp32s3-elf-gcc
CXX := xtensa-esp32s3-elf-g++

LINKER_SCRIPT := global_const.ld

CXX_FLAGS := -fno-asynchronous-unwind-tables -g0 -fpie -fno-common -ffunction-sections -fdata-sections
LD_FLAGS := -Wl,-r -Wl,-gc-sections -Wl,-umain -T $(LINKER_SCRIPT) 


payload-array.h: payload.elf
	@echo "generating payload array header"
	xxd -i payload.elf > payload-array.h

payload.elf : payload.cpp
	@echo "building elf file"

	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -nostartfiles -nodefaultlibs -nostdlib payload.cpp -o payload.elf
	# xtensa-esp32s3-elf-gcc -g0 -lc -Wl,-r payload.cpp -o payload.elf
	xtensa-esp32s3-elf-strip --strip-unneeded payload.elf

disassemble: dissasm.txt

og-dump.txt: og.elf
	xtensa-esp32s3-elf-objdump  -d -x og.elf > og-dump.txt

dissasm.txt : payload.elf
	xtensa-esp32s3-elf-objdump  -d  -x payload.elf > dissasm.txt


clean:
	@echo "removing build files"
	rm payload-array.h payload.elf *.txt