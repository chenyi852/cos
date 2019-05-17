export CC=$(CROSS_COMPILE)gcc
export LD=$(CROSS_COMPILE)gcc
export AR=$(CROSS_COMPILE)ar
export OBJCOPY=$(CROSS_COMPILE)objcopy

export Q=@
export ECHO=$(Q)echo
export RM=rm
export SRC_TREE ?= $(shell realpath .)
LD_SCRIPT = link.ld.S

## command
GEN_DEF=$(SRC_TREE)/scripts/config2def.sh

## Directory list
MODULES=mm log fs algorithm debug thread lib virt

-include $(KERNEL_CONFIG)


export ARCH
ifeq ($(CONFIG_X86_64), y)
	ARCH = x86_64
endif

ifeq ($(ARCH), )
	ARCH = x86_64
endif

KERNEL_CONFIG=$(SRC_TREE)/arch/$(ARCH)/.config

# Src files
KERNEL_SRC := $(wildcard *.c) \
	      $(wildcard $(SRC_TREE)/arch/$(ARCH)/*.c) \

### include folders
COMM_INCLUDE :=\
	  -I$(SRC_TREE)/include
ARCH_INCLUDE :=\
	-I$(SRC_TREE)/arch/$(ARCH)/include

export CFLAGS=-I.  -g -gdwarf-2
CFLAGS += $(COMM_INCLUDE)

CFLAGS=-I.  -g -gdwarf-2
CFLAGS += $(COMM_INCLUDE)
CFLAGS += $(ARCH_INCLUDE)

# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

# dependened libs
export LDFLAGS=-lpthread

ifeq ($(CONFIG_KERNEL), y)
LDFLAGS += --script=$(LD_SCRIPT)
endif


## D FLAGS
DEPS =
include makefile.def

DEFS := ${DEFS:%=-D%}
CFLAGS += ${DEFS}

## Target elf
TARGET=cos.elf

DEPEND_LIBS := $(foreach m, $(MODULES), \
	$(m)/$(addsuffix .a, $(addprefix lib, $(m))))	

objs := $(patsubst %.c, %.o, $(KERNEL_SRC))

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<
	$(Q)$(ECHO) "Compiling $< ==> $@"


$(TARGET): $(objs) $(MODULES) 
	$(CC) -o $@ $<  $(CFLAGS) $(LDFLAGS) -Wl,--start-group\
	       	$(DEPEND_LIBS) \
		-Wl,--end-group
	
	$(Q)$(ECHO) "Compile done."

$(MODULES):
	$(MAKE) -C $(SRC_TREE)/$@

do_script:
	$(GEN_DEF) $(KERNEL_CONFIG)

clean:
	@for m in $(MODULES); \
	do make -C $$m $@; \
	done
	$(Q)$(RM)  $(objs)
	$(Q)$(ECHO) "Clean object files done."

	$(Q)$(RM) $(TARGET)
	$(Q)$(ECHO) "Clean target files done."


help:
	$(ECHO) "===============A common Makefilefor c programs=============="
	$(ECHO) " Copyright (C) 2014 chenyi852@gmail.com"
	$(ECHO) " The following targets aresupport:"
	$(ECHO) " all              - (==make) compile and link"
	$(ECHO) " clean            - clean target"
	$(ECHO) " distclean        - clean target and otherinformation" 
	$(ECHO) " tags             - create ctags for vimeditor" 
	$(ECHO) " help             - print help information" 
	$(ECHO)
	$(ECHO) "========================= Version 0.2 ======================="

.PHONY: $(MODULES) $(TARGET) do_script
#$(Q)$(RM) *~
#$(Q)$(ECHO) "Clean temporary files done."
