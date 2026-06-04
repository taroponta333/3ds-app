TARGET      := mp4_player
BUILD       := build
# 動画デコードに必要なライブラリがあれば LIBS に追記します（例: -lavcodec などのffmpeg系）
LIBS        := -lctru -lm

# CIAビルド情報
APP_TITLE   := MP4 Player
APP_AUTHOR  := AuthorName
APP_PRODUCT := CTR-P-MP4P
APP_UNIQUE_ID := 0x12346 # 他のアプリと被らない固有のID

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/3ds_rules

.PHONY: all clean

all: $(TARGET).3dsx $(TARGET).cia

$(TARGET).3dsx: $(BUILD)/$(TARGET).elf
	3dsxtool $(BUILD)/$(TARGET).elf $(TARGET).3dsx

$(TARGET).cia: $(BUILD)/$(TARGET).elf App.rsf
	makerom -f cia -o $(TARGET).cia -elf $(BUILD)/$(TARGET).elf -rsf App.rsf -target t -exefslogo -major 1 -minor 0 -micro 0

# main.c がルート直下にある想定のコンパイルルール
$(BUILD)/$(TARGET).elf: main.c
	@mkdir -p $(BUILD)
	$(CC) -g -Wall -O2 -mfloat-abi=hard -mcpu=mpcore -mtune=mpcore -specs=3dsx.specs main.c $(LIBS) -I$(DEVKITPRO)/libctru/include -L$(DEVKITPRO)/libctru/lib -o $(BUILD)/$(TARGET).elf

clean:
	rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).cia
