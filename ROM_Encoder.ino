#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST,
           (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

void writeEEPROM(int address, byte data) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  setAddress(address, false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }

  // Based on OE input pulse ~1000ms = ~1us
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(5);
}

byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }

  setAddress(address, /*outputEnable*/ true);
  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }

  return data;
}

void printContents(int size) {

  for (int base = 0; base < size; base += 16) {
    byte data[16];
    for (int offset = 0; offset < 16; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf,
            "%03x: %02x %02x %02x %02x %02x %02x %02x %02x     %02x %02x %02x "
            "%02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6],
            data[7], data[8], data[9], data[10], data[11], data[12], data[13],
            data[14], data[15]);

    Serial.println(buf);
  }
}

// Custom data
byte customData[] = {0x01, 0xff, 0xf1, 0x42, 0x01, 0xff, 0xf1, 0x42,
                     0x01, 0xff, 0xf1, 0x42, 0x01, 0xff, 0xf1, 0x42};

void eraseEEPROM() {
  for (int address = 0; address < 2048; address += 1) {
    writeEEPROM(address, 0xff);
  }
}

// Program EEPROM with custom data
void programEEPROM(byte data[], int size) {
  for (int address = 0; address < size; address += 1) {
    writeEEPROM(address, data[address]);
  }
}

void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  // Custom EEPROM encoding goes here:
  programEEPROM(customData, sizeof(customData));

  // Print contents on the Serial Monitor
  printContents(sizeof(customData));
}

void loop() {}