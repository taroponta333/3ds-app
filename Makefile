TARGET		:=	my_3ds_app
BUILD		:=	build
LIBS		:=	-lctru -lm

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

.PHONY: all clean

all: $(TARGET).3dsx

$(TARGET).3dsx: $(BUILD)/$(TARGET).elf
	3dsxtool $(BUILD)/$(TARGET).elf $(TARGET).3dsx

$(BUILD)/$(TARGET).elf: main.c
	@mkdir -p $(BUILD)
	$(CC) -g -Wall -O2 -mfloat-abi=hard -mcpu=mpcore -mtune=mpcore -specs=3dsx.specs main.c $(LIBS) -I$(DEVKITPRO)/libctru/include -L$(DEVKITPRO)/libctru/lib -o $(BUILD)/$(TARGET).elf

clean:
	rm -rf $(BUILD) $(TARGET).3dsx
