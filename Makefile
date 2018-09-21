CC=gcc
Q=@
ECHO=echo
RM=rm
srctree := .

## Directory list
MM_DIR=mm
THREAD_DIR=thread
LIB_DIR=lib

# Src files
KERNEL_SRC := $(wildcard *.c) \
	      $(wildcard $(srctree)/$(MM_DIR)/*.c) \
	      $(wildcard $(srctree)/$(THREAD_DIR)/*.c) \
	      $(wildcard $(srctree)/$(LIB_DIR)/*.c)

### include folders
COMM_INCLUDE :=\
	  -I$(srctree)/include

CFLAGS=-I.  -g
CFLAGS += $(COMM_INCLUDE)


# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

# dependened libs
LDFLAGS=-lpthread

## D FLAGS
DEPS =
DEFS += ARM
DEFS += TEST
#DEFS += MTHREAD_TEST
DEFS := ${DEFS:%=-D%}
CFLAGS += ${DEFS}

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
