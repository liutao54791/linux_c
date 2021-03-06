#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#

all: make-dir lib-app lib-uart lib-processpthread lib-ipc thread-pool process-one main-process

make-dir:
	@echo ">>>Start makedir lib."
	#@mkdir bin
	@echo "<<<End makedir lib"
lib-app:
	@echo ">>>Start building libso."
	@cd libapp; make;cp libapp.so  ../lib
	@echo "<<<End building libso."

lib-uart:
	@echo ">>>Start building libuart."
	@cd uart; make;cp libuart.so  ../lib
	@echo "<<<End building libuart."

lib-processpthread:
	@echo ">>>Start building lib-processpthread."
	@cd libprocess_pthread; make;cp libprocess_pthread.so  ../lib
	@echo "<<<End building lib-processpthread."

lib-ipc:
	@echo ">>>Start building lib-app."
	@cd libipc; make;cp libipc.so  ../lib
	@echo "<<<End building lib-app."

thread-pool:
	@echo ">>>Start building thread-pool."
	@cd thread_pool; make;cp libthreadpool.so ../lib
	@echo "<<<End building thread-pool"

process-one:
	@echo ">>>Start building process-one."
	@cd process_one; make;cp process_one ../bin;rm process_one
	@echo "<<<End building process-one."

main-process:
	@echo ">>>Start building main-process."
	@cd mainprocess; make;cp process_main ../bin;rm process_main
	@echo "<<<End building main-process."
copy:
	@echo ">>>Start copying."
	@cd commom; cp *.so ../lib
	@cp -rf lib /opt/mycode
	@cp -rf dbhome /opt/mycode
	@cp -rf bin /opt/mycode
	@cp run.sh /opt/mycode
	@echo "<<<End copying."
#target for printing all targets
help:
	@echo following targets are available:
	@echo make
	@echo make clean

clean: libapp-clean libuart-clean mainprocess-clean processpthread-clean libipc-clean process-one-clean thread-pool-clean
    ifeq (process_main, $(wildcard process_main))
	rm process_main
    endif
    ifeq (lib, $(wildcard lib))
	rm -rf lib/*
    endif
    ifeq (bin, $(wildcard bin))
	rm -rf bin/*
    endif
libapp-clean:
	@echo ">>>Start cleaning libso."
	@cd libapp; make clean
	@echo "<<<End cleaning libso."
libuart-clean:
	@echo ">>>Start cleaning libuart."
	@cd uart; make clean
	@echo "<<<End cleaning libuart."
processpthread-clean:
	@echo ">>>Start cleaning lib-processpthread."
	@cd libprocess_pthread; make clean
	@echo "<<<End cleaning lib-processpthread."
libipc-clean:
	@echo ">>>Start cleaning lib-ipc."
	@cd libipc; make clean
	@echo "<<<End cleaning lib-ipc."

process-one-clean:
	@echo ">>>Start cleaning process-one."
	@cd process_one; make clean
	@echo "<<<End cleaning process-one."

mainprocess-clean:
	@echo ">>>Start cleaning mainapp."
	@cd mainprocess; make clean
	@echo "<<<End cleaning mainapp."

thread-pool-clean:
	@echo ">>>Start cleaning thread_pool."
	@cd thread_pool; make clean
	@echo "<<<End cleaning thread_pool."

run:
	@cd bin/;./process_main
.PHONY: run

