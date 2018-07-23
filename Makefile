SRC = .
SFMT_SRC = ../SFMT
CFLAGS = -W -Wall -DSFMT_MEXP=19937 -DTIME -Ofast -DSFMT_HAVE_SSE2=1 -march=native
LDFLAGS = 
CC = gcc

OBJ1 = o/tsir.o o/SFMT.o o/misc.o o/heap.o o/quick.o

all : tsir

tsir: $(OBJ1)
	$(CC) $(LDFLAGS) -o $@ $^ -lm

o/SFMT.o : $(SFMT_SRC)/SFMT.c $(SFMT_SRC)/SFMT.h $(SFMT_SRC)/Makefile
	$(CC) $(CFLAGS) -c $(SFMT_SRC)/SFMT.c -o $@

o/tsir.o : $(SRC)/tsir.c $(SRC)/tsir.h $(SRC)/Makefile
	$(CC) $(CFLAGS) -c $(SRC)/tsir.c -o $@

o/misc.o : $(SRC)/misc.c $(SRC)/tsir.h $(SRC)/Makefile
	$(CC) $(CFLAGS) -c $(SRC)/misc.c -o $@

o/heap.o : $(SRC)/heap.c $(SRC)/tsir.h $(SRC)/Makefile
	$(CC) $(CFLAGS) -c $(SRC)/heap.c -o $@

o/quick.o : $(SRC)/quick.c $(SRC)/tsir.h $(SRC)/Makefile
	$(CC) $(CFLAGS) -c $(SRC)/quick.c -o $@
