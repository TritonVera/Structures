#include <cstring>
#include <stdexcept>
#include <cmath>

#define guard(x) \
    if ((x))     \
        ;

union ShortToChars
{
    ShortToChars() : data(0) {}
    ShortToChars(const uint16_t &inData) : data(inData) {}
    uint16_t data;
    struct
    {
        unsigned char char0;
        unsigned char char1;
    };
};

union FloatLongToChars
{
    FloatLongToChars() : data(0) {}
    FloatLongToChars(const uint32_t &inData) : data(inData) {}
    FloatLongToChars(const float &inData) : dataFloat(inData) {}
    uint32_t data;
    float dataFloat;
    struct
    {
        unsigned char char0;
        unsigned char char1;
        unsigned char char2;
        unsigned char char3;
    };
};

union DoubleLongToChars
{
    DoubleLongToChars() : data(0) {}
    DoubleLongToChars(const uint64_t &inData) : data(inData) {}
    DoubleLongToChars(const double &inData) : dataFloat(inData) {}
    uint64_t data;
    double dataFloat;
    struct
    {
        unsigned char char0;
        unsigned char char1;
        unsigned char char2;
        unsigned char char3;
        unsigned char char4;
        unsigned char char5;
        unsigned char char6;
        unsigned char char7;
    };
};

#include "byteArray.hpp"

ByteArray::ByteArray() : beginIterator(nullptr),
                         endIterator(nullptr),
                         nextIterator(nullptr)
{
}

ByteArray::ByteArray(size_t stringSize) : beginIterator(new char[stringSize]),
                                          endIterator(beginIterator + stringSize),
                                          nextIterator(beginIterator)
{
}

ByteArray::ByteArray(const char *inData) : ByteArray(inData, strlen(inData))
{
}

ByteArray::ByteArray(const char *inData, size_t inSize) : ByteArray(inSize)
{
    memcpy(beginIterator, inData, inSize);
    nextIterator = endIterator;
}

ByteArray::ByteArray(const ByteArray &copy) : ByteArray(copy.beginIterator, copy.size())
{
}

void ByteArray::operator=(const ByteArray &copy)
{
    if (beginIterator)
        delete beginIterator;

    beginIterator = new char[copy.size()];
    endIterator = copy.endIterator;
    nextIterator = copy.nextIterator;

    memcpy(beginIterator, copy.beginIterator, copy.size());
}

void ByteArray::operator=(ByteArray &&move)
{
    char *tmpBegin = beginIterator;
    char *tmpEnd = endIterator;
    char *tmpNext = nextIterator;

    beginIterator = move.beginIterator;
    endIterator = move.endIterator;
    nextIterator = move.nextIterator;

    move.beginIterator = tmpBegin;
    move.endIterator = tmpEnd;
    move.nextIterator = tmpNext;
}

ByteArray::ByteArray(ByteArray &&move)
{
    beginIterator = move.beginIterator;
    endIterator = move.endIterator;
    nextIterator = move.nextIterator;

    move.beginIterator = nullptr;
    move.endIterator = nullptr;
    move.nextIterator = nullptr;
}

ByteArray::~ByteArray()
{
    if (beginIterator)
        delete beginIterator;
}

void ByteArray::changeCapacity(size_t newSize)
{
    char *newBegin = new char[newSize];
    size_t dataSize = std::min(size(), newSize);
    if (beginIterator)
    {
        memcpy(newBegin, beginIterator, dataSize);
        delete beginIterator;
    }
    beginIterator = newBegin;
    endIterator = newBegin + newSize;
    nextIterator = newBegin + dataSize;
}

void ByteArray::changeSize(size_t newSize)
{
    nextIterator = beginIterator + newSize;
}

size_t ByteArray::max_size() const
{
    return endIterator - beginIterator;
}

void ByteArray::appendByte(const uint8_t appendNum)
{
    *(nextIterator++) = appendNum;
}

uint8_t ByteArray::extractByte()
{
    return *(--nextIterator);
}

ByteAppendStatus ByteArray::pushBack(const char appendNum)
{
    guard(nextIterator < endIterator) else return NoAppend;

    appendByte(appendNum);
    return Append;
}

ByteAppendStatus ByteArray::appendChar(const uint8_t appendNum)
{
    guard(nextIterator < endIterator) else return NoAppend;

    appendByte(appendNum);
    return Append;
}

ByteAppendStatus ByteArray::appendShort(const uint16_t appendNum, ByteEndianness endian)
{
    guard(nextIterator + 2 <= endIterator) else return NoAppend;

    ShortToChars input(appendNum);
    switch (endian)
    {
    case ByteEndianness::Big:
        appendByte(input.char0);
        appendByte(input.char1);
        break;
    case ByteEndianness::Little:
        appendByte(input.char1);
        appendByte(input.char0);
        break;
    }
    return Append;
}

ByteAppendStatus ByteArray::appendLong(const uint32_t appendNum, ByteEndianness endian)
{
    guard(nextIterator + 4 <= endIterator) else return NoAppend;

    FloatLongToChars input(appendNum);
    switch (endian)
    {
    case ByteEndianness::Big:
        appendByte(input.char0);
        appendByte(input.char1);
        appendByte(input.char2);
        appendByte(input.char3);
        break;
    case ByteEndianness::Little:
        appendByte(input.char3);
        appendByte(input.char2);
        appendByte(input.char1);
        appendByte(input.char0);
        break;
    }
    return Append;
}

ByteAppendStatus ByteArray::appendDoubleLong(const uint64_t appendNum, ByteEndianness endian)
{
    // Проверка возможности добавления 8 новых символов
    guard(nextIterator + 8 <= endIterator) else return NoAppend;

    DoubleLongToChars input(appendNum);
    switch (endian)
    {
    case ByteEndianness::Big:
        appendByte(input.char0);
        appendByte(input.char1);
        appendByte(input.char2);
        appendByte(input.char3);
        appendByte(input.char4);
        appendByte(input.char5);
        appendByte(input.char6);
        appendByte(input.char7);
        break;
    case ByteEndianness::Little:
        appendByte(input.char7);
        appendByte(input.char6);
        appendByte(input.char5);
        appendByte(input.char4);
        appendByte(input.char3);
        appendByte(input.char2);
        appendByte(input.char1);
        appendByte(input.char0);
        break;
    }
    return Append;
}

ByteAppendStatus ByteArray::appendFloat(const float inNum, ByteEndianness endian)
{
    guard(nextIterator + 4 <= endIterator) else return NoAppend;

    FloatLongToChars input(inNum);
    switch (endian)
    {
    case ByteEndianness::Big:
        appendByte(input.char0);
        appendByte(input.char1);
        appendByte(input.char2);
        appendByte(input.char3);
        break;
    case ByteEndianness::Little:
        appendByte(input.char3);
        appendByte(input.char2);
        appendByte(input.char1);
        appendByte(input.char0);
        break;
    }
    return Append;
}

ByteAppendStatus ByteArray::appendDouble(const double inNum, ByteEndianness endian)
{
    guard(nextIterator + 8 <= endIterator) else return NoAppend;

    DoubleLongToChars input(inNum);
    switch (endian)
    {
    case ByteEndianness::Big:
        appendByte(input.char0);
        appendByte(input.char1);
        appendByte(input.char2);
        appendByte(input.char3);
        appendByte(input.char4);
        appendByte(input.char5);
        appendByte(input.char6);
        appendByte(input.char7);
        break;
    case ByteEndianness::Little:
        appendByte(input.char7);
        appendByte(input.char6);
        appendByte(input.char5);
        appendByte(input.char4);
        appendByte(input.char3);
        appendByte(input.char2);
        appendByte(input.char1);
        appendByte(input.char0);
        break;
    }
    return Append;
}

ByteAppendStatus ByteArray::appendArray(const ByteArray &appendString)
{
    // Проверка возможности добавления новых символов
    guard(nextIterator + appendString.size() <= endIterator) else return NoAppend;

    // Добавление новых сивролов
    for (auto elem : appendString)
        appendByte(elem);
    return Append;
}

ByteAppendStatus ByteArray::appendString(const char *inString)
{
    auto sizeString = strlen(inString);
    return appendString(inString, sizeString);
}

ByteAppendStatus ByteArray::appendString(const char *inString, size_t sizeString)
{
    // Проверка возможности добавления новых символов
    guard(nextIterator + sizeString <= endIterator) else return NoAppend;

    // Добавление новых символов
    for (size_t i = 0; i < sizeString; ++i)
        appendByte(inString[i]);
    return Append;
}

char ByteArray::popBack()
{
    guard(nextIterator > beginIterator) else return 0;
    return extractByte();
}

uint8_t ByteArray::extractChar()
{
    guard(nextIterator > beginIterator) else return 0;
    return extractByte();
}

uint16_t ByteArray::extractShort(ByteEndianness endian)
{
    guard(nextIterator - 2 >= beginIterator) else return 0;

    ShortToChars output(0);
    switch (endian)
    {
    case ByteEndianness::Big:
        output.char1 = extractByte();
        output.char0 = extractByte();
        break;
    case ByteEndianness::Little:
        output.char0 = extractByte();
        output.char1 = extractByte();
        break;
    }
    return output.data;
}

uint32_t ByteArray::extractLong(ByteEndianness endian)
{
    guard(nextIterator - 4 >= beginIterator) else return 0;

    FloatLongToChars output;
    switch (endian)
    {
    case ByteEndianness::Big:
        output.char3 = extractByte();
        output.char2 = extractByte();
        output.char1 = extractByte();
        output.char0 = extractByte();
        break;
    case ByteEndianness::Little:
        output.char0 = extractByte();
        output.char1 = extractByte();
        output.char2 = extractByte();
        output.char3 = extractByte();
        break;
    }
    return output.data;
}

uint64_t ByteArray::extractDoubleLong(ByteEndianness endian)
{
    guard(nextIterator - 8 >= beginIterator) else return 0;

    DoubleLongToChars output;
    switch (endian)
    {
    case ByteEndianness::Big:
        output.char7 = extractByte();
        output.char6 = extractByte();
        output.char5 = extractByte();
        output.char4 = extractByte();
        output.char3 = extractByte();
        output.char2 = extractByte();
        output.char1 = extractByte();
        output.char0 = extractByte();
        break;
    case ByteEndianness::Little:
        output.char0 = extractByte();
        output.char1 = extractByte();
        output.char2 = extractByte();
        output.char3 = extractByte();
        output.char4 = extractByte();
        output.char5 = extractByte();
        output.char6 = extractByte();
        output.char7 = extractByte();
        break;
    }
    return output.data;
}

float ByteArray::extractFloat(ByteEndianness endian)
{
    guard(nextIterator - 4 >= beginIterator) else return 0;

    FloatLongToChars output;
    switch (endian)
    {
    case ByteEndianness::Big:
        output.char3 = extractByte();
        output.char2 = extractByte();
        output.char1 = extractByte();
        output.char0 = extractByte();
        break;
    case ByteEndianness::Little:
        output.char0 = extractByte();
        output.char1 = extractByte();
        output.char2 = extractByte();
        output.char3 = extractByte();
        break;
    }
    return output.data;
}

double ByteArray::extractDouble(ByteEndianness endian)
{
    guard(nextIterator - 8 >= beginIterator) else return 0;

    DoubleLongToChars output;
    switch (endian)
    {
    case ByteEndianness::Big:
        output.char7 = extractByte();
        output.char6 = extractByte();
        output.char5 = extractByte();
        output.char4 = extractByte();
        output.char3 = extractByte();
        output.char2 = extractByte();
        output.char1 = extractByte();
        output.char0 = extractByte();
        break;
    case ByteEndianness::Little:
        output.char0 = extractByte();
        output.char1 = extractByte();
        output.char2 = extractByte();
        output.char3 = extractByte();
        output.char4 = extractByte();
        output.char5 = extractByte();
        output.char6 = extractByte();
        output.char7 = extractByte();
        break;
    }
    return output.data;
}

ByteArray ByteArray::extractArray(size_t inSize)
{
    guard(nextIterator - inSize > beginIterator) else return ByteArray();
    return ByteArray(nextIterator - inSize, inSize);
}

ByteAppendStatus ByteArray::pushFront(const char appendData)
{
    guard(nextIterator < endIterator) else return NoAppend;

    for (size_t i = 0; i < size(); ++i)
        nextIterator[-i] = nextIterator[-i - 1];
    *beginIterator = appendData;
    ++nextIterator;
    return Append;
}

char ByteArray::popFront()
{
    guard(nextIterator > beginIterator) else return NoAppend;

    char firstElement = *beginIterator;
    for (size_t i = 0; i < size(); ++i)
        beginIterator[i] = beginIterator[i + 1];
    --nextIterator;
    return firstElement;
}

char *ByteArray::begin()
{
    return beginIterator;
}

char *ByteArray::end()
{
    return nextIterator;
}

const char *ByteArray::begin() const
{
    return beginIterator;
}

const char *ByteArray::end() const
{
    return nextIterator;
}

char const *ByteArray::cbegin() const
{
    return beginIterator;
}

char const *ByteArray::cend() const
{
    return nextIterator;
}

char *ByteArray::rbegin()
{
    return &nextIterator[-1];
}

char *ByteArray::rend()
{
    return &beginIterator[-1];
}

const char *ByteArray::rbegin() const
{
    return &nextIterator[-1];
}

const char *ByteArray::rend() const
{
    return &beginIterator[-1];
}

char const *ByteArray::crbegin() const
{
    return &nextIterator[-1];
}

char const *ByteArray::crend() const
{
    return &beginIterator[-1];
}

size_t ByteArray::size() const
{
    return nextIterator - beginIterator;
}

void ByteArray::clear()
{
    nextIterator = beginIterator;
}

bool ByteArray::full()
{
    return nextIterator == endIterator;
}

bool ByteArray::empty()
{
    return nextIterator == beginIterator;
}

char &ByteArray::at(size_t position)
{
    guard(position < size()) else throw std::out_of_range("Out of range");
    return beginIterator[position];
}

const char &ByteArray::at(size_t position) const
{
    guard(position < size()) else throw std::out_of_range("Out of range");
    return beginIterator[position];
}

char &ByteArray::operator[](size_t position)
{
    guard(beginIterator + position < endIterator) else throw std::out_of_range("Out of range");
    return beginIterator[position];
}

const char &ByteArray::operator[](size_t position) const
{
    guard(beginIterator + position < endIterator) else throw std::out_of_range("Out of range");
    return beginIterator[position];
}

char &ByteArray::front()
{
    return *beginIterator;
}

char &ByteArray::back()
{
    return nextIterator[-1];
}

const char &ByteArray::front() const
{
    return *beginIterator;
}

const char &ByteArray::back() const
{
    return nextIterator[-1];
}

void ByteArray::reverse()
{
    guard(size() > 1) else return;

    auto halfSize = size() >> 1;
    for (size_t i = 0; i < halfSize; ++i)
    {
        auto tmp = beginIterator[i];
        beginIterator[i] = nextIterator[-i - 1];
        nextIterator[-i - 1] = tmp;
    }
}

uint8_t ByteArray::charData(size_t position)
{
    char out = at(position);
    return out;
}

uint16_t ByteArray::shortData(size_t position, ByteEndianness endian)
{
    guard(position + 2 <= size()) else return 0;

    ShortToChars out;
    switch (endian)
    {
    case ByteEndianness::Big:
        out.char1 = beginIterator[position + 1];
        out.char0 = beginIterator[position + 0];
        break;
    case ByteEndianness::Little:
        out.char0 = beginIterator[position + 1];
        out.char1 = beginIterator[position + 0];
        break;
    default:
        break;
    }
    return out.data;
}

uint32_t ByteArray::longData(size_t position, ByteEndianness endian)
{
    guard(position + 4 <= size()) else return 0;

    FloatLongToChars out;
    switch (endian)
    {
    case ByteEndianness::Big:
        out.char3 = beginIterator[position + 3];
        out.char2 = beginIterator[position + 2];
        out.char1 = beginIterator[position + 1];
        out.char0 = beginIterator[position + 0];
        break;
    case ByteEndianness::Little:
        out.char0 = beginIterator[position + 3];
        out.char1 = beginIterator[position + 2];
        out.char2 = beginIterator[position + 1];
        out.char3 = beginIterator[position + 0];
        break;
    default:
        break;
    }
    return out.data;
}

uint64_t ByteArray::doubleLongData(size_t position, ByteEndianness endian)
{
    guard(position + 8 <= size()) else return 0;

    DoubleLongToChars out;
    switch (endian)
    {
    case ByteEndianness::Big:
        out.char7 = beginIterator[position + 7];
        out.char6 = beginIterator[position + 6];
        out.char5 = beginIterator[position + 5];
        out.char4 = beginIterator[position + 4];
        out.char3 = beginIterator[position + 3];
        out.char2 = beginIterator[position + 2];
        out.char1 = beginIterator[position + 1];
        out.char0 = beginIterator[position + 0];
        break;
    case ByteEndianness::Little:
        out.char0 = beginIterator[position + 7];
        out.char1 = beginIterator[position + 6];
        out.char2 = beginIterator[position + 5];
        out.char3 = beginIterator[position + 4];
        out.char4 = beginIterator[position + 3];
        out.char5 = beginIterator[position + 2];
        out.char6 = beginIterator[position + 1];
        out.char7 = beginIterator[position + 0];
        break;
    default:
        break;
    }
    return out.data;
}

float ByteArray::floatData(size_t position, ByteEndianness endian)
{
    guard(position + 4 <= size()) else return 0;

    FloatLongToChars out;
    switch (endian)
    {
    case ByteEndianness::Big:
        out.char3 = beginIterator[position + 3];
        out.char2 = beginIterator[position + 2];
        out.char1 = beginIterator[position + 1];
        out.char0 = beginIterator[position + 0];
        break;
    case ByteEndianness::Little:
        out.char0 = beginIterator[position + 3];
        out.char1 = beginIterator[position + 2];
        out.char2 = beginIterator[position + 1];
        out.char3 = beginIterator[position + 0];
        break;
    default:
        break;
    }
    return out.data;
}

double ByteArray::doubleData(size_t position, ByteEndianness endian)
{
    guard(position + 8 <= size()) else return 0;

    DoubleLongToChars out;
    switch (endian)
    {
    case ByteEndianness::Big:
        out.char7 = beginIterator[position + 7];
        out.char6 = beginIterator[position + 6];
        out.char5 = beginIterator[position + 5];
        out.char4 = beginIterator[position + 4];
        out.char3 = beginIterator[position + 3];
        out.char2 = beginIterator[position + 2];
        out.char1 = beginIterator[position + 1];
        out.char0 = beginIterator[position + 0];
        break;
    case ByteEndianness::Little:
        out.char0 = beginIterator[position + 7];
        out.char1 = beginIterator[position + 6];
        out.char2 = beginIterator[position + 5];
        out.char3 = beginIterator[position + 4];
        out.char4 = beginIterator[position + 3];
        out.char5 = beginIterator[position + 2];
        out.char6 = beginIterator[position + 1];
        out.char7 = beginIterator[position + 0];
        break;
    default:
        break;
    }
    return out.data;
}

ByteArray &ByteArray::operator>>(char &inData)
{
    inData = popBack();
    return *this;
}

ByteArray &ByteArray::operator>>(uint8_t &inData)
{
    inData = extractByte();
    return *this;
}

ByteArray &ByteArray::operator>>(uint16_t &inData)
{
    inData = popBack();
    return *this;
}

ByteArray &ByteArray::operator>>(uint32_t &inData)
{
    inData = extractLong();
    return *this;
}

ByteArray &ByteArray::operator>>(uint64_t &inData)
{
    inData = extractDoubleLong();
    return *this;
}

ByteArray &ByteArray::operator>>(int8_t &inData)
{
    inData = extractChar();
    return *this;
}

ByteArray &ByteArray::operator>>(int16_t &inData)
{
    inData = extractShort();
    return *this;
}

ByteArray &ByteArray::operator>>(int32_t &inData)
{
    inData = extractLong();
    return *this;
}

ByteArray &ByteArray::operator>>(int64_t &inData)
{
    inData = extractDoubleLong();
    return *this;
}

ByteArray &ByteArray::operator>>(float &inData)
{
    inData = extractFloat();
    return *this;
}

ByteArray &ByteArray::operator>>(double &inData)
{
    inData = extractDouble();
    return *this;
}

ByteArray &ByteArray::operator<<(char inData)
{
    pushBack(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(uint8_t inData)
{
    appendChar(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(uint16_t inData)
{
    appendShort(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(uint32_t inData)
{
    appendLong(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(uint64_t inData)
{
    appendDoubleLong(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(int8_t inData)
{
    appendChar(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(int16_t inData)
{
    appendShort(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(int32_t inData)
{
    appendLong(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(int64_t inData)
{
    appendDoubleLong(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(float inData)
{
    appendFloat(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(double inData)
{
    appendDouble(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(const ByteArray &inData)
{
    appendArray(inData);
    return *this;
}

ByteArray &ByteArray::operator<<(const char *inData)
{
    appendString(inData);
    return *this;
}

ByteArray &ByteArray::operator>>(ByteArray &inData)
{
    inData = extractArray(this->size());
    return *this;
}

ByteArray &ByteArray::operator>>(char *const inData)
{
    memcpy(inData, beginIterator, strlen(beginIterator));
    clear();
    inData[strlen(inData)] = 0;
    return *this;
}