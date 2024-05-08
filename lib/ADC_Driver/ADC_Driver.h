#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

#include "esp_log.h"
#include <driver/gpio.h>
#include <esp_adc/adc_cali.h>
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class ADC_Driver
{
    public:
        ADC_Driver(adc_channel_t battery_channel, adc_channel_t ph_channel, adc_channel_t turb_channel);
        void measure_bat_level(uint8_t *bat_lvl);
        void measure_ph_level(float *ph_lvl);
        void measure_turbidity(int *turb_lvl);
    private:
        adc_channel_t battery_channel;
        adc_oneshot_unit_handle_t adc1_handle;
        adc_cali_handle_t adc1_cali_handle;
        
        adc_channel_t   ph_channel;
        adc_channel_t turbidity_channel;
        adc_oneshot_unit_handle_t adc2_handle;
        adc_cali_handle_t adc2_cali_handle;

        const char *tag = "ADC_Driver";
};


#endif