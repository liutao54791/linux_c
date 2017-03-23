#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#
all: lib-so lib-uart main-app

lib-so:
	@echo ">>>Start building libso."
	@cd libso; make
	@echo "<<<End building libso."

lib-uart:
	@echo ">>>Start building libuart."
	@cd libuart; make
	@echo "<<<End building libuart."

main-app:
	@echo ">>>Start building mainapp."
	@cd mainapp; make;cp c++_app ../;rm c++_app
	@echo "<<<End building mainapp."

#target for printing all targets
help:
	@echo following targets are available:
	@echo make
	@echo make clean

clean: libso-clean libuart-clean mainapp-clean
	rm c++_app
	
libso-clean:
	@echo ">>>Start cleaning libso."
	@cd libso; make clean
	@echo "<<<End cleaning libso."
libuart-clean:
	@echo ">>>Start cleaning libuart."
	@cd libuart; make clean
	@echo "<<<End cleaning libuart."

mainapp-clean:
	@echo ">>>Start cleaning mainapp."
	@cd mainapp; make clean
	@echo "<<<End cleaning mainapp."
run:
	$(NO_ECHO)./c++_app
.PHONY: run

