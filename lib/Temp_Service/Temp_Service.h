#ifndef TEMP_SERVICE_H
#define TEMP_SERVICE_H

#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "owb.h"
#include "owb_rmt.h"
#include "ds18b20.h"

class Temp_Service
{
    public:
        Temp_Service(gpio_num_t enable_gpio, gpio_num_t data_bus);
        ~Temp_Service();
        esp_err_t enable_service();
        esp_err_t disable_service();
        esp_err_t measure(float *val);
    private:
        gpio_num_t en_pin;
        gpio_num_t data_pin;

        const char *tag = "Temp_Service";
    
        OneWireBus *owb;
        owb_rmt_driver_info rmt_driver_info;
        DS18B20_Info *device;
};

#endif