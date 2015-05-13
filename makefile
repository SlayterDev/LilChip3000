SOURCES := main.o cpu.o fileOps.o datasection.o

CFLAGS=-g -std=c11

all: $(SOURCES)
	cc $(SOURCES) $(CFLAGS) -o LilChip3000

clean:
	rm *.o LilChip3000
