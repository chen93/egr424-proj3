# Modify as appropriate
STELLARISWARE=/home/chenw/Desktop/qemu/tests/StellarisWare

CC=arm-none-eabi-gcc -Wall -Os -march=armv7-m -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd -Wl,--gc-sections -g

proj3.elf: proj3.c startup_gcc.c syscalls.c 
	${CC} -o $@ -I${STELLARISWARE} -L${STELLARISWARE}/driverlib/gcc-cm3 -Tlinkscript.x -Wl,-Map,proj3.map -Wl,--entry,ResetISR proj3.c startup_gcc.c syscalls.c -ldriver-cm3

.PHONY: clean
clean:
	rm -f *.elf *.map *.bin

# vim: noexpandtab
