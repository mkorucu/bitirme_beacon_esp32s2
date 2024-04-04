#ifndef TEMP_DRIVER_H
#define TEMP_DRIVER_H

#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"

class Temp_Driver
{
    public:
        Temp_Driver(gpio_num_t enable_gpio, gpio_num_t data_bus);
        ~Temp_Driver();
        esp_err_t init();
        esp_err_t enable();
        esp_err_t disable();
        esp_err_t measure(float *val);
    private:
        gpio_num_t en_pin;
        gpio_num_t data_pin;

        const char *tag = "Temp_Driver";
    
        OneWireBus *owb;
        owb_rmt_driver_info rmt_driver_info;
        DS18B20_Info *device;
};

#endif