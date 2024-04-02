#ifndef __DISTANCE_SERVICE_H
#define __DISTANCE_SERVICE_H

#include <stdio.h>
#include <stdbool.h>
#include <esp_err.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>

#define MAX_DISTANCE_CM 500 // 5m max

class Distance_Service
{
    public:
        Distance_Service(gpio_num_t en_gpio, gpio_num_t trig_gpio, gpio_num_t echo_gpio);
        esp_err_t measure_distance(float *val);
        esp_err_t enable();
        esp_err_t disable();
    private:
        gpio_num_t en_pin;
        gpio_num_t trig_pin;
        gpio_num_t echo_pin;
        
        const char *tag = "Distance_Service";
        
        ultrasonic_sensor_t sensor;
};
#endif