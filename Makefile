#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#

all: lib-dir lib-app lib-uart lib-processpthread lib-ipc process-one main-app 

lib-dir:
	@echo ">>>Start makedir lib."
	@mkdir lib
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
	@cd process_pthread; make;cp libprocess_pthread.so  ../lib
	@echo "<<<End building lib-processpthread."

lib-ipc:
	@echo ">>>Start building lib-app."
	@cd libipc; make;cp libipc.so  ../lib
	@echo "<<<End building lib-app."

process-one:
	@echo ">>>Start building process-one."
	@cd process_one; make
	@echo "<<<End building process-one."

main-app:
	@echo ">>>Start building mainapp."
	@cd mainapp; make;cp c++_app ../;rm c++_app
	@echo "<<<End building mainapp."

#target for printing all targets
help:
	@echo following targets are available:
	@echo make
	@echo make clean

clean: libapp-clean libuart-clean mainapp-clean processpthread-clean libipc-clean process-one-clean
    ifeq (c++_app, $(wildcard c++_app))
	rm c++_app
    endif
    ifeq (lib, $(wildcard lib))
	rm -rf lib
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
	@cd process_pthread; make clean
	@echo "<<<End cleaning lib-processpthread."
libipc-clean:
	@echo ">>>Start cleaning lib-ipc."
	@cd libipc; make clean
	@echo "<<<End cleaning lib-ipc."

process-one-clean:
	@echo ">>>Start cleaning process-one."
	@cd process_one; make clean
	@echo "<<<End cleaning process-one."

mainapp-clean:
	@echo ">>>Start cleaning mainapp."
	@cd mainapp; make clean
	@echo "<<<End cleaning mainapp."

run:
	$(NO_ECHO)./c++_app
.PHONY: run

