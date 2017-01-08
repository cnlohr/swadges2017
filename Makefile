include user.cfg
SRCS = user/user_main.c esp82xx_simplified/uart.c esp82xx_simplified/esp82xxutil.c user/ws2812_i2s.c user/esp_rawsend.c user/customnmi.S user/gpio_buttons.c user/ssid.c
-include esp82xx_simplified/common.mf
-include esp82xx_simplified/main.mf


CFLAGS+=

% :
	$(warning This is the empty rule. Something went wrong.)
	@true

ifndef TARGET
$(info Modules were not checked out... use git clone --recursive in the future. Pulling now.)
$(shell git submodule update --init --recursive)
endif

# Example for a custom rule.
# Most of the build is handled in main.mf
.PHONY : showvars
showvars:
	$(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))
	true

