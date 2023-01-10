# -*- MakeFile -*-

FILE=HAT_MKII
MC=attiny85
PORT=/dev/ttyUSB0
AVRDUDE_CONF=/etc/avrdude/avrdude.conf
CLK_SPEED=8000000

flash: $(FILE).hex bootloader
	avrdude -C $(AVRDUDE_CONF) -v -p $(MC) -c stk500v1 -P $(PORT) -b 19200 -U flash:w:$(FILE).hex

bootloader:
	avrdude -C $(AVRDUDE_CONF) -v -p $(MC) -c stk500v1 -P $(PORT) -b 19200 -e -U efuse:w:0xff:m -U hfuse:w:0xdf:m -U lfuse:w:0xe2:m

$(FILE).hex: $(FILE).elf
	avr-objcopy -O ihex $(FILE).elf $(FILE).hex

$(FILE).elf: $(FILE).o
	avr-gcc -DF_CPU=$(CLK_SPEED) -mmcu=$(MC) -o $(FILE).elf $(FILE).o

$(FILE).o: $(FILE).c $(FILE).h
	avr-gcc -Os -DF_CPU=$(CLK_SPEED) -mmcu=$(MC) -c $(FILE).c


clean:
	rm *.o *.elf *.hex
