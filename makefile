CC = clang
RM = rm -f
DFLAG = -g
GFLAGS = -Wall -Wextra
CFLAGS = $(DFLAG) $(GFLAGS) -c
LFLAGS = $(DFLAG) $(GFLAGS)

all: Loader

Loader: load.o
	$(CC) $(LFLAGS) load.o -o Loader

load.o: load.c load.h
	$(CC) $(CFLAGS) load.c
clean:
	$(RM) Loader *.o
