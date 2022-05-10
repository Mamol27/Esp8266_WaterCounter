// LED.h
#ifndef DATAFLOW_h
#define DATAFLOW_h

#include <Arduino.h>
#include <EEPROM.h>

class DataFlow {
  private :
  unsigned int m_eepromBaud;

  public:
  DataFlow() : m_eepromBaud(4096) {};
  DataFlow(unsigned int eepromBaud) : m_eepromBaud(eepromBaud) {};
  
  void writeStringToMemory(unsigned int addr, String value) const;
  String readStringFromMemory(unsigned int addr, unsigned int sizeOf) const;
  
  void writeByteToMemory(unsigned int addr, byte value) const ;
  byte readByteFromMemory(unsigned int addr) const ;
  
  void writeUintToMemory(unsigned int addr, unsigned int value) const;
  unsigned int readUintFromMemory(unsigned int addr) const;
};

#endif
