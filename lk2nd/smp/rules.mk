# SPDX-License-Identifier: BSD-3-Clause
LOCAL_DIR := $(GET_LOCAL_DIR)
MODULES += lib/libfdt

# Additional initialization for CPU clocks (if needed)
OBJS += $(patsubst %.c,%.o, $(wildcard $(LOCAL_DIR)/clock-$(PLATFORM).c))

CPU_BOOT_OBJ := $(LOCAL_DIR)/cortex-a.o
DEFINES += CPU_BOOT_CORTEX_A=1

OBJS += $(LOCAL_DIR)/cpu-boot.o $(CPU_BOOT_OBJ)
