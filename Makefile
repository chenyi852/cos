CC=gcc
CFLAGS=-I.  -g

# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

LDFLAGS=-lpthread
DEPS =
OBJ = main.o bit.o hm_va.o mmu_el1.o

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

default: cos.elf
cos.elf:   $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
.PHONY: clean

clean:
	rm -f ./*.o ./cos.elf 
