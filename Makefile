BOARD_TAG    = uno
MONITOR_PORT = /dev/ttyACM0

# https://github.com/sudar/Arduino-Makefile
ARDUINO_LIBS = Wire LiquidCrystal_v2

USER_LIB_PATH = /
include /usr/share/arduino/Arduino.mk
