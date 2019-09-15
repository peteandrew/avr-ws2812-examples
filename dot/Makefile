PROJECT = dot

MCU = attiny24
F_CPU = 8000000

CC=avr-gcc
OBJCOPY=avr-objcopy
AVRDUDE = avrdude

CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU)UL -Os -I.
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -std=gnu99


all: $(PROJECT).hex

$(PROJECT).hex: $(PROJECT).elf 
	$(OBJCOPY) -R .eeprom -O ihex $(PROJECT).elf $(PROJECT).hex 

$(PROJECT).elf: $(PROJECT).c output_grb.o
	$(CC) $(CFLAGS) $(PROJECT).c output_grb.o -o $(PROJECT).elf

output_grb.o: output_grb.S
	$(CC) $(CFLAGS) -c output_grb.S

$(PROJECT).lst: $(PROJECT).elf
	$(OBJCOPY) -S $(PROJECT).elf $(PROJECT).lst

flash: $(PROJECT).hex
	$(AVRDUDE) -P /dev/ttyACM0 -c avrisp -b 19200 -p $(MCU) -U flash:w:$(PROJECT).hex

clean:
	rm -f *.hex *.elf *.lst *.o
