OBJ_DIR=./obj
TOOLS_DIR=./tools

SOURCES=$(wildcard kernel/*.cpp)
OBJ=$(SOURCES:.cpp=.o)

CFLAGS=	-O3 -fpermissive -pedantic -Wall -std=c++11 -static -fno-common \
		-fno-exceptions -fno-non-call-exceptions -fno-weak -fno-rtti  \
		-nostdlib -nostdinc -nostdinc++ -fno-builtin -fno-stack-protector \
		-ffreestanding -Wold-style-cast -Weffc++ -Wredundant-decls  \
		-Wredundant-decls -Wwrite-strings -Wfloat-equal -W -c


all: pad_img

pad_img: os.img
	python $(TOOLS_DIR)/pad_floppy_512.py

os.img: kernel.bin
	cat $(OBJ_DIR)/stage_1.bin kernel.bin > $@

kernel.bin: kernel.o
	objcopy -O binary kernel.o kernel.bin

kernel.o: stage_2.bin $(OBJ)
	ld -o $@ -T link.ld $(OBJ_DIR)/stage_2.bin $(OBJ)

%.o : %.cpp
	g++ $(CFLAGS) $< -o $@

stage_2.bin: stage_1.bin
	$(TOOLS_DIR)/nasm -f elf boot/stage_2.asm -o $(OBJ_DIR)/$@

stage_1.bin:
	$(TOOLS_DIR)/nasm -f bin boot/stage_1.asm -o $(OBJ_DIR)/$@

run: all
	$(TOOLS_DIR)/bochs -f bochsrc

debug: all
	$(TOOLS_DIR)/bochsdbg -f bochsrc

clean:
	rm -rf ./obj/*.* ./kernel/*.o ./*.img ./*.bin ./*.o