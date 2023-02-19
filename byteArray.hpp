#ifndef __BYTE_ARRAY_HPP
#define __BYTE_ARRAY_HPP
#pragma once
// #include <bitMasks.h>
#include <cstdint>
#include <cstdlib>
#include <cmath>
// #include <extra/uint128.h>

enum ByteAppendStatus : bool
{
    NoAppend = false,
    Append = true,
};

enum class ByteEndianness : bool
{
    Little = true,
    Big = false,
};

class ByteArray
{
public:
    ByteArray();
    void operator=(const ByteArray &copy);
    ByteArray(const ByteArray &copy);
    void operator=(ByteArray &&copy);
    ByteArray(ByteArray &&copy);
    explicit ByteArray(size_t inSize);
    explicit ByteArray(const char *inData);
    explicit ByteArray(const char *inData, size_t inSize);
    ~ByteArray();

    void changeCapacity(size_t newSize);
    void changeSize(size_t newSize);
    size_t max_size() const;

    ByteAppendStatus pushFront(const char appendData);
    ByteAppendStatus pushBack(const char appendData);
    char popFront();
    char popBack();

    ByteAppendStatus appendChar(const uint8_t appendNum);
    ByteAppendStatus appendArray(const ByteArray &appendString);
    ByteAppendStatus appendString(const char *appendString);
    ByteAppendStatus appendString(const char *appendString, size_t sizeString);
    ByteAppendStatus appendShort(const uint16_t appendNum, ByteEndianness endian = ByteEndianness::Big);
    ByteAppendStatus appendLong(const uint32_t appendNum, ByteEndianness endian = ByteEndianness::Big);
    ByteAppendStatus appendDoubleLong(const uint64_t appendNum, ByteEndianness endian = ByteEndianness::Big);
    ByteAppendStatus appendFloat(const float appendNum, ByteEndianness endian = ByteEndianness::Big);
    ByteAppendStatus appendDouble(const double appendNum, ByteEndianness endian = ByteEndianness::Big);

    ByteArray extractArray(size_t inSize);
    uint8_t extractChar();
    uint16_t extractShort(ByteEndianness endian = ByteEndianness::Big);
    uint32_t extractLong(ByteEndianness endian = ByteEndianness::Big);
    uint64_t extractDoubleLong(ByteEndianness endian = ByteEndianness::Big);
    float extractFloat(ByteEndianness endian = ByteEndianness::Big);
    double extractDouble(ByteEndianness endian = ByteEndianness::Big);

    ByteArray &operator>>(char &inData);
    ByteArray &operator>>(uint8_t &inData);
    ByteArray &operator>>(uint16_t &inData);
    ByteArray &operator>>(uint32_t &inData);
    ByteArray &operator>>(uint64_t &inData);
    ByteArray &operator>>(int8_t &inData);
    ByteArray &operator>>(int16_t &inData);
    ByteArray &operator>>(int32_t &inData);
    ByteArray &operator>>(int64_t &inData);
    ByteArray &operator>>(float &inData);
    ByteArray &operator>>(double &inData);
    ByteArray &operator>>(ByteArray &inData);
    ByteArray &operator>>(char *const inData);

    ByteArray &operator<<(char inData);
    ByteArray &operator<<(uint8_t inData);
    ByteArray &operator<<(uint16_t inData);
    ByteArray &operator<<(uint32_t inData);
    ByteArray &operator<<(uint64_t inData);
    ByteArray &operator<<(int8_t inData);
    ByteArray &operator<<(int16_t inData);
    ByteArray &operator<<(int32_t inData);
    ByteArray &operator<<(int64_t inData);
    ByteArray &operator<<(float inData);
    ByteArray &operator<<(double inData);
    ByteArray &operator<<(const ByteArray &inData);
    ByteArray &operator<<(const char *inData);

    size_t size() const;
    void clear();
    bool full();
    bool empty();
    void reverse();

    char *begin();
    char *end();
    const char *begin() const;
    const char *end() const;
    const char *cbegin() const;
    const char *cend() const;
    char *rbegin();
    char *rend();
    const char *rbegin() const;
    const char *rend() const;
    char const *crbegin() const;
    char const *crend() const;

    char &at(size_t position);
    const char &at(size_t position) const;
    char &operator[](size_t position);
    const char &operator[](size_t position) const;
    char &front();
    const char &front() const;
    char &back();
    const char &back() const;

    uint8_t charData(size_t position);
    uint16_t shortData(size_t position, ByteEndianness endian = ByteEndianness::Big);
    uint32_t longData(size_t position, ByteEndianness endian = ByteEndianness::Big);
    uint64_t doubleLongData(size_t position, ByteEndianness endian = ByteEndianness::Big);
    float floatData(size_t position, ByteEndianness endian = ByteEndianness::Big);
    double doubleData(size_t position, ByteEndianness endian = ByteEndianness::Big);

private:
    char *beginIterator;
    char *endIterator;
    char *nextIterator;

    void appendByte(const uint8_t appendNum);
    uint8_t extractByte();
};

#endif // __BYTE_ARRAY_HPP