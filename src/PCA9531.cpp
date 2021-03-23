/**
 * @file PCA9531.cpp.
 * @brief PCA9531 class source file
*/

#include "PCA9531.h"

PCA9531::PCA9531(I2C *i2c, char address)
{
    _i2c = i2c;
    addr = address;
}

PCA9531::~PCA9531()
{
}

void PCA9531::setLEDs(uint16_t state)
{
    setSelectorLEDs(state, LS0);
    setSelectorLEDs((state >> 8), LS1);
}

void PCA9531::setSelectorLEDs(uint8_t state, uint8_t selector)
{
    char temp[3];

    temp[0] = addr;
    temp[1] = selector;
    temp[2] = state;

    _i2c->write(addr, temp, 3);
}