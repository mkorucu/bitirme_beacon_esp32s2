#ifndef __PH_DRIVER_H
#define __PH_DRIVER_H

#include "ADC_Driver.h"

class PH_Driver
{
    public:
        PH_Driver(gpio_num_t en_gpio_num);
        void enable();
        void disable();
    private:
        gpio_num_t enable_pin;
};

#endif