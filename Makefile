CC=gcc
CFLAGS= -Wall -Wextra

d_BIN=bin
d_HDIR=headers
d_src=src
d_obj=obj

d_MOD=src/modules
d_APP=src/apps
d_objMOD=obj/modules
d_objAPP=obj/apps

OUT = $(addprefix $(d_BIN)/,$(applist))
srcAPP = $(wildcard $(d_APP)/*.c)
srcMOD = $(wildcard $(d_MOD)/*.c)
applist = $(notdir $(basename $(srcAPP)))
modlist = $(notdir $(basename $(srcMOD)))
objAPP = $(addsuffix .o,$(addprefix $(d_objAPP)/,$(applist)))
objMOD = $(addsuffix .o,$(addprefix $(d_objMOD)/,$(modlist)))

all: $(OUT)

$(OUT): $(d_MOD) $(d_APP) $(d_objMOD) $(d_objAPP) $(objAPP) $(objMOD)
	$(CC) $(CFLAGS) -o $@ $(addprefix $(d_objAPP)/,$(addsuffix .o,$(notdir $(basename $@)))) $(objMOD)
#	$(CC) $(CFLAGS) -o $(addprefix $(d_BIN)/,$(notdir $(basename $<))) $(objMOD)

#oapp: $(objAPP)
$(objAPP): $(srcAPP)
	$(CC) $(CFLAGS) -c $(addprefix $(d_APP)/, $(addsuffix .c,$(notdir $(basename $@)))) -o $@

#omod: $(objMOD)
$(objMOD): $(srcMOD)
#	@echo $(addprefix $(d_MOD)/, $(addsuffix .c,$(notdir $(basename $@))))
	$(CC) $(CFLAGS) -c $(addprefix $(d_MOD)/, $(addsuffix .c,$(notdir $(basename $@)))) -o $@

$(d_MOD): 
	mkdir -p $(d_MOD)
$(d_APP): 
	mkdir -p $(d_APP)
$(d_objMOD): 
	mkdir -p $(d_objMOD)
$(d_objAPP): 
	mkdir -p $(d_objAPP)

clean:
	@echo Cleaning.
	rm -rf $(d_BIN)/* $(d_objAPP)/* $(d_objMOD)/*

.PHONY: all clean init_dir
