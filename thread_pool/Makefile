#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#
CC = gcc
XX = g++
CFLAGS = -Wall -O -g

SOROOT = $(shell pwd)
SRCS += $(wildcard $(SOROOT)/*.cc) 

OBJS        := $(SRCS:.cc=.o)

INCLUDES += -I$(SOROOT)
SOS = -shared -fpic 

CFLAGS += $(INCLUDES)
CFLAGS += $(SOS)

all: libthreadpool.so
libthreadpool.so : 
	$(XX) $(CFLAGS) -c $(SRCS) 
	$(XX) $(CFLAGS) -o libthreadpool.so $(OBJS)

clean:
	rm -rf *.so
	rm -rf *.o
