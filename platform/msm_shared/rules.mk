LOCAL_DIR := $(GET_LOCAL_DIR)

INCLUDES += \
			-I$(LOCAL_DIR)/include -I$(LK_TOP_DIR)/dev/panel/msm -I$(LK_TOP_DIR)/app/aboot

DEFINES += $(TARGET_XRES)
DEFINES += $(TARGET_YRES)

DEFINES += DISPLAY_TYPE_MDSS=1

OBJS += \
	$(LOCAL_DIR)/debug.o \
	$(LOCAL_DIR)/smem.o \
	$(LOCAL_DIR)/smem_ptable.o \
	$(LOCAL_DIR)/partition_parser.o \
	$(LOCAL_DIR)/ab_partition_parser.o \
	$(LOCAL_DIR)/hsusb.o \
	$(LOCAL_DIR)/boot_stats.o \
	$(LOCAL_DIR)/qgic_common.o \
	$(LOCAL_DIR)/crc32.o \
	$(LOCAL_DIR)/rpm-ipc.o \
	$(LOCAL_DIR)/sdhci.o \
	$(LOCAL_DIR)/sdhci_msm.o \
	$(LOCAL_DIR)/mmc_sdhci.o \
	$(LOCAL_DIR)/mmc_wrapper.o \
	$(LOCAL_DIR)/qgic.o \
	$(LOCAL_DIR)/qtimer.o \
	$(LOCAL_DIR)/qtimer_mmap.o \
	$(LOCAL_DIR)/interrupts.o \
	$(LOCAL_DIR)/clock.o \
	$(LOCAL_DIR)/clock_pll.o \
	$(LOCAL_DIR)/clock_lib2.o \
	$(LOCAL_DIR)/uart_dm.o \
	$(LOCAL_DIR)/board.o \
	$(LOCAL_DIR)/scm.o \
	$(LOCAL_DIR)/mdp5.o \
	$(LOCAL_DIR)/display.o \
	$(LOCAL_DIR)/mipi_dsi.o \
	$(LOCAL_DIR)/mipi_dsi_phy.o \
	$(LOCAL_DIR)/mipi_dsi_autopll.o \
	$(LOCAL_DIR)/spmi.o \
	$(LOCAL_DIR)/bam.o \
	$(LOCAL_DIR)/certificate.o \
	$(LOCAL_DIR)/image_verify.o \
	$(LOCAL_DIR)/crypto_hash.o \
	$(LOCAL_DIR)/crypto5_eng.o \
	$(LOCAL_DIR)/crypto5_wrapper.o \
	$(LOCAL_DIR)/dev_tree.o \
	$(LOCAL_DIR)/gpio.o \
	$(LOCAL_DIR)/dload_util.o \
	$(LOCAL_DIR)/shutdown_detect.o \
	$(LOCAL_DIR)/regulator.o

PANIC_REBOOT_MODE ?= NORMAL_DLOAD
DEFINES += PANIC_REBOOT_MODE=$(PANIC_REBOOT_MODE)

ifneq ($(filter 1,$(ENABLE_SECAPP_LOADER) $(VERIFIED_BOOT) $(VERIFIED_BOOT_2)),)
OBJS += \
	$(LOCAL_DIR)/secapp_loader.o \
	$(LOCAL_DIR)/qseecom_lk.o
endif

ifneq ($(filter 1,$(VERIFIED_BOOT) $(VERIFIED_BOOT_2)),)
OBJS += $(LOCAL_DIR)/boot_verifier.o
endif

ifeq ($(ENABLE_FBCON_DISPLAY_MSG),1)
DEFINES += FBCON_DISPLAY_MSG=1
OBJS += \
	$(LOCAL_DIR)/menu_keys_detect.o \
	$(LOCAL_DIR)/display_menu.o
endif

ifeq ($(VERIFIED_BOOT_2), 1)
include platform/msm_shared/avb/rules.mk
endif

ifeq ($(ENABLE_RPMB_SUPPORT), 1)
include platform/msm_shared/rpmb/rules.mk
endif

ifeq ($(ENABLE_DISPLAY),0)
DEFINES := $(filter-out DISPLAY_SPLASH_SCREEN=1, $(DEFINES))
endif

ifneq ($(filter DEVICE_TREE=1, $(DEFINES)),)
MODULES += lib/libfdt
endif

# Default to libufdt for DTBOs if device tree is enabled
DTBO_BACKEND ?= $(if $(filter $(DEFINES), DEVICE_TREE=1),libufdt,none)
ifeq ($(DTBO_BACKEND), libufdt)
MODULES += lib/libufdt
else ifeq ($(DTBO_BACKEND), libfdt)
MODULES += lib/libfdt
else ifeq ($(DTBO_BACKEND), none)
DEFINES += DTBO_DISABLED=1
else
$(error Unknown DTBO backend: $(DTBO_BACKEND))
endif

CRYPTO_SW_BACKEND ?= openssl
ifeq ($(CRYPTO_SW_BACKEND), openssl)
MODULES += lib/openssl
else ifeq ($(CRYPTO_SW_BACKEND), none)
ifneq ($(SIGNED_KERNEL)$(VERIFIED_BOOT)$(VERIFIED_BOOT_2),)
$(error Crypto software backend is required for secure boot)
endif
OBJS := $(filter-out $(LOCAL_DIR)/image_verify.o, $(OBJS))
else
$(error Unknown crypto software backend: $(CRYPTO_SW_BACKEND))
endif
