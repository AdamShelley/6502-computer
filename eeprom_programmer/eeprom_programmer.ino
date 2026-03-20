#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

void setAddress(int address, bool outputEnable)
{
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

void writeEEPROM(int address, byte data)
{
  setAddress(address, false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++)
  {
    pinMode(pin, OUTPUT);
  }
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++)
  {
    digitalWrite(pin, data & 1);
    data >>= 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}

void setup()
{
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);
  Serial.println("READY");
}

void loop()
{
  if (Serial.available() >= 3)
  {
    byte addr_high = Serial.read();
    byte addr_low = Serial.read();
    byte data = Serial.read();

    int address = (addr_high << 8) | addr_low;
    writeEEPROM(address, data);

    Serial.write('K');
  }
}