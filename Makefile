#
# Copyright (c) 2016 Compnay.
# All rights, including trade secret rights, reserved.
#

OUTPUT_FILENAME  := c++_app

SRC_CODES_PATH   := src
INCLUDES_PATH	 := include

MAKEFILE_NAME    := $(MAKEFILE_LIST)
MAKEFILE_DIR     := $(dir $(MAKEFILE_NAME))

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO := 
else
NO_ECHO := @
endif

# Toolchain commands
CC              = g++
AS              = as
AR              = ar -r
LD              = ld
NM              = nm
OBJDUMP         = objdump
OBJCOPY         = objcopy
SIZE            = size

LIBS     		= -lpthread
CFLAGS          = -lstdc++ -Wall  -g

MK             := mkdir
RM             := rm -rf

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#source application codes to all targets
C_SOURCE_FILES += $(SRC_CODES_PATH)/main.cc
C_SOURCE_FILES += $(SRC_CODES_PATH)/person.cc
C_SOURCE_FILES += $(SRC_CODES_PATH)/son.cc
C_SOURCE_FILES += $(SRC_CODES_PATH)/grandson.cc

INC_PATHS += -I"$(INCLUDES_PATH)"

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )


C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.cc=.o) )


vpath %.cc $(C_PATHS)

OBJECTS = $(C_OBJECTS) 

#default target - first one defined
default: clean app

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj --no-print-directory
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e app --no-print-directory

#target for printing all targets
help:
	@echo following targets are available:
	@echo make
	@echo make clean

app: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME)
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -lm -o $(MAKEFILE_DIR)/$(OUTPUT_FILENAME)
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize --no-print-directory

## Create build directories
$(BUILD_DIRECTORIES):
	@echo $(MAKEFILE_NAME) Starting.....
	$(NO_ECHO)$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.cc
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<

finalize: echosize

echosize:
	-@echo '*****************************************************************'
	$(NO_ECHO)$(SIZE) -t $(MAKEFILE_DIR)/$(OUTPUT_FILENAME)
	-@echo '*****************************************************************'

clean:
	$(NO_ECHO)$(RM) $(BUILD_DIRECTORIES)
	$(NO_ECHO)$(RM) $(MAKEFILE_DIR)/$(OUTPUT_FILENAME)
	@echo Deleting folder: _build

cleanobj:
	$(NO_ECHO)$(RM) $(BUILD_DIRECTORIES)/*.o

run:
	$(NO_ECHO)./c++_app
.PHONY: run

