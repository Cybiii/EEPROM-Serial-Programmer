# EEPROM Serial Programmer

An Arduino-based EEPROM programmer that uses shift registers to address and program/read EEPROMs via serial interface.

## Features

- **Read EEPROM contents**: Retrieve EEPROM data to serial monitor in hex format
- **Write EEPROM data**: Program custom data patterns to EEPROM
- **Erase EEPROM**: Fill entire EEPROM with 0xFF (erase state)
- **Serial output**: View EEPROM contents in organized hex dump
- **Shift register addressing**: Efficient address line control using 74HC595 shift registers

## Hardware Requirements

- Arduino Uno (or compatible)
- EEPROM (tested with common parallel EEPROMs)
- 74HC595 shift registers (for address lines)

## Pin Connections

- Pin 2 → Shift Register Data (SER)
- Pin 3 → Shift Register Clock (SRCLK)
- Pin 4 → Shift Register Latch (RCLK)

- Pins 5-12 → EEPROM Data lines D0-D7
- Pin 13 → EEPROM Write Enable (WE)
- Shift Register Outputs → EEPROM Address lines
- Output Enable (OE) controlled via shift register bit 7

## Circuit Description

The circuit uses shift registers to control the EEPROM address lines, allowing the Arduino to address up to 2048 bytes (11 address bits) with just 3 control pins. The EEPROM data lines connect directly to Arduino digital pins 5-12.

### Core Functions

- `setAddress(address, outputEnable)` - Set EEPROM address and control OE
- `writeEEPROM(address, data)` - Write a byte to specific EEPROM address
- `readEEPROM(address)` - Read a byte from specific EEPROM address
- `printContents(size)` - Display EEPROM contents in hex dump format
- `eraseEEPROM()` - Erase entire EEPROM (fill with 0xFF)
- `programEEPROM(data[], size)` - Program custom data array to EEPROM

### Usage

1. **Upload the sketch** to your Arduino
2. **Open Serial Monitor** at 57600 baud
   
### Sample Output

```
000: 01 ff f1 42 01 ff f1 42     01 ff f1 42 01 ff f1 42
010: 01 ff f1 42 01 ff f1 42     01 ff f1 42 01 ff f1 42
```

## Customization

To program your own data:

1. Modify the `customData[]` array with your desired byte values
2. Update the size parameter in `programEEPROM()` and `printContents()` calls
3. Upload the modified sketch

## Technical Notes

- **Write timing**: Uses 1µs write pulse with 5ms delay for EEPROM write cycle
- **Address range**: Supports up to 2048 bytes (2K EEPROM)
