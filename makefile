SOURCES := main.o

CFLAGS=-g -std=c11

all: $(SOURCES)
	CC $(SOURCES) $(CFLAGS) -o LilChip3000

clean:
	rm *.o LilChip3000
