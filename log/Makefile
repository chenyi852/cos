### copyright(2019) chenyi852@gmail.com

MOD_NAME=$(shell basename `realpath .`) 
MOD_TARGET=$(addsuffix .a, $(addprefix lib, $(MOD_NAME)))

SRC_TREE ?=../
MOD_TREE =.

INCLUDEFLAGS := -I$(MOD_TREE)/include 
CFLAGS += $(INCLUDEFLAGS)

SRC_FILES = $(wildcard $(MOD_TREE)/src/*.c)
SRC_OBJS = $(SRC_FILES:.c=.o)
SRC_LIBS = $(MOD_TARGET)

include $(SRC_TREE)/scripts/Makefile.rule
