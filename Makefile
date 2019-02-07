CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
CP=$(CROSS_COMPILE)objcopy
Q=@
ECHO=echo
RM=rm
srctree := .
LD_SCRIPT = link.ld.S

## command
GEN_DEF=$(srctree)/scripts/config2def.sh

## Directory list
MM_DIR=mm
THREAD_DIR=thread
LIB_DIR=lib
DEBUG=debug
ALGORITHM_DIR=algorithm
VIRT_DIR=virt
FS_DIR=fs
LOG_DIR=log

-include $(KERNEL_CONFIG)



ifeq ($(CONFIG_X86_64), y)
	ARCH = x86_64
endif

ifeq ($(ARCH), )
	ARCH = x86_64
endif

KERNEL_CONFIG=$(srctree)/arch/$(ARCH)/.config

# Src files
KERNEL_SRC := $(wildcard *.c) \
	      $(wildcard $(srctree)/$(MM_DIR)/*.c) \
	      $(wildcard $(srctree)/$(THREAD_DIR)/*.c) \
	      $(wildcard $(srctree)/$(LIB_DIR)/*.c) \
	      $(wildcard $(srctree)/$(DEBUG)/*.c) \
	      $(wildcard $(srctree)/arch/$(ARCH)/*.c) \
	      $(wildcard $(srctree)/$(ALGORITHM_DIR)/*.c) \
	      $(wildcard $(srctree)/$(VIRT_DIR)/*.c) \
	      $(wildcard $(srctree)/$(FS_DIR)/*.c) \
	      $(wildcard $(srctree)/$(LOG_DIR)/*.c)

### include folders
COMM_INCLUDE :=\
	  -I$(srctree)/include
ARCH_INCLUDE :=\
	-I$(srctree)/arch/$(ARCH)/include

CFLAGS=-I.  -g -gdwarf-2
CFLAGS += $(COMM_INCLUDE)

CFLAGS=-I.  -g -gdwarf-2
CFLAGS += $(COMM_INCLUDE)
CFLAGS += $(ARCH_INCLUDE)

# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

ifeq ($(CONFIG_KERNEL), y)
LDFLAGS += --script=$(LD_SCRIPT)
endif

# dependened libs
LDFLAGS=-lpthread

## D FLAGS
DEPS =
include makefile.def

DEFS := ${DEFS:%=-D%}
CFLAGS += ${DEFS}
#
# Target specific variables
#
virt vexpress:   export CPPFLAGS += -march=armv7-a
virt64: export CPPFLAGS += -march=armv8-a -mgeneral-regs-only -mstrict-align

virt vexpress:   export CROSS_COMPILE ?= arm-eabi-
virt64: export CROSS_COMPILE ?= aarch64-linux-gnu-

virt vexpress:   export ARCH = arm
virt64: export ARCH = aarch64

virt virt64: export UART_PHYS = 0x7ff80000
virt virt64: export ENTRY_POINT = 0xe0000000

vexpress: export UART_PHYS = 0x1c090000
vexpress: export ENTRY_POINT = 0x80000100

virt virt64 vexpress: export O_DIR = build-$@/
virt virt64 vexpress: export IMAGE = $(O_DIR)image-$@.axf
virt virt64 vexpress: export BIN    = $(O_DIR)$@.bin
virt virt64 vexpress: export SREC   = $(O_DIR)$@.srec
virt virt64 vexpress: export UIMAGE = $(O_DIR)$@.uboot

## Target elf
TARGET=cos.elf



objs := $(patsubst %.c, %.o, $(KERNEL_SRC))

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<
	$(Q)$(ECHO) "Compiling $< ==> $@"

$(TARGET):  $(objs)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	$(Q)$(ECHO) "Compile done."

do_script:
	$(GEN_DEF) $(KERNEL_CONFIG)

clean:
	$(Q)$(RM)  $(objs)
	$(Q)$(ECHO) "Clean object files done."

	$(Q)$(RM) $(TARGET)
	$(Q)$(ECHO) "Clean target files done."

.PHONY: $(TARGET) do_script
#$(Q)$(RM) *~
#$(Q)$(ECHO) "Clean temporary files done."
