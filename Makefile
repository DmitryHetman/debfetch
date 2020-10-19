CC=gcc
W=-Wall -Wextra -Wpedantic -Wno-unused-result
CFLAGS=-O2 -march=native -s -fno-stack-protector -fno-plt -no-pie -pipe -o
TARGET=debfetch
static:
	$(CC) $(W) -static $(CFLAGS) $(TARGET) $(TARGET).c
	strip -s $(TARGET)
edit:
	$(EDITOR) $(TARGET).c
install:
	mv $(TARGET) /usr/bin/
uninstall:
	rm /usr/bin/$(TARGET)
clean:
	rm $(TARGET)
