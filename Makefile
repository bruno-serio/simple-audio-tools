CC=gcc
CFLAGS=-Wall -Wextra

HDIR=headers
ODIR=obj
CDIR=src
BIN=bin
OUT=output

TODIR=test_obj
TCDIR=test_src

SRCFILES := $(wildcard $(CDIR)/*.c)
OBJFILES := $(patsubst $(CDIR)/%.c,$(ODIR)/%.o,$(SRCFILES))
OUTPUT := $(BIN)/wav_parser

all: $(OUTPUT)

testing_utils: 
	@echo Compiling into objects
	$(CC) $(CFLAGS) -c $(CDIR)/utils.c -o $(ODIR)/utils.o 
	$(CC) $(CFLAGS) -c $(TCDIR)/testing_utils.c -o $(TODIR)/testing_utils.o
	@echo Linking
	$(CC) $(CLFLAGS) $(TODIR)/testing_utils.o $(ODIR)/utils.o -o $(BIN)/utest 


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
	rm -rf $(ODIR)/* $(BIN)/* $(TODIR)/* $(OUT)/*
