// memvalue.h
#ifndef MEMVALUE_h
#define MEMVALUE_h

#include <Arduino.h>
#include "DataFlow.h"

class MemValue
{
protected:
  DataFlow *m_df;
  unsigned int m_memOffset = 0;

  MemValue(DataFlow *df) : m_df(df){};

public:
  virtual void save() const {};
  virtual void load(){};
  virtual const byte getLen() const {};
  virtual const unsigned int getEndOfRecord() const {};

  void setOffset(unsigned int memOffset)
  {
    m_memOffset = memOffset;
  };

  const unsigned int getOffset() const
  {
    return m_memOffset;
  };

  virtual void load(unsigned int memOffset)
  {
    setOffset(memOffset);
    load();
  };
};

class StringValue : public MemValue
{
private:
  String m_value = "";
  byte m_maxLen = 12;

public:
  StringValue(DataFlow *df) : MemValue(df){};

  StringValue(DataFlow *df, byte maxLen) : MemValue(df),
                                           m_maxLen(maxLen){};

  void load();
  void save() const;
  void setValue(String value);
  void setOffsetAndValue(unsigned int memOffset, String value);
  const String getValue() const;
  const byte getLen() const;
  const unsigned int getEndOfRecord() const;
  void load(unsigned int memOffset);
};

class UintValue : public MemValue
{
private:
  unsigned int m_value = 0;
public:
  UintValue(DataFlow *df) : MemValue(df){};

  void save() const;
  void load();
  void setValue(unsigned int value);
  void setOffsetAndValue(unsigned int memOffset, unsigned int value);
  const unsigned int getValue() const;
  const unsigned int getEndOfRecord() const;
  void load(unsigned int memOffset);
};

#endif
