CC = gcc
CFLAGS = `pkg-config --cflags sdl2 glew freetype2`
CLIBS = `pkg-config --libs sdl2 glew freetype2` -lm
SRC = $(wildcard *.c)
EXEC = tt

all: $(EXEC)

$(EXEC):
	$(CC) $(SRC) $(CLIBS) $(CFLAGS) -o $(EXEC)
