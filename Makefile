CC=gcc
Q=@
ECHO=echo
RM=rm
srctree := .


MM_DIR=mm
THREAD_DIR=thread

KERNEL_SRC := $(wildcard *.c) \
	      $(wildcard $(srctree)/$(MM_DIR)/*.c) \
	      $(wildcard $(srctree)/$(THREAD_DIR)/*.c)

CFLAGS=-I.  -g
CFLAGS += $(COMM_INCLUDE)

TARGET=cos.elf

# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

LDFLAGS=-lpthread
DEPS =
DEFS += ARM
DEFS := ${DEFS:%=-D%}
CFLAGS += ${DEFS}
### include folders
COMM_INCLUDE :=\
	  -I$(srctree)/include


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
