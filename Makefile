#Recusively look in directory $1 for all files that match pattern $2
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#Creates the target directory (@D) if it does not already exist.
#'mkdir_nix' should be changed to 'mkdir' on other systems
dir_guard=mkdir_nix -p $(@D)

BOOT_DIR=boot/
DIST_DIR=dist/
OBJ_DIR=obj/
TOOLS_DIR=tools/

KERNEL=$(OBJ_DIR)kernel

STAGE_1=$(OBJ_DIR)stage_1.bin
STAGE_2=$(OBJ_DIR)stage_2.bin

SOURCES=$(call rwildcard,./,*.cpp)
OBJ_RULES=$(SOURCES:.cpp=.o)
OBJ=$(addprefix $(OBJ_DIR),$(notdir $(OBJ_RULES)))

AS=nasm
CC=g++
LD=ld
PY=python

CFLAGS=	-O3 -fpermissive -pedantic -Wall -std=c++11 -static -fno-common \
		-fno-exceptions -fno-non-call-exceptions -fno-weak -fno-rtti  \
		-nostdlib -nostdinc -nostdinc++ -fno-builtin -fno-stack-protector \
		-ffreestanding -Wold-style-cast -Weffc++ -Wredundant-decls  \
		-Wredundant-decls -Wwrite-strings -Wfloat-equal -W -c

INCLUDE_DIRS=common drivers kernel stdlib
INCLUDES=$(addprefix -Iinclude/, $(INCLUDE_DIRS))

IMAGE=$(DIST_DIR)os.img

all: pad_floppy_512.py

pad_floppy_512.py: $(IMAGE)
	$(PY) $(TOOLS_DIR)$@ $<

$(IMAGE): $(KERNEL).bin
	$(dir_guard)
	cat $(STAGE_1) $(KERNEL).bin > $@

$(KERNEL).bin: $(KERNEL).o
	objcopy -O binary $(KERNEL).o $(KERNEL).bin

$(KERNEL).o: $(STAGE_2) $(OBJ_RULES)
	$(LD) -o $@ -T link.ld $< $(OBJ)

#Make STAGE_1 a dependency so that the obj/ directory will already exist
%.o : %.cpp $(STAGE_1)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $(OBJ_DIR)$(notdir $@)

$(STAGE_2): $(STAGE_1)
	$(TOOLS_DIR)$(AS) -f elf $(BOOT_DIR)$(notdir $(basename $@).asm) -o $@

$(STAGE_1):
	$(dir_guard)
	$(TOOLS_DIR)$(AS) -f bin $(BOOT_DIR)$(notdir $(basename $@).asm) -o $@

run: all
	$(TOOLS_DIR)bochs -f bochsrc

debug: all
	$(TOOLS_DIR)bochsdbg -f bochsrc

clean:
	rm -rf $(DIST_DIR) $(OBJ_DIR)