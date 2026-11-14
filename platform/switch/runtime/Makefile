DEVKITPRO ?= /opt/devkitpro
DEVKITA64 ?= $(DEVKITPRO)/devkitA64
RUST_TOOLCHAIN ?= nightly-2026-01-20
TARGET := ethornell_switch
BUILD := build
ROMFS ?= romfs
CC := $(DEVKITA64)/bin/aarch64-none-elf-gcc
export PATH := $(DEVKITA64)/bin:$(DEVKITPRO)/tools/bin:$(PATH)
CFLAGS := -std=gnu17 -O2 -g -Wall -Wextra -ffunction-sections -fdata-sections -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE -I$(DEVKITPRO)/libnx/include -I../../console/include
LDFLAGS := -specs=$(DEVKITPRO)/libnx/switch.specs -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE -Wl,--gc-sections -Wl,-Map,$(TARGET).map -L$(DEVKITPRO)/libnx/lib
RUST_LIB := ../../console/target/aarch64-switch/release/libethornell_console.a
LIBC_VERSION := 0.2.186
LIBC_SHA256 := 68ab91017fe16c622486840e4c83c9a37afeff978bd239b5293d61ece587de66
LIBC_DIR := $(BUILD)/libc-$(LIBC_VERSION)
# build-std resolves the pinned nightly's own lockfile separately from the
# application. Both libc copies must use the same devkitA64 ABI corrections.
STD_LIBC_VERSION := 0.2.178
STD_LIBC_SHA256 := 37c93d8daa9d8a012fd8ab92f088405fb202ea0b6ab73ee2482ae66af4f42091
STD_LIBC_DIR := $(BUILD)/libc-$(STD_LIBC_VERSION)
LIBC_PATCH := ../patches/libc-horizon-aarch64-stat.patch
OBJECTS := $(BUILD)/main.o $(BUILD)/gpu.o $(BUILD)/deko.o
SHADERS := $(ROMFS)/shaders/ethornell_vsh.dksh $(ROMFS)/shaders/ethornell_fsh.dksh
.PHONY: all rust clean
all: $(TARGET).nro
rust: $(LIBC_DIR)/.patched $(STD_LIBC_DIR)/.patched
	cd ../../console && rustup run $(RUST_TOOLCHAIN) cargo build --release --target ../switch/rust/aarch64-switch.json -Zbuild-std=std,panic_abort --config 'patch.crates-io.libc.path="../switch/runtime/$(LIBC_DIR)"' --config 'patch.crates-io.libc-std.package="libc"' --config 'patch.crates-io.libc-std.path="../switch/runtime/$(STD_LIBC_DIR)"'
$(BUILD) $(ROMFS)/shaders:
	mkdir -p $@
$(LIBC_DIR)/.patched: LIBC_ARCHIVE_SHA256 := $(LIBC_SHA256)
$(STD_LIBC_DIR)/.patched: LIBC_ARCHIVE_SHA256 := $(STD_LIBC_SHA256)
$(BUILD)/libc-%/.patched: $(LIBC_PATCH) | $(BUILD)
	curl -sSfL -o $(BUILD)/libc-$*.crate https://static.crates.io/crates/libc/libc-$*.crate
	printf '%s  %s\n' '$(LIBC_ARCHIVE_SHA256)' '$(BUILD)/libc-$*.crate' | sha256sum -c -
	tar -xzf $(BUILD)/libc-$*.crate -C $(BUILD)
	cd $(BUILD)/libc-$* && patch -p1 < ../../../patches/libc-horizon-aarch64-stat.patch
	touch $@
$(BUILD)/%.o: source/%.c | $(BUILD)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@
$(ROMFS)/shaders/ethornell_vsh.dksh: shaders/ethornell_vsh.glsl | $(ROMFS)/shaders
	uam --stage=vert --out=$@ $<
$(ROMFS)/shaders/ethornell_fsh.dksh: shaders/ethornell_fsh.glsl | $(ROMFS)/shaders
	uam --stage=frag --out=$@ $<
$(TARGET).elf: $(OBJECTS) rust
	$(CC) $(LDFLAGS) $(OBJECTS) $(RUST_LIB) -ldeko3d -lnx -lm -o $@
$(TARGET).nacp:
	nacptool --create "Ethornell" "ethornell-rs" "0.1.0" $@ --titleid=0100E7A0E1100001
$(TARGET).nro: $(TARGET).elf $(TARGET).nacp $(SHADERS)
	elf2nro $< $@ --nacp=$(TARGET).nacp --romfsdir=$(ROMFS)
clean:
	rm -rf $(BUILD) $(TARGET).elf $(TARGET).nacp $(TARGET).map $(TARGET).nro
-include $(OBJECTS:.o=.d)
