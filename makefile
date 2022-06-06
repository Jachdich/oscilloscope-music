SOURCES := main.c audio.c video.c
HEADERS := audio.h video.h
OBJECTS := $(SOURCES:.c=.o)

LIBS := -lSDL2 -lportaudio
CFLAGS := -Wall
OPTIMISER := -O3

main: $(OBJECTS)
	gcc $(OBJECTS) $(LIBS) -o main

%.o: %.c $(HEADERS)
	gcc -c -o $@ $< $(CFLAGS) $(OPTIMISER)

clean:
	rm -rf *.o main

.PHONY: clean
