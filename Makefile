
SPEED		= 115200
BUILD_DIR	= build
SRCDIR		= ./src/
SRCLIBDIR 	= ./libs
WWW_DIR		= ./www/
WWW_BIN		= webcontent.bin
WWW_MAXSIZE	= 57344
PORT_USER	= /dev/ttyUSB0
# linking libgccirom.a instead of libgcc.a causes reset when working with flash memory (ie spi_flash_erase_sector)
# linking libcirom.a causes conflicts with come std c routines (like strstr, strchr...)
LIBS		= -lminic -lm -lgcc -lhal -lphy -lpp -lnet80211 -lwpa -lmain -lfreertos -llwip
# this directory
ROOT_DIR	:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))) 
# let's assume the SDK is up there.

# homebrew installs the toolchain, but neglects to put it in your $PATH
PORT		:= $(PORT_USER)
XELF		:= xtensa-lx106-elf
AR		:= $(BIN_EXEC)$(XELF)-ar
CC		:= $(BIN_EXEC)$(XELF)-g++
CPP		:= $(CC)
LD		:= $(CC)
NM		:= $(BIN_EXEC)xt-nm
OBJCOPY		:= $(BIN_EXEC)$(XELF)-objcopy
OD		:= $(BIN_EXEC)$(XELF)-objdump
ESPTOOL		:= python ./script/esptool.py
INCLUDES	:= -I ./ESP8266_IOT_RTOS_SDK/include
INCLUDES	+= $(addprefix -I ./ESP8266_IOT_RTOS_SDK/include/, espressif lwip lwip/lwip lwip/ipv4 lwip/ipv6)
INCLUDES	+= -I ./ESP8266_IOT_RTOS_SDK/extra_include
INCLUDES	+= -I $(SRCLIBDIR)

# don't change -Os (or add other -O options) otherwise FLASHMEM and FSTR data will be duplicated in RAM
CFLAGS		= -g -Os -Wpointer-arith -Wl,-EL \
		-nostdlib -mlongcalls -mtext-section-literals \
		-fno-exceptions -fno-rtti -fno-inline-functions \
		-fno-threadsafe-statics -fno-use-cxa-atexit \
		-DICACHE_FLASH -D__ets__
LDFLAGS		= -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--gc-sections
LD_SCRIPT	= eagle.app.v6.ld
SDK_LIBDIR	:= -L./ESP8266_IOT_RTOS_SDK/lib
SDK_LDDIR	= ./ESP8266_IOT_RTOS_SDK/ld
OBJ		:= $(addprefix $(BUILD_DIR)/, \
		user_main.o fdvserial.o fdvsync.o \
		fdvutils.o fdvflash.o fdvprintf.o \
		fdvdebug.o fdvcstring.o fdvnetwork.o \
		fdvcollections.o fdvconfmanager.o fdvdatetime.o \
		fdvserialbinary.o fdvtask.o fdvgpio.o \
		fdvserialconsole.o fdvhttp.o fdvwwwhandlers.o)
WWW_ADDRS	= 0x6D000
TARGET_OUT	:= $(BUILD_DIR)/app.out
BINS		:= $(addprefix $(TARGET_OUT),-0x00000.bin -0x11000.bin)
WWW_CONTENT	= $(BUILD_DIR)/$(WWW_BIN)

.PHONY: all flash clean flashweb flashdump flasherase fresh mkdirs $(WWW_CONTENT)

all: mkdirs $(BINS)

mkdirs:
	@-mkdir -p $(BUILD_DIR)

$(TARGET_OUT): $(BUILD_DIR)/libuser.a
	$(LD) $(ELF_LIBDIR) $(SDK_LIBDIR) -T$(SDK_LDDIR)/$(LD_SCRIPT) \
	      -Wl,-M >$(BUILD_DIR)/out.map $(LDFLAGS) \
	      -Wl,--start-group $(LIBS) $^ \
	      -Wl,--end-group -o $@
	$(OD) -h -j .data -j .rodata -j .bss -j .text -j .irom0.text $@
	$(OD) -t -j .text $@ >$(BUILD_DIR)/_text_content.map
	$(OD) -t -j .irom0.text $@ > $(BUILD_DIR)/_irom0_text_content.map

$(BINS): $(TARGET_OUT)
	$(ESPTOOL) elf2image $^

$(BUILD_DIR)/libuser.a: $(OBJ)
	$(AR) cru $@ $^
	
$(BUILD_DIR)/%.o: $(SRCDIR)%.c $(wildcard $(SRCDIR)*.h)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRCDIR)%.cpp $(wildcard $(SRCDIR)*.h)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

ESP_CMD := $(ESPTOOL) --port $(PORT) --baud $(SPEED)

flash: flashweb
	-$(ESP_CMD) write_flash 0x11000 $(TARGET_OUT)-0x11000.bin 0x00000 $(TARGET_OUT)-0x00000.bin

$(WWW_CONTENT):
	python ./script/binarydir.py $(WWW_DIR) $@ $(WWW_MAXSIZE)

flashweb: $(WWW_CONTENT)
	-$(ESP_CMD) write_flash $(WWW_ADDRS) $^

flashdump:
	-$(ESP_CMD) read_flash 0x0000 0x80000 flash.dump
	xxd flash.dump > flash.hex

flasherase:
	$(ESPTOOL) --port $(PORT) erase_flash
	
clean:
	-rm -rf $(BUILD_DIR)/*

fresh: clean $(BINS) flash
