#ifndef __DIGITAL_FILTER_HPP
#define __DIGITAL_FILTER_HPP

#include "cycleArray.hpp"

class DigitalFilter : public CycleArray<uint16_t>
{
public:
    DigitalFilter(size_t bufferSize, uint16_t *const inputCoef = nullptr, uint16_t *const outputCoef = nullptr);
    ~DigitalFilter();

    void setOutputCoef(uint16_t *const inA);
    void setInputCoef(uint16_t *const inB);

    DigitalFilter &operator<<(uint16_t &rhs);
    DigitalFilter &operator>>(uint16_t &rhs);

private:
    uint16_t *coefA;
    uint16_t *coefB;
};

#endif // __DIGITAL_FILTER_HPP