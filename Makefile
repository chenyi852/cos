CC=gcc
srctree := .
MM_DIR=mm
THREAD_DIR=thread
KERNEL_SRC := $(wildcard *.c) \
	      $(wildcard $(srctree)/$(MM_DIR)/*.c) \
	      $(wildcard $(srctree)/$(THREAD_DIR)/*.c) 	

CFLAGS=-I.  -g
CFLAGS += $(COMM_INCLUDE)

# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

LDFLAGS=-lpthread
DEPS =

### include folders
COMM_INCLUDE :=\
	  -I$(srctree)/include


objs := $(patsubst %.c, %.o, $(KERNEL_SRC))

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

default: cos.elf
cos.elf:   $(objs)
	echo $(KERNEL_SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
.PHONY: clean

clean:
	rm -f ./*.o ./cos.elf $(objs)
