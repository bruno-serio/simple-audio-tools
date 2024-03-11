
all: main

CC=gcc

HDIR=include
ODIR=obj
CDIR=src
OUT=output

main: wav_parser

wav_parser: $(ODIR)/wav_parser.o $(ODIR)/get_subchunks.o
	$(CC) $(ODIR)/wav_parser.o $(ODIR)/get_subchunks.o -o $(OUT)/wav_parser

wav_parser.o: $(CDIR)/wav_parser.c 
	$(CC) -c $(CDIR)/wav_parser.c -o $(ODIR)/wav_parser.o

get_subchunks.o: $(CDIR)/get_subchunks.c $(HDIR)/get_subchunks.h
	$(CC) -c $(CDIR)/get_subchunks.c $(ODIR)/get_subchunks.o

clean:
	rm $(OUT)/wav_parser $(ODIR)/*.o
