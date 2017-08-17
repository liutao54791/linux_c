# Copyright (C) 2012-2014 Opera Software AS.  All rights reserved.
#
# This file is part of the Opera Devices SDK.
# It includes Opera proprietary information and distribution is prohibited
# without Opera Software's prior, explicit and written consent.

VM_LINUX_ROOT ?= $(word 1, $(subst /apollo/,/apollo /, $(shell pwd)))
TOOL_CHAIN ?= 4.8.2
ENABLE_CA9_NEON ?= true

CUSTOMER ?= hisense_linux
MODEL_NAME ?= mt5891_eu_linux
OBJECT_TYPE ?= rel

include $(VM_LINUX_ROOT)/linux_mts/mak/common_inc.mak
include $(VM_LINUX_ROOT)/linux_mts/mak/path.mak
include $(VM_LINUX_ROOT)/linux_mts/mak/host.mak

ENABLE_HBBTV ?= YES
USE_MTK_NEW_BACKENDS ?= NO

CROSS ?= armv7a-mediatek482_001_neon-linux-gnueabi-
TARGET = $(BROWSER_OUTPUT_DIR)/bin/hisense_browser
SRCS = main.cc \
       browser-impl.cc \
       browser-mode-off.cc \
       browser-mode-store.cc \
       browser-mode-opera-store.cc \
       browser-mode-hibrowser.cc \
       browser-mode-oipf.cc \
	bookmarkmanager.cc \
	browser_mouse.cc \
	historymanager.cc \
       language.cc \
       sysinfo.cc \
       browser-uva-backends.cc \
       browser-storage.cc \
       configuration.cc \
       graphics.cc \
       browser-input.cc \
       browser-opera.cc \
       virtual-keyboard.cc \
       browser_debug.cc \
       app-manager.cc \
       signal_proc.cc \
       url_filter.cc \
	   sqlite_mgr.cc \
	   history_db_manager.cc \
	   bookmark_db_mgr.cc\
	   socket-manager.cc \
	   browser-util.cc \
	   memory-controller.cc
ifeq ($(ENABLE_HBBTV), YES)
    SRCS += browser-mode-hbbtv.cc
endif

HDRS =

UVA_UTILS_SOURCES = ../uva/utils/log.cpp
UVA_UTILS_OBJECTS = $(patsubst %.cpp,%.o,$(UVA_UTILS_SOURCES))

CXXFLAGS ?= $(SYSTEM_CXX_HEADERS) -fno-rtti -fno-threadsafe-statics -fvisibility-inlines-hidden -Wsign-compare -Wno-abi -std=c++0x -std=gnu++11 -Wno-narrowing -Wno-literal-suffix
LDFLAGS ?= -Wl,--build-id -Wl,-z,now -Wl,-z,relro -pthread -Wl,-z,noexecstack -fPIC -Wl,-O1 -Wl,--as-needed -Wl,--gc-sections -Wl,-rpath=/lib/ -Wl,-rpath-link=lib/  -lpthread
DEFINES ?= -DVIDEO_HOLE=1 -DV8_DEPRECATION_WARNINGS -D_FILE_OFFSET_BITS=64 -DDISABLE_NACL -DOPERA_TVSDK -DENABLE_UVA -DLINUX_EMBEDDED -DCHROMIUM_BUILD '-DLINUX_SANDBOX_BINARY=chrome-sandbox' '-DLINUX_SANDBOX_DEV_ENVVAR=CHROME_DEVEL_SANDBOX' -DENABLE_LOCALSTORAGE_LIMIT -DUI_COMPOSITOR_IMAGE_TRANSPORT -DUSE_AURA=1 -DUSE_DEFAULT_RENDER_THEME=1 -DUSE_LIBJPEG_TURBO=1 -DENABLE_ONE_CLICK_SIGNIN -DENABLE_PRE_SYNC_BACKUP -DENABLE_WEBRTC=1 -DUSE_PROPRIETARY_CODECS -DENABLE_CONFIGURATION_POLICY -DENABLE_NOTIFICATIONS -DDONT_EMBED_BUILD_METADATA -DENABLE_JSPLUGINS=1 -DENABLE_GOOGLE_NOW=1 -DCLD_VERSION=2 -DENABLE_PRINTING=1 -DENABLE_BASIC_PRINTING=1 -DENABLE_PRINT_PREVIEW=1 -DENABLE_SPELLCHECK=1 -DENABLE_SUPERVISED_USERS=1 -DENABLE_MDNS=1 -DENABLE_SERVICE_DISCOVERY=1 -DV8_USE_EXTERNAL_STARTUP_DATA -DMOJO_USE_SYSTEM_IMPL -DLIBPEERCONNECTION_LIB=1 -DUSE_LIBPCI=1 -DUSE_NSS=1 -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -DNVALGRIND -DDYNAMIC_ANNOTATIONS_ENABLED=0

ifeq ($(ENABLE_HBBTV), YES)
    DEFINES += -DENABLE_HBBTV
endif

ifeq ($(USE_MTK_NEW_BACKENDS), YES)
    DEFINES += -DUSE_MTK_NEW_BACKENDS
endif

DIAGS ?= YES
ifeq ($(DIAGS), YES)
    DEFINES += -DDIAGS -DLOGTAG=\"browser\"
endif

ifeq ("$(MW_MODEL)", "mt5891_eu_linux")
CXXFLAGS += -DAPP_MARKET=\"eu\"
else ifeq ("$(MW_MODEL)", "mt5891_sa_linux")
CXXFLAGS += -DAPP_MARKET=\"sa\"
else ifeq ("$(MW_MODEL)", "mt5891_em_linux")
CXXFLAGS += -DAPP_MARKET=\"em\"
else
CXXFLAGS += -DAPP_MARKET=\"us\"
endif

OPENGLESBASEDIR ?=

USE_DIRECTFB = YES
DIRECTFB_X11 = NO

VERBOSE ?= NO
DEBUG ?= NO
OPERA_LIB_DIR ?= $(OPERA_ENV_DIR)/common/lib
OPERA_INCLUDE_DIRS ?= $(OPERA_INCLUDE_DIR)/

ifeq ($(DEBUG), YES)
	STRIP ?= NO
else
	STRIP ?= YES
endif

ifneq ($(DEBUG), YES)
    DEFINES += -DNDEBUG -D_FORTIFY_SOURCE=2
endif

CXX = $(CROSS)g++
BUILD_DIR = $(PWD)

CXXFlags = $(DEFINES) $(filter-out "", $(CXXFLAGS))

ifeq ($(VERBOSE),YES)
  HUSH =
  HUSH_ECHO = @echo
else
  HUSH = @
  HUSH_ECHO = @true
endif

ifeq ($(DEBUG),YES)
  CXXFlags += -O0 -g
else
ifeq ($(strip $(OPT_LEVEL)),)
  CXXFlags += -O2
else
  CXXFlags += -O$(OPT_LEVEL)
endif
endif

ifeq ($(USE_DATABASE_FOR_BROWSER), YES)
	CXXFlags += -DDATABASE_IMPLEMENT
endif

ifeq ($(USE_PLATFORM_LOGLEVEL), YES)
	CXXFlags += -DPLATFORM_DEBUG_LEVEL
	CXXFlags += -I$(PLATFORM_LOGLEVEL_INC)
	LDFlags  += -L$(PLATFROM_LOGLEVEL_LIB_PATH) \
				-llogLevel
endif

#if available, add OpenGL to the include path and lib path
ifneq ($(OPENGLESBASEDIR),)
  INCLUDES += -I$(OPENGLESBASEDIR)/include
  LDFlags  += -L$(OPENGLESBASEDIR)/lib
endif

ifneq ("$(MW_MODEL)", "mt5891_sa_linux")
LDFlags += -L$(VM_LINUX_ROOT)/third_party/source/ms-screen/output/
LDFlags += -L$(VM_LINUX_ROOT)/third_party/source/ms-screen/third_party/lib/
LDFlags += -ldlna_stack_adapter
LDFlags += -lutils
LDFlags += -lrpclite
LDFlags += -llinux_dlna_mtk
endif

#Sqlite include
CXXFlags += -I$(PROJECT_HOME)/oss/library/gnuarm-4.8.2_neon_ca9/sqlite/3.8.4.3/include/

ifeq ($(USE_RECEIVE_KEY_DFB_CORE),YES)
	CXXFlags += -DRECEIVE_KEY_DFB_CORE
endif

ifeq ($(TVSYSINFO), YES)
	CXXFlags += -DTVSYSINFO
endif

CXXFlags += -Wall -Wno-write-strings
INCLUDES += $(addprefix -I, $(abspath $(OPERA_INCLUDE_DIRS)))
LDFlags += -Wl,-Bstatic -ljsoncpp -Wl,-Bdynamic -rdynamic
LDFlags += -Wl,-rpath-link=$(abspath $(OPERA_LIB_DIR)) -L$(abspath $(OPERA_LIB_DIR)) -lomi_components -lomi -ldl
ifeq ($(ENABLE_HBBTV), YES)
    LDFlags += -lhbbtvomi_components
endif
LDFlags += -L$(OPERA_4X_OUTPUT_DIR)/lib
LDFlags += $(LDFLAGS)

LDFlags += -L$(PROJECT_HOME)/output/hisense_linux/$(MW_MODEL)/rel/obj/rootfs/rootfs_$(ROOTFS_NAME)/lib
LDFlags += -L$(PROJECT_HOME)/output/hisense_linux/$(MW_MODEL)/rel/obj/rootfs/rootfs_$(ROOTFS_NAME)/basic/lib
LDFlags += -L$(PROJECT_HOME)/output/hisense_linux/$(MW_MODEL)/rel/obj/rootfs/rootfs_$(ROOTFS_NAME)/basic
LDFlags += -L$(PROJECT_HOME)/sys_build/hisense_linux/$(MW_MODEL)/output/rel/basic/lib
LDFlags += -L$(MTK_UTIL_LIB_ROOT)/mtk_stream
LDFlags += -L$(MTK_UTIL_LIB_ROOT)/securestorage/lib
LDFlags += -L$(VM_LINUX_ROOT)/sys_build/hisense_linux/$(MW_MODEL)
LDFlags += -L$(BROWSER_OUTPUT_DIR)/lib

ifeq ($(USE_MTK_NEW_BACKENDS), YES)
  LDFlags += -Wl,-rpath-link=$(VM_LINUX_ROOT)/third_party/library/gnuarm-$(TOOL_CHAIN)$(VFP_SUFFIX)/playready/$(PLAYREADY_VERSION)/common/lib
  LDFlags += -Wl,-rpath-link=$(BROWSER_DIR_TOP)/browser/uva/backends/drm
  LDFlags += -Wl,-rpath-link=$(OSS_LIB_ROOT)/libmxml/$(LIBMXML_VERSION)/lib
  LDFlags += -L$(BROWSER_COM_OUTPUT_DIR)/opera_dir/uva_backends -lbackend -luvamse
else
  LDFlags += -L$(BROWSER_COM_OUTPUT_DIR)/opera_dir/uva_backends -lpistrm
endif

LDFlags += -lfreetype -lexpat -lpng12 -ljpeg -lz -lfontconfig
LDFlags += -lmtk_stream \
	   -ldtv_system \
	   -lapp_if_rpc \
	   -lrpc_ipc \
	   -lcmpb \
	   -lsecurestorage \
	   -ldtv_osai \
	   -lmtal \
	   -lhandle_app\
		   -lapp_client

LDFlags += -Wl,-rpath-link=$(VM_LINUX_ROOT)/sys_build/hisense_linux/$(MW_MODEL)/output/rel/basic
LDFlags += -Wl,-rpath-link=$(VM_LINUX_ROOT)/sys_build/hisense_linux/$(MW_MODEL)/output/rel/basic/lib

ifeq ($(USE_DIRECTFB), YES)
  CXXFlags += -DUSE_DIRECTFB
  INCLUDES += -I$(DFB_PATH)/include
  INCLUDES += -I$(DFB_PATH)/lib
  INCLUDES += -I$(DFB_PATH)/../FusionDale/include
  INCLUDES += -I$(DFB_KEYMAP_INC)
  LDFlags += -ldirectfb
endif

INCLUDES += -I$(BROWSER_DIR_TOP)
INCLUDES += -I$(BROWSER_DIR_TOP)/browser
INCLUDES += -I$(BROWSER_DIR_TOP)/browser/uva/backends/mtk/backend
INCLUDES += -I$(BROWSER_DIR_TOP)/browser/uva/backends/mtk/common

INCLUDES += -I$(VM_LINUX_ROOT)/custom/dev
ifeq ($(TVSYSINFO), YES)
INCLUDES += -I$(VM_LINUX_ROOT)/third_party/source/tvsysinfo/include
LDFlags += -L$(VM_LINUX_ROOT)/third_party/source/tvsysinfo/build/lib -ltvsysinfo
endif

# mtk headers
INCLUDES += -I$(VM_LINUX_ROOT)/output/hisense_linux/$(MW_MODEL)/rel/inc/c_inc
INCLUDES += -I$(VM_LINUX_ROOT)/output/hisense_linux/$(MW_MODEL)/rel/inc/x_inc

#TODO delete
#INCLUDES += -I$(VM_LINUX_ROOT)/mtk_obj/hisense_linux/mt5891_eu_linux/rel/inc/c_inc/
#INCLUDES += -I$(VM_LINUX_ROOT)/mtk_obj/hisense_linux/mt5891_eu_linux/rel/inc/x_inc/

CXXFlags += $(INCLUDES)

ifeq ($(DIRECTFB_X11), YES)
  CXXFlags += -DDIRECTFB_X11
endif

OBJS   = $(SRCS:.cc=.o)

.phony: clean all

all: $(TARGET)

clean:
	$(HUSH_ECHO) Cleaning $(TARGET)
	$(HUSH) rm -f $(TARGET) $(OBJS) $(UVA_UTILS_OBJECTS)

%.o: %.cc
	$(HUSH_ECHO) Compiling $< ...
	$(HUSH) $(CXX) $(CXXFlags) -c $< -o $(abspath $@)

%.o: %.cpp
	$(HUSH_ECHO) Compiling $< ...
	$(HUSH) $(CXX) $(CXXFlags) -c $< -o $(abspath $@)

$(TARGET): $(OBJS) $(UVA_UTILS_OBJECTS)
	$(HUSH_ECHO) Linking $(TARGET) ...
	mkdir -p $(BROWSER_OUTPUT_DIR)/bin
	$(HUSH) $(CXX) $(realpath $(OBJS)) $(realpath $(UVA_UTILS_OBJECTS)) $(LDFlags) -o $(abspath $(TARGET))

SOURCE_FILES = $(SRCS) $(UVA_UTILS_SOURCES)

lint: $(SOURCE_FILES)
	PCLINT_PATH=$(VM_LINUX_ROOT)/tools/lint $(VM_LINUX_ROOT)/tools/lint/gcclint-arm-neon $(INCLUDES) $^
