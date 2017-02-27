#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#
all: lib-so main-app 

lib-so:
	@echo ">>>Start building libso."
	@cd libso; make
	@echo "<<<End building libso."

main-app:
	@echo ">>>Start building mainapp."
	@cd mainapp; make;cp c++_app ../;rm c++_app
	@echo "<<<End building mainapp."

#target for printing all targets
help:
	@echo following targets are available:
	@echo make
	@echo make clean

clean: libso-clean mainapp-clean
	rm c++_app
	
libso-clean:
	@echo ">>>Start cleaning libso."
	@cd libso; make clean
	@echo "<<<End cleaning libso."
mainapp-clean:
	@echo ">>>Start cleaning mainapp."
	@cd mainapp; make clean
	@echo "<<<End cleaning mainapp."
run:
	$(NO_ECHO)./c++_app
.PHONY: run

