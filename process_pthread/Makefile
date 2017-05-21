#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#
CC = gcc
XX = g++
CFLAGS += -Wall -O -g

#MYSQL_LIB=/usr/local/shipu/mdpserver/msgsql_20161214/mysql_lib
#MYSQL_INC=/usr/local/shipu/mdpserver/msgsql_20161214/mysql_include
SOROOT = $(shell pwd)
SRCS += $(wildcard $(SOROOT)/*.cc) 

## all .o based on all .cc  
OBJS        := $(SRCS:.cc=.o)

INCLUDES += -I$(SOROOT)
INCLUDES += -I$(SOROOT)/../libapp
SOS = -shared -fpic 

CFLAGS += $(INCLUDES)
CFLAGS += $(SOS)

all: libprocess_pthread.so
libprocess_pthread.so :
	$(XX) $(CFLAGS) -c $(SRCS) 
	$(XX) $(CFLAGS) -o libprocess_pthread.so $(OBJS)

clean:
	rm -rf *.so 
	rm -rf *.o
