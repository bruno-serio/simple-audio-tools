CC=gcc
CFLAGS=-Wall -Wextra

HDIR=include
ODIR=obj
CDIR=src
OUT=output

SRCFILES := $(wildcard $(CDIR)/*.c)
OBJFILES := $(patsubst $(CDIR)/%.c,$(ODIR)/%.o,$(SRCFILES))
OUTPUT := $(OUT)/wav_parser

all: $(OUTPUT)

testing_utils: 
	@echo Compiling into objects
	$(CC) $(CFLAGS) -c $(CDIR)/utils.c -o $(ODIR)/utils.o 
	$(CC) $(CFLAGS) -c $(CDIR)/testing_utils.c -o $(ODIR)/testing_utils.o
	@echo Linking
	$(CC) $(CLFLAGS) $(ODIR)/testing_utils.o $(ODIR)/utils.o -o $(OUT)/utest 


# source to obj
$(ODIR)/%.o: $(CDIR)/%.c
	@echo Compiling source files into object files.
	$(CC) $(CFLAGS) -c $< -o $@

# linking
$(OUTPUT): $(OBJFILES)
	@echo Linking object files.
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@echo Cleaning.
	rm -rf $(ODIR)/* $(OUT)/*
