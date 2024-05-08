#ifndef __TURBIDITY_DRIVER_H
#define __TURBIDITY_DRIVER_H

#include "ADC_Driver.h"

class Turbidity_Driver
{
    public:
        Turbidity_Driver(gpio_num_t en_gpio_num);
        void enable();
        void disable();
    private:
        gpio_num_t enable_pin;
};

#endif