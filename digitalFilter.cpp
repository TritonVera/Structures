#include "digitalFilter.hpp"
#include <K1921VK01T.h>
#include <bitMasks.h>

DigitalFilter::DigitalFilter(size_t bufferSize, uint16_t *const inputCoef, uint16_t *const outputCoef) : CycleArray((uint16_t)0, bufferSize + 1)
{
    this->coefA = (uint16_t *)calloc(bufferSize + 1, sizeof(uint16_t));
    this->coefB = (uint16_t *)calloc(bufferSize + 1, sizeof(uint16_t));
    setOutputCoef(outputCoef);
    setInputCoef(inputCoef);
}

DigitalFilter::~DigitalFilter()
{
    free(this->coefA);
    free(this->coefB);
}

void DigitalFilter::setOutputCoef(uint16_t *const coef)
{
    this->coefA[size() - 1] = 0;
    if (coef == nullptr)
        for (size_t i = 0; i < size() - 1; ++i)
            this->coefA[i] = 0;
    else
        for (size_t i = 0; i < size() - 1; ++i)
            this->coefA[i] = coef[i];
}

void DigitalFilter::setInputCoef(uint16_t *const coef)
{
    this->coefB[0] = 1;
    this->coefB[size() - 1] = 0;
    if (coef == nullptr)
        for (size_t i = 1; i < size() - 1; ++i)
            this->coefB[i] = 0;
    else
        for (size_t i = 0; i < size() - 1; ++i)
            this->coefB[i] = coef[i];
}

DigitalFilter &DigitalFilter::operator<<(uint16_t &rhs)
{
    // Переменные
    uint32_t inputSum = rhs;
    auto checkOddSize = this->size();

    // Двойное умножение со сложнением
    for (size_t i = 0; i < checkOddSize / 2; ++i)
    {
        uint32_t bufferX = this->operator[](2 * i) + (this->operator[](2 * i + 1) << 16);
        inputSum += __SMUAD(bufferX, *((uint32_t *)&(this->coefA[2 * i])));
    }

    // Округление результата
    if (inputSum & BIT_MASK_15)
        this->append((inputSum >> 16) + 1);
    else
        this->append(inputSum >> 16);
    return *this;
}

DigitalFilter &DigitalFilter::operator>>(uint16_t &rhs)
{
    uint32_t outputSum = 0;
    auto checkOddSize = this->size();

    // Двойное умножение со сложнением
    for (size_t i = 0; i < checkOddSize / 2; ++i)
    {
        uint32_t bufferX = this->operator[](2 * i) + (this->operator[](2 * i + 1) << 16);
        outputSum += __SMUAD(bufferX, *((uint32_t *)&(this->coefB[2 * i])));
    }

    // Округление результата
    if (outputSum & BIT_MASK_15)
        rhs = (outputSum >> 16) + 1;
    else
        rhs = (outputSum >> 16);
        
    return *this;
}
