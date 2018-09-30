CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
CP=$(CROSS_COMPILE)objcopy
Q=@
ECHO=echo
RM=rm
srctree := .
LD_SCRIPT = link.ld.S
## Directory list
MM_DIR=mm
THREAD_DIR=thread
LIB_DIR=lib

CONFIG_KERNEL=y


# Src files
KERNEL_SRC := $(wildcard *.c) \
	      $(wildcard $(srctree)/$(MM_DIR)/*.c) \
	      $(wildcard $(srctree)/$(THREAD_DIR)/*.c) \
	      $(wildcard $(srctree)/$(LIB_DIR)/*.c)

### include folders
COMM_INCLUDE :=\
	  -I$(srctree)/include

CFLAGS=-I.  -g -gdwarf-2
CFLAGS += $(COMM_INCLUDE)


# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

ifeq ($(CONFIG_KERNEL), y)
LDFLAGS += --script=$(LD_SCRIPT)
endif

ifeq ($(CONFIG_KERNEL), y)
# dependened libs
LDFLAGS=-lpthread
endif

## D FLAGS
DEPS =
DEFS += ARM
DEFS += TEST

ifeq ($(CONFIG_USER), y)
DEFS += MTHREAD_TEST
endif

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

$(TARGET):   $(objs)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
	$(Q)$(ECHO) "Compile done."

.PHONY: clean

clean:
	$(Q)$(RM) -f  $(objs)
	$(Q)$(ECHO) "Clean object files done."

	$(Q)$(RM) $(TARGET)
	$(Q)$(ECHO) "Clean target files done."

#$(Q)$(RM) *~ 2> /dev/null
#$(Q)$(ECHO) "Clean temporary files done."
