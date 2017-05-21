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
INCLUDES += -I$(SOROOT)
INCLUDES += -I$(SOROOT)/../libapp
SOS = -shared -fpic 

all: libprocess_pthread.so
libprocess_pthread.so :
	$(XX) $(CFLAGS) $(SRCS) -o libprocess_pthread.so $(INCLUDES) $(SOS)

clean:
	rm -rf *.so 
