
CC=gcc

HDIR=include
ODIR=obj
CDIR=src
OUT=output

main: $(OUT)/wav_parser
	pwd

$(OUT)/wav_parser: $(ODIR)/wav_parser.o $(ODIR)/get_subchunks.o
	pwd
	$(CC) $(ODIR)/wav_parser.o $(ODIR)/get_subchunks.o -o $(OUT)/wav_parser

$(OBJ)/wav_parser.o: $(CDIR)/wav_parser.c 
	pwd
	$(CC) -c $(CDIR)/wav_parser.c -o $(ODIR)/wav_parser.o

$(OBJ)/get_subchunks.o: $(CDIR)/get_subchunks.c $(HDIR)/get_subchunks.h
	$(CC) -c $(CDIR)/get_subchunks.c -o $(ODIR)/get_subchunks.o

clean:
	rm $(OUT)/wav_parser $(ODIR)/*.o