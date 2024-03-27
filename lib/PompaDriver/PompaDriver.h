#ifndef __POMPADRIVER_H
#define __POMPADRIVER_H

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
class PompaDriver
{
    public:
        PompaDriver(gpio_num_t enable_pin, gpio_num_t right_rotate_pin, gpio_num_t left_rotate_pin);
        ~PompaDriver();
        void enablePompa();
        void disablePompa();
        void rotateLeft(uint32_t ms);
        void rotateRight(uint32_t ms);
    private:
        const gpio_num_t pompaEnablePin;
        const gpio_num_t pompaRightPin;
        const gpio_num_t pompaLeftPin;
};

#endif