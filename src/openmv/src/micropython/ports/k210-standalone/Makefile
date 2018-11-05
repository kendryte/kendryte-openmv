.PHONY: clean_all build
include ../../py/mkenv.mk

CROSS_COMPILE=riscv64-unknown-elf-
SDK_PATH ?= ../../../../../..
MPY_MOD_PATH ?= mpy-mod
FS_PORT_PATH ?= spiffs-port
BOARD_DRIVERS_PATH ?= board-drivers

#hutu
MPY_ROOT_PATH ?= ../..
OMV_MOD_PATH ?= ../../../omv

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

FROZEN_MPY_DIR ?= buildin-py

# include py core make definitionsvers/uarths.c
include $(TOP)/py/py.mk

# mpy header
INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -Iconfig
INC += -Iinclude
INC += -I$(TOP)/lib/timeutils
INC += -I$(SDK_PATH)/lib/bsp/include
INC += -I$(SDK_PATH)/lib/drivers/include
INC += -I$(SDK_PATH)/lib/syslog/include
INC += -I$(SDK_PATH)/lib/math

# mpy mod header
INC += -I$(MPY_MOD_PATH)/machine/include
INC += -I$(MPY_MOD_PATH)/uos/include

# board drivers header
INC += -I$(BOARD_DRIVERS_PATH)/include

# spiffs ports header
INC += -Ispiffs-port/include

# sdk header
INC += -I$(SDK_PATH)/lib/bsp/include
INC += -I$(SDK_PATH)/lib/drivers/include
INC += -I$(SDK_PATH)/lib/utils/include
INC += -I$(SDK_PATH)/lib/math

# spiffs header
INC += -Ispiffs/src
INC += -Ispiffs/src/default

#hutu omv header
INC += -I$(OMV_MOD_PATH)
INC += -I$(OMV_MOD_PATH)/py
INC += -I$(OMV_MOD_PATH)/nn
INC += -I$(OMV_MOD_PATH)/img
INC += -I$(OMV_MOD_PATH)/boards/OPENMV3

INC += -I$(MPY_ROOT_PATH)/py
INC += -I$(MPY_ROOT_PATH)//lib/mp-readline
INC += -I$(MPY_ROOT_PATH)//lib/oofatfs

# compile option
C_DEFINES ?=
C_DEFINES += -DCONFIG_LOG_COLORS \
			 -DCONFIG_LOG_ENABLE \
			 -DCONFIG_LOG_LEVEL=LOG_VERBOSE \
			 -DDEBUG=1 \
			 -DFPGA_PLL \
			 -DLOG_KERNEL \
			 -D__riscv64

ARCH_FLAGS = -march=rv64imafdc -mabi=lp64d

BOTH_FLAGS = -mcmodel=medany \
			 -fno-common \
			 -ffunction-sections \
			 -fdata-sections \
			 -fstrict-volatile-bitfields \
			 -fno-zero-initialized-in-bss \
			 -Os \
			 -ggdb \
			 -Wall \
			 -Werror=all \
			 -Wno-error=unused-function \
			 -Wno-error=unused-but-set-variable \
			 -Wno-error=unused-variable \
			 -Wno-error=deprecated-declarations \
			 -Wextra \
			 -Werror=frame-larger-than=65536 \
			 -Wno-unused-parameter \
			 -Wno-sign-compare \
			 -Wno-error=missing-braces \
			 -Wno-error=return-type \
			 -Wno-error=pointer-sign \
			 -Wno-missing-braces \
			 -Wno-pointer-to-int-cast \
			 -Wno-strict-aliasing \
			 -Wno-implicit-fallthrough \
			 -Linclude/

CFLAGS_RV64_K210 = -lm \
				   -mfdiv \
				   -mdiv \
				   -std=gnu11 \
				   -Wno-old-style-declaration \
				   -g \
				   $(C_DEFINES) \
				   $(BOTH_FLAGS)

CFLAGS = $(INC) \
		 -std=gnu11 \
		 $(CFLAGS_RV64_K210) \
		 -g -Wno-error=unused-label \
		 -Wno-error=unused-const-variable= \
		 -Wno-error=format= \
		 -Wno-error=parentheses


CFLAGS_MOD += $(C_DEFINES)
CFLAGS += $(CFLAGS_MOD)

LDPPFLAGS += \
	-DDUSE_BINUTILS_2_19=1
LDFLAGS += --gc-sections

LDFLAGS = -lm $(BOTH_FLAGS) -T $(SDK_PATH)/lds/kendryte.ld \
        -nostartfiles \
        -static \
        -Wl,--gc-sections \
        -Wl,-static \
        -Wl,--start-group \
        -Wl,--whole-archive \
        -Wl,--no-whole-archive \
        -Wl,--end-group \
        -Wl,-EL

LIBS =

MODULES_SRC_C= \
		$(MPY_MOD_PATH)/uos/moduos.c \
		$(MPY_MOD_PATH)/machine/modmachine.c \
		$(MPY_MOD_PATH)/machine/machine_uarths.c\
		$(MPY_MOD_PATH)/machine/machine_uart.c \
		$(MPY_MOD_PATH)/machine/machine_pin.c \
		$(MPY_MOD_PATH)/machine/machine_pwm.c \
		$(MPY_MOD_PATH)/machine/machine_timer.c\
		$(MPY_MOD_PATH)/machine/machine_st7789.c\
		$(MPY_MOD_PATH)/machine/machine_ov2640.c\
		$(MPY_MOD_PATH)/machine/machine_burner.c\
		$(MPY_MOD_PATH)/machine/machine_face_detect.c\
		$(MPY_MOD_PATH)/machine/machine_spiflash.c\
		$(MPY_MOD_PATH)/machine/machine_zmodem.c\
		$(MPY_MOD_PATH)/machine/machine_fpioa.c\
		$(MPY_MOD_PATH)/machine/machine_ws2812.c

LIB_SRC_C ?=
MICROPY_FATFS=1
ifeq ($(MICROPY_FATFS), 1)
LIB_SRC_C += \
lib/oofatfs/ff.c \
lib/oofatfs/option/unicode.c
endif

FS_SRC_C = \
		$(FS_PORT_PATH)/spiffs-port.c \
		spiffs/src/spiffs_cache.c \
		spiffs/src/spiffs_check.c \
		spiffs/src/spiffs_gc.c \
		spiffs/src/spiffs_hydrogen.c \
		spiffs/src/spiffs_nucleus.c \
		file_io.c

BOARD_DRIVERS = $(BOARD_DRIVERS_PATH)/lcd.c\
				$(BOARD_DRIVERS_PATH)/st7789.c\
				$(BOARD_DRIVERS_PATH)/ov2640.c\
				$(BOARD_DRIVERS_PATH)/ov5640.c\
				$(BOARD_DRIVERS_PATH)/w25qxx.c\
				$(BOARD_DRIVERS_PATH)/face_detect.c\
				$(BOARD_DRIVERS_PATH)/region_layer.c\
				$(BOARD_DRIVERS_PATH)/uart_core.c\
				$(BOARD_DRIVERS_PATH)/ws2812b.c

MPY_LIB_SRC = lib/timeutils/timeutils.c \
			  lib/utils/sys_stdio_mphal.c
			  
#OMV_SRC_C = \
	$(OMV_MOD_PATH)/xalloc.c           \
	$(OMV_MOD_PATH)fb_alloc.c          \
	$(OMV_MOD_PATH)umm_malloc.c        \
	$(OMV_MOD_PATH)ff_wrapper.c        \
	$(OMV_MOD_PATH)ini.c               \
	$(OMV_MOD_PATH)framebuffer.c       \
	$(OMV_MOD_PATH)array.c             \
	$(OMV_MOD_PATH)cambus.c            \
	$(OMV_MOD_PATH)ov9650.c            \
	$(OMV_MOD_PATH)ov2640.c            \
	$(OMV_MOD_PATH)ov7725.c            \
	$(OMV_MOD_PATH)mt9v034.c           \
	$(OMV_MOD_PATH)lepton.c            \
	$(OMV_MOD_PATH)sensor.c            \
	$(OMV_MOD_PATH)soft_i2c.c          \
	$(OMV_MOD_PATH)mutex.c             \
	$(OMV_MOD_PATH)trace.c

OMV_SRC_C += \
	$(OMV_MOD_PATH)/xalloc.c            \
	$(OMV_MOD_PATH)/fb_alloc.c          \
	$(OMV_MOD_PATH)/umm_malloc.c        \
	$(OMV_MOD_PATH)/ff_wrapper.c        \
	$(OMV_MOD_PATH)/ini.c               \
	$(OMV_MOD_PATH)/framebuffer.c       \
	$(OMV_MOD_PATH)/array.c             \
	$(OMV_MOD_PATH)/mutex.c             \
	$(OMV_MOD_PATH)/trace.c

OMV_SRC_C += \
	$(OMV_MOD_PATH)/img/binary.c                \
	$(OMV_MOD_PATH)/img/blob.c                  \
	$(OMV_MOD_PATH)/img/clahe.c                 \
	$(OMV_MOD_PATH)/img/draw.c                  \
	$(OMV_MOD_PATH)/img/qrcode.c                \
	$(OMV_MOD_PATH)/img/apriltag.c              \
	$(OMV_MOD_PATH)/img/dmtx.c                  \
	$(OMV_MOD_PATH)/img/zbar.c                  \
	$(OMV_MOD_PATH)/img/fmath.c                 \
	$(OMV_MOD_PATH)/img/fsort.c                 \
	$(OMV_MOD_PATH)/img/qsort.c                 \
	$(OMV_MOD_PATH)/img/fft.c                   \
	$(OMV_MOD_PATH)/img/filter.c                \
	$(OMV_MOD_PATH)/img/haar.c                  \
	$(OMV_MOD_PATH)/img/imlib.c                 \
	$(OMV_MOD_PATH)/img/collections.c           \
	$(OMV_MOD_PATH)/img/stats.c                 \
	$(OMV_MOD_PATH)/img/integral.c              \
	$(OMV_MOD_PATH)/img/integral_mw.c           \
	$(OMV_MOD_PATH)/img/kmeans.c                \
	$(OMV_MOD_PATH)/img/lab_tab.c               \
	$(OMV_MOD_PATH)/img/xyz_tab.c               \
	$(OMV_MOD_PATH)/img/yuv_tab.c               \
	$(OMV_MOD_PATH)/img/rainbow_tab.c           \
	$(OMV_MOD_PATH)/img/rgb2rgb_tab.c           \
	$(OMV_MOD_PATH)/img/invariant_tab.c         \
	$(OMV_MOD_PATH)/img/mathop.c                \
	$(OMV_MOD_PATH)/img/pool.c                  \
	$(OMV_MOD_PATH)/img/point.c                 \
	$(OMV_MOD_PATH)/img/rectangle.c             \
	$(OMV_MOD_PATH)/img/bmp.c                   \
	$(OMV_MOD_PATH)/img/ppm.c                   \
	$(OMV_MOD_PATH)/img/gif.c                   \
	$(OMV_MOD_PATH)/img/mjpeg.c                 \
	$(OMV_MOD_PATH)/img/fast.c                  \
	$(OMV_MOD_PATH)/img/agast.c                 \
	$(OMV_MOD_PATH)/img/orb.c                   \
	$(OMV_MOD_PATH)/img/template.c              \
	$(OMV_MOD_PATH)/img/phasecorrelation.c      \
	$(OMV_MOD_PATH)/img/shadow_removal.c        \
	$(OMV_MOD_PATH)/img/font.c                  \
	$(OMV_MOD_PATH)/img/jpeg.c                  \
	$(OMV_MOD_PATH)/img/lbp.c                   \
	$(OMV_MOD_PATH)/img/eye.c                   \
	$(OMV_MOD_PATH)/img/hough.c                 \
	$(OMV_MOD_PATH)/img/line.c                  \
	$(OMV_MOD_PATH)/img/lsd.c                   \
	$(OMV_MOD_PATH)/img/sincos_tab.c            \
	$(OMV_MOD_PATH)/img/edge.c                  \
	$(OMV_MOD_PATH)/img/hog.c                   \
	$(OMV_MOD_PATH)/img/selective_search.c

#OMV_SRC_C +=  \
	$(OMV_MOD_PATH)/nn/nn.c


#OMV_SRC_C +=  \
	$(OMV_MOD_PATH)/py/py_helper.c             \
	$(OMV_MOD_PATH)/py/py_omv.c                \
	$(OMV_MOD_PATH)/py/py_sensor.c             \
	$(OMV_MOD_PATH)/py/py_image.c              \
	$(OMV_MOD_PATH)/py/py_time.c               \
	$(OMV_MOD_PATH)/py/py_lcd.c                \
	$(OMV_MOD_PATH)/py/py_gif.c                \
	$(OMV_MOD_PATH)/py/py_mjpeg.c              \
	$(OMV_MOD_PATH)/py/py_cpufreq.c            \
	$(OMV_MOD_PATH)/py/py_nn.c

OMV_SRC_C +=  \
	$(OMV_MOD_PATH)/py/py_helper.c             \
	$(OMV_MOD_PATH)/py/py_omv.c                \
	$(OMV_MOD_PATH)/py/py_image.c              \
	$(OMV_MOD_PATH)/py/py_gif.c                \
	$(OMV_MOD_PATH)/py/py_mjpeg.c
SRC_C = \
                $(MODULES_SRC_C)\
                main.c \
                $(BOARD_DRIVERS) \
                $(MPY_LIB_SRC) \
                $(FS_SRC_C) \
		$(LIB_SRC_C) \
		$(OMV_SRC_C)

PY_SRC_C = \
        help.c \
        lib/utils/stdout_helpers.c \
		lib/utils/interrupt_char.c \
		lib/utils/pyexec.c \
		lib/libc/string0.c \
		lib/mp-readline/readline.c 


# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) 
# Append any auto-generated sources that are needed by sources listed in SRC_QSTR
SRC_QSTR_AUTO_DEPS +=

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o)) $(addprefix $(BUILD)/, $(PY_SRC_C:.c=.o))

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

all: micropython

micropython: $(OBJ)
	$(AR) r micropython.a $(OBJ)


clean:  clean_py
			rm -rf *.a
			make -C ../../mpy-cross clean
			rm -rf ../../mpy-cross/build
			rm -rf ../../omv



include $(TOP)/py/mkrules.mk


