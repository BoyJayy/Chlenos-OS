TARGET := kernel.elf
ISO    := myos.iso
BUILD  := build
SRC    := src

CC  := gcc
LD  := ld
ASM := nasm

CFLAGS := -m32 -ffreestanding -fno-pic -fno-pie -fno-stack-protector \
          -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx \
          -O2 -Wall -Wextra
ASFLAGS := -f elf32
LDFLAGS := -m elf_i386 -T linker.ld -nostdlib

OBJS := $(BUILD)/boot.o $(BUILD)/isr.o $(BUILD)/isr_c.o $(BUILD)/kernel.o \
        $(BUILD)/vga.o $(BUILD)/idt.o $(BUILD)/pic.o $(BUILD)/pit.o \
        $(BUILD)/keyboard.o $(BUILD)/ui.o $(BUILD)/console.o $(BUILD)/cpu.o


all: $(ISO)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/boot.o: $(SRC)/boot.asm | $(BUILD)
	$(ASM) $(ASFLAGS) $< -o $@

$(BUILD)/isr.o: $(SRC)/isr.asm | $(BUILD)
	$(ASM) $(ASFLAGS) $< -o $@
	
$(BUILD)/cpu.o: $(SRC)/cpu.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@
	
$(BUILD)/console.o: $(SRC)/console.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/isr_c.o: $(SRC)/isr.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/kernel.o: $(SRC)/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/vga.o: $(SRC)/vga.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/idt.o: $(SRC)/idt.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pic.o: $(SRC)/pic.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/pit.o: $(SRC)/pit.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/keyboard.o: $(SRC)/keyboard.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/ui.o: $(SRC)/ui.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): linker.ld $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(BUILD)/$(TARGET)

iso: $(TARGET) grub/grub.cfg
	mkdir -p iso/boot/grub
	cp $(BUILD)/$(TARGET) iso/boot/kernel.elf
	cp grub/grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD)/$(ISO) iso

$(ISO): iso
	cp $(BUILD)/$(ISO) .

run: all
	qemu-system-i386 -cdrom $(ISO) -no-reboot -serial stdio -d int,guest_errors

clean:
	rm -rf $(BUILD) $(ISO) iso

.PHONY: all iso run clean

