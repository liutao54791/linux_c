#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#
CC = gcc
XX = g++
CFLAGS = -Wall -O -g

#MYSQL_LIB=/usr/local/shipu/mdpserver/msgsql_20161214/mysql_lib
#MYSQL_INC=/usr/local/shipu/mdpserver/msgsql_20161214/mysql_include
SOROOT = $(shell pwd)
SRCS += $(wildcard $(SOROOT)/*.cc) 

OBJS        := $(SRCS:.cc=.o)

INCLUDES += -I$(SOROOT)
SOS = -shared -fpic 

CFLAGS += $(INCLUDES)
CFLAGS += $(SOS)

all: libipc.so
libipc.so : 
	$(XX) $(CFLAGS) -c $(SRCS) 
	$(XX) $(CFLAGS) -o libipc.so $(OBJS)

clean:
	rm -rf *.so
	rm -rf *.o
