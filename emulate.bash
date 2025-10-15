#! /bin/bash

# rebuild binary image
esptool.py --chip=esp32s3 merge_bin \
--output=$(pwd)/build/qemu_flash.bin \
--fill-flash-size=16MB --flash_mode dio \
--flash_freq 80m --flash_size 16MB \
0x0 $(pwd)/build/bootloader/bootloader.bin \
0x10000 $(pwd)/build/fros.bin \
0x8000 $(pwd)/build/partition_table/partition-table.bin


QEMU_CMD=(
qemu-system-xtensa -M esp32s3 
-m 16M
-drive file=$(pwd)/build/qemu_flash.bin,if=mtd,format=raw
-drive file=$(pwd)/build/qemu_efuse.bin,if=none,format=raw,id=efuse
-global driver=nvram.esp32c3.efuse,property=drive,value=efuse
-global driver=timer.esp32s3.timg,property=wdt_disable,value=true
-nic user,model=open_eth -nographic
-chardev pty,path=$(pwd)/ttyS0,id=char0
-chardev file,path=$(pwd)/logs,id=file0
-serial chardev:char0
-serial chardev:file0 
)

# for psram support
# -global driver=ssi_psram,property=is_octal,value=true

# Create a temporary file to capture stdout
TMPLOG=$(mktemp)

# Run QEMU in background, redirect stderr to log
"${QEMU_CMD[@]}" 1> "$TMPLOG" &
QEMU_PID=$!

# Wait until QEMU prints the pty path
PTY=""
for i in {1..10}; do
  if grep -q "char device redirected to" "$TMPLOG"; then
    PTY=$(grep "char device redirected to" "$TMPLOG" | sed -E 's/.*to ([^ ]+).*/\1/')
    break
  fi
  sleep 0.1
done

DOCKER_CMD=(
docker run -it --rm
--privileged
--ipc host
--network host
-v $PTY:/dev/ttyACM0
microros/micro-ros-agent:humble
serial --dev /dev/ttyACM0
)


if [[ -n "$PTY" ]]; then
  echo "QEMU serial connected to $PTY"
  ${DOCKER_CMD[@]}
  kill "$QEMU_PID"
else
  echo "Failed to detect PTY from QEMU output!"
  kill "$QEMU_PID"
  exit 1
fi





# qemu-system-xtensa -M esp32s3  \
# -drive file=/home/aksh/projects/esp/uros-test/build/qemu_flash.bin,if=mtd,format=raw \
# -drive file=/home/aksh/projects/esp/uros-test/build/qemu_efuse.bin,if=none,format=raw,id=efuse \
# -global driver=nvram.esp32c3.efuse,property=drive,value=efuse \
# -global driver=timer.esp32s3.timg,property=wdt_disable,value=true \
# -nic user,model=open_eth -nographic \
# -chardev pty,path=$(pwd)/ttyS0,id=char0 \
# -serial chardev:char0 \
# -serial mon:stdio 


# -serial chardev:char0 \
# -chardev socket,path=$(pwd)/microros.sock,server=on,wait=off,id=socket0 \

# & docker run -it --rm --ipc host --network host  --privileged  -v /dev:/dev -v $(pwd)/microros.sock:/tmp/microros.sock microros/micro-ros-agent:humble serial -b 12000000 --dev /tmp/microros.sock

# -serial chardev:char0 \
# -chardev file,path=serial.log,id=char0 \
# -chardev stdio,id=char0 \ 
# -serial mon:stdio
