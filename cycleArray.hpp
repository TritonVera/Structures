#ifndef __CYCLE_ARRAY_HPP
#define __CYCLE_ARRAY_HPP
#pragma once
#include <cstdint>
#include <cstdlib>
#include <cmath>

template <typename T>
class CycleArray
{
public:
    CycleArray(size_t arraySize) : beginIterator((T *)calloc(arraySize, sizeof(T))),
                                   endIterator(beginIterator + arraySize),
                                   nextElemIterator(beginIterator),
                                   lastElemIterator(endIterator - 1) {}

    CycleArray(const T initSymbol, size_t arraySize) : CycleArray(arraySize)
    {
        T *appendIterator = beginIterator;
        while (appendIterator != endIterator)
            *beginIterator = initSymbol;
    }

    CycleArray(const T *initData, size_t arraySize) : CycleArray(arraySize)
    {
        size_t i = 0;
        T *appendIterator = beginIterator;
        while (appendIterator != endIterator)
            *beginIterator = initData[i++ % arraySize];
    }

    ~CycleArray()
    {
        free(beginIterator);
    }

    void append(const T inputElement)
    {
        *(nextElemIterator++) = inputElement;
        if (nextElemIterator == endIterator)
            nextElemIterator = beginIterator;
        ++lastElemIterator;
        if (lastElemIterator == endIterator)
            lastElemIterator = beginIterator;
    }

    T &operator[](size_t index)
    {
        if (index >= size())
            throw 0;
        return lastElemIterator[convertElemIndex(index)];
    }

    T operator[](size_t index) const
    {
        if (index >= size())
            return 0;
        return lastElemIterator[convertElemIndex(index)];
    }

    void clear(const T initSymbol = 0)
    {
        T *appendIterator = beginIterator;
        while (appendIterator != endIterator)
            *beginIterator = initSymbol;
        nextElemIterator = beginIterator;
        lastElemIterator = endIterator;
    }

    size_t size() const
    {
        return endIterator - beginIterator;
    }

    const T *begin() const
    {
        return beginIterator;
    }

    const T *end() const
    {
        return endIterator;
    }

private:
    T *const beginIterator;
    T *const endIterator;
    T *nextElemIterator;
    T *lastElemIterator;

    int convertElemIndex(int index) const
    {
        return -index + ((lastElemIterator - beginIterator < index) ? size() : 0);
    }
};

#endif // __CYCLE_ARRAY_HPP