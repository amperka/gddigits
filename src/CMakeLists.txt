
set(FIRMWARE_NAME firmware)

set(${FIRMWARE_NAME}_BOARD uno)
set(${FIRMWARE_NAME}_SRCS app.cpp)

set(${FIRMWARE_NAME}_PORT /dev/ttyACM0)
set(${FIRMWARE_NAME}_SERIAL picocom @INPUT_PORT@ -b 9600 -l )

generate_arduino_firmware(${FIRMWARE_NAME})
