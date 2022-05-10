#include "DataFlow.h"

String DataFlow::readStringFromMemory(unsigned int addr, unsigned int sizeOf) const
{
  String result = "";
  EEPROM.begin(m_eepromBaud);
  for (int i = 0; i < sizeOf; i++) {
    result += char(EEPROM.read(addr));
    addr++;
  }
  return result;
}

void DataFlow::writeStringToMemory(unsigned int addr, String value) const
{
  EEPROM.begin(m_eepromBaud);
  byte size = value.length();
  for (int i = 0; i < size; i++) {
    char c = value[i];
    EEPROM.write(addr, c);
    addr++;
  }
  EEPROM.commit();
}

void DataFlow::writeByteToMemory(unsigned int addr, byte value) const
{
  EEPROM.begin(m_eepromBaud);
  if (EEPROM.read(addr) != value) {
    EEPROM.put(addr, value);
    EEPROM.commit();
  }
}

byte DataFlow::readByteFromMemory(unsigned int addr) const
{
  EEPROM.begin(m_eepromBaud);
  return EEPROM.read(addr);
}

void DataFlow::writeUintToMemory(unsigned int addr, unsigned int value) const
{
  EEPROM.begin(m_eepromBaud);
  if (EEPROM.read(addr) != value) {
    EEPROM.put(addr, value);
    EEPROM.commit();
  }
}

unsigned int DataFlow::readUintFromMemory(unsigned int addr) const
{
  EEPROM.begin(m_eepromBaud);
  unsigned int result;
  EEPROM.get(addr, result);
  return result;
}
