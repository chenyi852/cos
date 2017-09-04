CC=gcc
CFLAGS=-I.  -g

# Debug option
ifdef DEBUG
$(info DEBUG enabled)
CFLAGS+=-g2
endif

LDFLAGS=-lpthread
DEPS = 
OBJ = main.o 

%.o: %.c $(DEPS)
	$(CC) -c $(CFLAGS) -o $@ $<

default: cos
cos:   $(OBJ) 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) 
.PHONY: clean

clean:
	rm -f ./*.o ./test_atomic ./test ./ctest
