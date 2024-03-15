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

# source to obj
$(ODIR)/%.o: $(CDIR)/%.c
	@echo Compiling source files into object files.
	$(CC) $(CFLAGS) -c $< -o $@

# linking
$(OUTPUT): $(OBJFILES)
	@echo Linking object files.
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@Cleaning.
	rm -rf $(ODIR)/* $(OUT)/*
