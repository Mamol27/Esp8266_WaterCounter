#include "MemValue.h"

void StringValue::load()
{
    if (!m_memOffset) {
        return;
    }
    if (!m_df) {
        return;
    }

    byte len = m_df->readByteFromMemory(m_memOffset);
    m_maxLen = m_df->readByteFromMemory(m_memOffset + sizeof(byte));
    m_value = m_df->readStringFromMemory(m_memOffset + sizeof(byte) + sizeof(byte), len);
};

void StringValue::save() const
{
    if (!m_memOffset) {
        return;
    }
    if (!m_df) {
        return;
    }

    m_df->writeByteToMemory(m_memOffset, m_value.length());
    m_df->writeByteToMemory(m_memOffset + sizeof(byte), m_maxLen);
    m_df->writeStringToMemory(m_memOffset + sizeof(byte) + sizeof(byte), m_value);
};

void StringValue::setValue(String value)
{
    if (value.length() > m_maxLen) {
        Serial.println("WARNING: value length exceeded. Value wasn't saved;");
        return;
    }
    m_value = value;
    save();
};

void StringValue::setOffsetAndValue(unsigned int memOffset, String value)
{
    setOffset(memOffset);
    setValue(value);
};

const String StringValue::getValue() const
{
    return m_value;
};

const byte StringValue::getLen() const
{
    return m_value.length();
};

const unsigned int StringValue::getEndOfRecord() const
{
    return m_memOffset + sizeof(unsigned int) + m_maxLen;
};

void StringValue::load(unsigned int memOffset)
{
    setOffset(memOffset);
    load();
};



void UintValue::load()
{
    if (!m_memOffset) {
        return;
    }

    if (!m_df) {
        return;
    }

    m_value = m_df->readUintFromMemory(m_memOffset);
};

void UintValue::save() const
{
    m_df->writeUintToMemory(m_memOffset, m_value);
};

void UintValue::setValue(unsigned int value)
{
    m_value = value;
    save();
};

void UintValue::setOffsetAndValue(unsigned int memOffset, unsigned int value)
{
    setOffset(memOffset);
    setValue(value);
};

const unsigned int UintValue::getValue() const
{
    return m_value;
};

const unsigned int UintValue::getEndOfRecord() const
{
    return m_memOffset + sizeof(unsigned int);
};

void UintValue::load(unsigned int memOffset)
{
    setOffset(memOffset);
    load();
};