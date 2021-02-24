################################################################################
# \file Makefile
# \version 1.0
#
# \brief
# Top-level application make file.
#
################################################################################
# \copyright
# Copyright 2018-2020 Cypress Semiconductor Corporation
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

################################################################################
# MCUboot Specific Configuration
###############################################################################

# Supported values: BOOT, UPGRADE
# The Blinky app defines the LED period differently depending on whether the
# image is BOOT or UPGRADE. 
# BOOT: Use when image is built for the primary slot. 
#       --pad argument is not passed to the imgtool. 
# UPGRADE: Use when image is built for secondary slot. 
#          --pad argument is passed to the imgtool. 
#UPGRADE BOOT 
IMG_TYPE ?= UPGRADE

# Starting address of the CM4 app or the offset at which the header of an image
# will begin. Image = Header + App + TLV + Trailer. See MCUboot documenation for
# details. 
# New relocated address = ORIGIN + HEADER_OFFSET
# ORIGIN is defined in the CM4 linker script and is usually the address next to
# the end of CM0+ image. 
ifeq ($(IMG_TYPE), BOOT)
    HEADER_OFFSET_INT ?= 0
    HEADER_OFFSET_EXT ?= 0
else
    HEADER_OFFSET_INT ?= 0x80e8000
    HEADER_OFFSET_EXT ?= 0x300000
endif

# Path of the key file passed as a parameter to the Python module "imgtool" for
# signing the image. Name of the key file is set in bootloader_cm0p/shared_config.mk
# as a common parameter to both the Bootloader app and Blinky app.
KEY_FILE_PATH=../../bootloader_cm0p/keys

# Set the version of the app using the following three variables.
# This version information is passed to the Python module "imgtool" while
# signing the image in the post build step. Default values are set as follows.
ifeq ($(IMG_TYPE), BOOT)
APP_VERSION_MAJOR ?= 1
APP_VERSION_MINOR ?= 0
APP_VERSION_BUILD ?= 0
else
ifeq ($(IMG_TYPE), UPGRADE)
APP_VERSION_MAJOR ?= 2
APP_VERSION_MINOR ?= 0
APP_VERSION_BUILD ?= 0
endif
endif

# Number of images supported in case of multi-image bootloading. 
# This application supports only up to 2 images.
MCUBOOT_IMAGE_NUMBER ?= 1


################################################################################
# Basic Configuration
################################################################################

# Target board/hardware (BSP).
# To change the target, it is recommended to use the Library manager 
# ('make modlibs' from command line), which will also update Eclipse IDE launch 
# configurations. If TARGET is manually edited, ensure TARGET_<BSP>.mtb with a 
# valid URL exists in the application, run 'make getlibs' to fetch BSP contents
# and update or regenerate launch configurations for your IDE.
TARGET=CY8CKIT-062S2-43012

# Underscore needed for $(TARGET) directory
TARGET_UNDERSCORE=$(subst -,_,$(TARGET))

# Core processor
CORE?=CM4

# Name of application (used to derive name of final linked file).
# 
# If APPNAME is edited, ensure to update or regenerate launch 
# configurations for your IDE.
APPNAME=mtb-example-psoc6-dual-cpu-empty-app

# Name of toolchain to use. Options include:
#
# GCC_ARM -- GCC provided with ModusToolbox IDE
# ARM     -- ARM Compiler (must be installed separately)
# IAR     -- IAR Compiler (must be installed separately)
#
# See also: CY_COMPILER_PATH below
TOOLCHAIN=GCC_ARM

# Default build configuration. Options include:
#
# Debug -- build with minimal optimizations, focus on debugging.
# Release -- build with full optimizations
# Custom -- build with custom configuration, set the optimization flag in CFLAGS
# 
# If CONFIG is manually edited, ensure to update or regenerate launch configurations 
# for your IDE.
CONFIG=Debug

# If set to "true" or "1", display full command-lines when building.
VERBOSE=


################################################################################
# Advanced Configuration
################################################################################

# Enable optional code that is ordinarily disabled by default.
#
# Available components depend on the specific targeted hardware and firmware
# in use. In general, if you have
#
#    COMPONENTS=foo bar
#
# ... then code in directories named COMPONENT_foo and COMPONENT_bar will be
# added to the build
#
COMPONENTS=FREERTOS PSOC6HAL LWIP MBEDTLS WICED_BLE WCM SECURE_SOCKETS
COMPONENTS+=CUSTOM_DESIGN_MODUS

# Like COMPONENTS, but disable optional code that was enabled by default.
DISABLE_COMPONENTS=CM0P_SLEEP BSP_DESIGN_MODUS
#DISABLE_COMPONENTS= BSP_DESIGN_MODUS

# Add dependency to the custom CM0+ App
#DEPENDENT_APP_PATHS=../cm0p_app

CY_EXTAPP_PATH=../shareFile/COMPONENT_CUSTOM_DESIGN_MODUS/TARGET_CY8CKIT-062S2-43012

# By default the build system automatically looks in the Makefile's directory
# tree for source code and builds it. The SOURCES variable can be used to
# manually add source code to the build process from a location not searched
# by default, or otherwise not found by the build system.
SOURCES=

# Like SOURCES, but for include directories. Value should be paths to
# directories (without a leading -I).
INCLUDES=../shareFile/include

# Custom configuration of mbedtls library.
MBEDTLSFLAGS = MBEDTLS_USER_CONFIG_FILE='"configs/mbedtls_user_config.h"'

# Add additional defines to the build process (without a leading -D).
DEFINES=$(MBEDTLSFLAGS) CYBSP_WIFI_CAPABLE CY_RETARGET_IO_CONVERT_LF_TO_CRLF
DEFINES+=APP_VERSION_MAJOR=$(APP_VERSION_MAJOR) \
         APP_VERSION_MINOR=$(APP_VERSION_MINOR) \
         APP_VERSION_BUILD=$(APP_VERSION_BUILD)
DEFINES+=CY_BOOT_USE_EXTERNAL_FLASH=1
DEFINES+=CY_SD_HOST_CLK_RAMP_UP_TIME_MS_WAKEUP=0 CY_RTOS_AWARE

DEFINES+=KADAAS_COULD=1 
#DEFINES+=KAADAS_BOARD=1

DEFINES+=ECC256_KEY_FILE='"$(SIGN_KEY_FILE).pub"' \
         MCUBOOT_IMAGE_NUMBER=$(MCUBOOT_IMAGE_NUMBER) \
         MCUBOOT_MAX_IMG_SECTORS=$(MCUBOOT_MAX_IMG_SECTORS)
 
#DEFINES+=CY_ENABLE_BLE_DEL
 # CY8CPROTO-062-4343W board shares the same GPIO for the user button (SW2)
# and the CYW4343W host wake up pin. Since this example uses the GPIO for
# interfacing with the user button, the SDIO interrupt to wake up the host is
# disabled by setting CY_WIFI_HOST_WAKE_SW_FORCE to '0'.
ifeq ($(TARGET), CY8CPROTO-062-4343W)
DEFINES+=CY_WIFI_HOST_WAKE_SW_FORCE=0
endif

 # Enable external memeory in XIP mode
# The bootloader have been enabled CY_ENABLE_XIP_PROGRAM, so disable in the application.
ifeq ($(MCUBOOT_IMAGE_NUMBER), 2)
 ifeq ($(TARGET), CY8CPROTO-062S3-4343W)
 #DEFINES+=CY_ENABLE_XIP_PROGRAM 
 DEFINES+=CY_STORAGE_WIFI_DATA=\".cy_xip\"
 else ifeq ($(TARGET), CY8CPROTO-062-4343W)
 DEFINES+=CY_STORAGE_WIFI_DATA=\".cy_xip\"
 else ifeq ($(TARGET), CY8CKIT-062S2-43012)
 DEFINES+=CY_STORAGE_WIFI_DATA=\".cy_xip\"
 endif
endif

# Custom flash map description, please check the cy_flash_map.c file in the top-level code example directory.
DEFINES+=CY_FLASH_MAP_EXT_DESC

# Enable the debug logs output
#DEFINES+=ENABLE_WIFI_MIDDLEWARE_LOGS
#DEFINES+=ENABLE_SECURE_SOCKETS_LOGS 
#DEFINES+=ENABLE_WCM_LOGS
#DEFINES+=LWIP_DEBUG TCP_INPUT_DEBUG=LWIP_DBG_ON TCP_DEBUG=LWIP_DBG_ON PBUF_DEBUG=LWIP_DBG_ON
#DEFINES+=LWIP_DEBUG TCP_CWND_DEBUG=LWIP_DBG_ON
        
# Select softfp or hardfp floating point. Default is softfp.
VFP_SELECT=hardfp

# Additional / custom C compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
CFLAGS=

# Additional / custom C++ compiler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
CXXFLAGS=

# Additional / custom assembler flags.
#
# NOTE: Includes and defines should use the INCLUDES and DEFINES variable
# above.
ASFLAGS=

# Additional / custom linker flags.
ifeq ($(TOOLCHAIN),GCC_ARM)
LDFLAGS=-Wl,--undefined=uxTopUsedPriority
else
ifeq ($(TOOLCHAIN),IAR)
LDFLAGS=--keep uxTopUsedPriority
else
ifeq ($(TOOLCHAIN),ARM)
LDFLAGS=--undefined=uxTopUsedPriority
else
LDFLAGS=
endif
endif
endif

# Additional / custom linker flags.
LDFLAGS=

# Additional / custom libraries to link in to the application.
LDLIBS=

# Path to the linker script to use (if empty, use the default linker script).
#LINKER_SCRIPT=

# Custom pre-build commands to run.
PREBUILD=

# Custom post-build commands to run.
# POSTBUILD=

# Include the flash map configuration make file
include ../../bootloader_cm0p/shared_config.mk

# The following defines describe the flash map used by MCUBoot
DEFINES+=CY_BOOT_BOOTLOADER_SIZE=$(BOOTLOADER_APP_FLASH_SIZE) \
         CY_BOOT_APP_CM0P_SIZE=$(APP_CM0P_FLASH_SIZE) \
         CY_BOOT_PRIMARY_1_START=0x10000000+$(CY_BOOT_BOOTLOADER_SIZE) \
         CY_BOOT_PRIMARY_1_SIZE=$(MCUBOOT_PRIMARY_1_SLOT_SIZE) \
         CY_BOOT_SECONDARY_1_SIZE=$(MCUBOOT_SECONDARY_1_SLOT_SIZE) \
         CY_BOOT_SCRATCH_SIZE=$(MCUBOOT_SCRATCH_SIZE)

# Define the slot sizes for the image #2 when you need to work with dual images.    
ifeq ($(MCUBOOT_IMAGE_NUMBER), 2)   
DEFINES+=CY_BOOT_PRIMARY_2_SIZE=$(MCUBOOT_PRIMARY_2_SLOT_SIZE) \
         CY_BOOT_SECONDARY_2_SIZE=$(MCUBOOT_SECONDARY_2_SLOT_SIZE)
endif

# Path to the linker script to use and other linker options.
ifeq ($(TOOLCHAIN), GCC_ARM)
LINKER_SCRIPT=$(wildcard ./linker_script/TARGET_$(TARGET)/TOOLCHAIN_$(TOOLCHAIN)/*.ld)
LDFLAGS+=-Wl,--defsym=MCUBOOT_HEADER_SIZE=$(MCUBOOT_HEADER_SIZE)
LDFLAGS+=-Wl,--defsym=BOOTLOADER_CM0P_FLASH_SIZE=$(BOOTLOADER_APP_FLASH_SIZE),--defsym=BOOTLOADER_CM0P_RAM_SIZE=$(BOOTLOADER_APP_RAM_SIZE)
LDFLAGS+=-Wl,--defsym=APP_CM0P_FLASH_SIZE=$(APP_CM0P_FLASH_SIZE),--defsym=APP_CM0P_RAM_SIZE=$(APP_CM0P_RAM_SIZE)
LDFLAGS+=-Wl,--defsym=APP_CM4_FLASH_SIZE=$(APP_CM4_FLASH_SIZE)

CY_ELF_TO_HEX_TOOL=$(CY_CROSSPATH)/bin/arm-none-eabi-objcopy
CY_ELF_TO_HEX_OPTIONS=-O ihex
else
$(error Only GCC_ARM is supported at this moment)
endif

# Define the erase value of the primary 1 image, when the UPGRADE mode, the erase vaule is 0xFF 
ifeq ($(IMG_TYPE), BOOT)
    ERASED_VALUE=0
else
    ERASED_VALUE=0xff
endif

# Set build directory for BOOT and UPGRADE images
CY_BUILD_LOCATION=./build/$(IMG_TYPE)
BINARY_OUT_PATH=$(CY_BUILD_LOC)/$(TARGET)/$(CONFIG)/$(APPNAME)
IMGTOOL_PATH=../../mtb_shared/mcuboot/v1.7.0-cypress/scripts/imgtool.py
CY_BUILD_VERSION=$(APP_VERSION_MAJOR).$(APP_VERSION_MINOR).$(APP_VERSION_BUILD)
# The internal flash image (primary 1) sign arguments
SIGN_ARGS_INT=sign --header-size $(MCUBOOT_HEADER_SIZE) --pad-header --align 8 -v $(CY_BUILD_VERSION) \
               -S $(MCUBOOT_PRIMARY_1_SLOT_SIZE) -M $(MCUBOOT_MAX_IMG_SECTORS) --overwrite-only -R $(ERASED_VALUE) -k $(KEY_FILE_PATH)/$(SIGN_KEY_FILE).pem
ifeq ($(MCUBOOT_IMAGE_NUMBER), 2)
# The external flash image (primary 2) sign arguments
SIGN_ARGS_EXT=sign --header-size $(MCUBOOT_HEADER_SIZE) --pad-header --align 8 -v $(CY_BUILD_VERSION) \
               -S $(MCUBOOT_PRIMARY_2_SLOT_SIZE) -M $(MCUBOOT_EXT_MAX_IMG_SECTORS) --overwrite-only -R 0xff -k $(KEY_FILE_PATH)/$(SIGN_KEY_FILE).pem
endif

# UPGRADE Mode: Use when image is built for secondary slot. --pad argument is passed to the imgtool.
ifeq ($(IMG_TYPE), BOOT)
    DEFINES+=BOOT_IMG
else 
ifeq ($(IMG_TYPE), UPGRADE)
    DEFINES+=UPGRADE_IMG
    SIGN_ARGS_INT += --pad
    ifeq ($(MCUBOOT_IMAGE_NUMBER), 2)
    SIGN_ARGS_EXT += --pad
    endif
else
    $(error Set IMG_TYPE to be either BOOT or UPGRADE)
endif
endif

# Python path definition
ifeq ($(OS),Windows_NT)
PYTHON_PATH?=C:/Users/yish/ModusToolbox/tools_2.2/python-3.7.137/python
else
PYTHON_PATH?=python3
endif

# The srecord tool path definition, this tool use to merge the hex files
ifeq ($(OS),Windows_NT)
SREC_CAT_PATH?=../tools/srecord/srec_cat.exe
else
SREC_CAT_PATH?=srec_cat
endif

# The app cm0p path
APP_CM0P_BINARY_OUT_PATH=$(CY_INTERNAL_APPLOC)../../cm0p_app/build/$(TARGET)/$(CONFIG)/cy_m0p_image

# Custom post-build commands to run.
# 1. Take a backup of the original hex file (_raw.hex)
# 2. Delete the original hex
# 3. Merge the app_cm0p and app_cm4 elf files.
# 4. Relocate the starting address based on HEADER_OFFSET_INT and HEADER_OFFSET_EXT,
#    also convert .elf to _unsigned_int.hex (on the internal flash) and _unsigned_ext (XIP code).hex
# 5. Sign the image using imgtool (_int.hex and _ext.hex)
# 6. Merge the _int.hex and _ext.hex to .hex by srec_cat tool
#
# Note that the final file should be $(APPNAME).hex so that Eclipse and
# Make CLI can locate the file for programming.
ifeq ($(MCUBOOT_IMAGE_NUMBER), 2)
    ifeq ($(IMG_TYPE), BOOT)
    POSTBUILD=\
    cp -f $(BINARY_OUT_PATH).hex $(BINARY_OUT_PATH)_raw.hex;\
    rm -f $(BINARY_OUT_PATH).hex;\
    "$(CY_MCUELFTOOL_DIR)/bin/cymcuelftool.exe" -M  $(CY_CONFIG_DIR)/$(APPNAME).elf $(APP_CM0P_BINARY_OUT_PATH).elf --output $(CY_CONFIG_DIR)/$(APPNAME).elf;\
    $(CY_ELF_TO_HEX_TOOL) --change-addresses=$(HEADER_OFFSET_INT) --remove-section=.cy_xip $(CY_ELF_TO_HEX_OPTIONS) $(BINARY_OUT_PATH).elf $(BINARY_OUT_PATH)_unsigned_int.hex;\
    $(CY_ELF_TO_HEX_TOOL) --change-addresses=$(HEADER_OFFSET_EXT) --only-section=.cy_xip $(CY_ELF_TO_HEX_OPTIONS) $(BINARY_OUT_PATH).elf $(BINARY_OUT_PATH)_unsigned_ext.hex;\
    $(PYTHON_PATH) $(IMGTOOL_PATH) $(SIGN_ARGS_INT) $(BINARY_OUT_PATH)_unsigned_int.hex $(BINARY_OUT_PATH)_int.hex;\
    $(PYTHON_PATH) $(IMGTOOL_PATH) $(SIGN_ARGS_EXT) $(BINARY_OUT_PATH)_unsigned_ext.hex $(BINARY_OUT_PATH)_ext.hex;\
    $(SREC_CAT_PATH) $(BINARY_OUT_PATH)_int.hex -Intel $(BINARY_OUT_PATH)_ext.hex -Intel -o $(BINARY_OUT_PATH).hex -Intel;
    else 
    POSTBUILD=\
    cp -f $(BINARY_OUT_PATH).hex $(BINARY_OUT_PATH)_raw.hex;\
    rm -f $(BINARY_OUT_PATH).hex;\
    "$(CY_MCUELFTOOL_DIR)/bin/cymcuelftool.exe" -M  $(CY_CONFIG_DIR)/$(APPNAME).elf $(APP_CM0P_BINARY_OUT_PATH).elf --output $(CY_CONFIG_DIR)/$(APPNAME).elf;\
    $(CY_ELF_TO_HEX_TOOL) --change-addresses=$(HEADER_OFFSET_INT) --remove-section=.cy_xip $(CY_ELF_TO_HEX_OPTIONS) $(BINARY_OUT_PATH).elf $(BINARY_OUT_PATH)_unsigned_int.hex;\
    $(CY_ELF_TO_HEX_TOOL) --change-addresses=$(HEADER_OFFSET_EXT) --only-section=.cy_xip $(CY_ELF_TO_HEX_OPTIONS) $(BINARY_OUT_PATH).elf $(BINARY_OUT_PATH)_unsigned_ext.hex;\
    $(PYTHON_PATH) $(IMGTOOL_PATH) $(SIGN_ARGS_INT) $(BINARY_OUT_PATH)_unsigned_int.hex $(BINARY_OUT_PATH)_int.hex;\
    $(PYTHON_PATH) $(IMGTOOL_PATH) $(SIGN_ARGS_EXT) $(BINARY_OUT_PATH)_unsigned_ext.hex $(BINARY_OUT_PATH)_ext.hex;\
    $(CY_ELF_TO_HEX_TOOL) --input-target=ihex --output-target=binary $(BINARY_OUT_PATH)_int.hex $(CY_BUILD_LOC)/$(TARGET)/$(CONFIG)/app-fw-0.bin;\
    $(CY_ELF_TO_HEX_TOOL) --input-target=ihex --output-target=binary $(BINARY_OUT_PATH)_ext.hex $(CY_BUILD_LOC)/$(TARGET)/$(CONFIG)/app-fw-1.bin;
    endif
else
    ifeq ($(IMG_TYPE), BOOT)
    POSTBUILD=\
    cp -f $(BINARY_OUT_PATH).hex $(BINARY_OUT_PATH)_raw.hex;\
    rm -f $(BINARY_OUT_PATH).hex;\
    "$(CY_MCUELFTOOL_DIR)/bin/cymcuelftool.exe" -M  $(CY_CONFIG_DIR)/$(APPNAME).elf $(APP_CM0P_BINARY_OUT_PATH).elf --output $(CY_CONFIG_DIR)/$(APPNAME).elf;\
    $(CY_ELF_TO_HEX_TOOL) --change-addresses=$(HEADER_OFFSET_INT) --remove-section=.cy_xip $(CY_ELF_TO_HEX_OPTIONS) $(BINARY_OUT_PATH).elf $(BINARY_OUT_PATH)_unsigned_int.hex;\
    $(PYTHON_PATH) $(IMGTOOL_PATH) $(SIGN_ARGS_INT) $(BINARY_OUT_PATH)_unsigned_int.hex $(BINARY_OUT_PATH).hex;
    else 
    POSTBUILD=\
    cp -f $(BINARY_OUT_PATH).hex $(BINARY_OUT_PATH)_raw.hex;\
    rm -f $(BINARY_OUT_PATH).hex;\
    "$(CY_MCUELFTOOL_DIR)/bin/cymcuelftool.exe" -M  $(CY_CONFIG_DIR)/$(APPNAME).elf $(APP_CM0P_BINARY_OUT_PATH).elf --output $(CY_CONFIG_DIR)/$(APPNAME).elf;\
    $(CY_ELF_TO_HEX_TOOL) --change-addresses=$(HEADER_OFFSET_INT) --remove-section=.cy_xip $(CY_ELF_TO_HEX_OPTIONS) $(BINARY_OUT_PATH).elf $(BINARY_OUT_PATH)_unsigned_int.hex;\
    $(PYTHON_PATH) $(IMGTOOL_PATH) $(SIGN_ARGS_INT) $(BINARY_OUT_PATH)_unsigned_int.hex $(BINARY_OUT_PATH)_int.hex;\
    $(CY_ELF_TO_HEX_TOOL) --input-target=ihex --output-target=binary $(BINARY_OUT_PATH)_int.hex $(CY_BUILD_LOC)/$(TARGET)/$(CONFIG)/app-fw.bin;
    endif
endif



################################################################################
# Paths
################################################################################

# Relative path to the project directory (default is the Makefile's directory).
#
# This controls where automatic source code discovery looks for code.
CY_APP_PATH=

# Relative path to the shared repo location.
#
# All .mtb files have the format, <URI>#<COMMIT>#<LOCATION>. If the <LOCATION> field 
# begins with $$ASSET_REPO$$, then the repo is deposited in the path specified by 
# the CY_GETLIBS_SHARED_PATH variable. The default location is one directory level 
# above the current app directory.
# This is used with CY_GETLIBS_SHARED_NAME variable, which specifies the directory name.
CY_GETLIBS_SHARED_PATH=../../

# Directory name of the shared repo location.
#
CY_GETLIBS_SHARED_NAME=mtb_shared

# Absolute path to the compiler's "bin" directory.
#
# The default depends on the selected TOOLCHAIN (GCC_ARM uses the ModusToolbox
# IDE provided compiler by default).
CY_COMPILER_PATH=


# Locate ModusToolbox IDE helper tools folders in default installation
# locations for Windows, Linux, and macOS.
CY_WIN_HOME=$(subst \,/,$(USERPROFILE))
CY_TOOLS_PATHS ?= $(wildcard \
    $(CY_WIN_HOME)/ModusToolbox/tools_* \
    $(HOME)/ModusToolbox/tools_* \
    /Applications/ModusToolbox/tools_*)

# If you install ModusToolbox IDE in a custom location, add the path to its
# "tools_X.Y" folder (where X and Y are the version number of the tools
# folder).
CY_TOOLS_PATHS+=

# Default to the newest installed tools folder, or the users override (if it's
# found).
CY_TOOLS_DIR=$(lastword $(sort $(wildcard $(CY_TOOLS_PATHS))))

ifeq ($(CY_TOOLS_DIR),)
$(error Unable to find any of the available CY_TOOLS_PATHS -- $(CY_TOOLS_PATHS))
endif

$(info Tools Directory: $(CY_TOOLS_DIR))

include $(CY_TOOLS_DIR)/make/start.mk

getlibs : getlibs_cm0p
getlibs_cm0p:
	$(MAKE) -C ../cm0p_app/ $(MAKECMDGOALS)
