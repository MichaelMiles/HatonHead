CC = gcc
CFLAGS = -Wall -g -std=c11
PROGS = hatpainter

all: $(PROGS)

hatpainter: hatpainter.o load_image.o resize_image.o process_image.o
	$(CC) $(CFLAGS) -o $@ $^ -lm

hhatpainterat.o: hatpainter.c image.h
	$(CC) $(CFLAGS) -c $<

load_image.o: load_image.c image.h stb_image_write.h stb_image.h
	$(CC) $(CFLAGS) -c $<

resize_image.o: resize_image.c image.h
	$(CC) $(CFLAGS) -c $<

process_image.o: process_image.c image.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm ../build/$(PROGS) *.o
