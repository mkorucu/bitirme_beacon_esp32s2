#include "ADC_Driver.h"

ADC_Driver::ADC_Driver(adc_channel_t battery_channel, adc_channel_t ph_channel, adc_channel_t turb_channel) : battery_channel(battery_channel), ph_channel(ph_channel), turbidity_channel(turb_channel)
{

    // Initialize ADC_UNIT_1 in one-shot mode
    adc_oneshot_unit_init_cfg_t adc1_init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc1_init_config, &adc1_handle));

    adc_cali_line_fitting_config_t cali_config_1 = {
    .unit_id = ADC_UNIT_1,
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_12,        
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config_1, &adc1_cali_handle));


    // Initialize ADC_UNIT_2 in one-shot mode
    adc_oneshot_unit_init_cfg_t adc2_init_config = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc2_init_config, &adc2_handle));

    adc_cali_line_fitting_config_t cali_config_2 = {
    .unit_id = ADC_UNIT_2,
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_12,        
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config_2, &adc2_cali_handle));


    adc_oneshot_chan_cfg_t config = {
    .atten = ADC_ATTEN_DB_11,
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, battery_channel, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, ph_channel, &config));
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc2_handle, turbidity_channel, &config));

    ESP_LOGI(tag,"INITIALIZED.");
}

void ADC_Driver::measure_bat_level(uint8_t *lvl)
{
    int bat = 0;
    for(int i = 0, j = 0; i < 10; i++)
    {
        ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(adc1_handle,adc1_cali_handle, battery_channel, &j));
        bat += j;
        vTaskDelay(1);
    }
    bat *= 2;
    bat /= 10;
    *lvl = (bat - 3600) * 100 / 600;
    ESP_LOGI(tag, "bat adc: %d | battery level=%d", bat, *lvl);
}

void ADC_Driver::measure_ph_level(float *ph_lvl)
{
    int ph = 0;
    float calibration_value = 21.34 - 1.7, adc_lvl = 0;

    for (int i = 0, j = 0; i < 10; i++)
    {
        ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(adc2_handle,adc2_cali_handle, ph_channel, &j));
        ph += j;
        vTaskDelay(1);
    }
    adc_lvl = (float)ph / 10;
    *ph_lvl = adc_lvl * 2 / 1000 ;
    //*ph_lvl = -5.7 * (*ph_lvl) + calibration_value;
    *ph_lvl = 7 + ((2.5 - *ph_lvl) / 0.167) + 1;
    ESP_LOGI(tag, "ph_adc= %.1f | ph_res=%.1f",adc_lvl, *ph_lvl);
}

void ADC_Driver::measure_turbidity(int *turb_lvl)
{
    int turb = 0;
    for (int i = 0, j = 0; i < 10; i++)
    {
        ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(adc2_handle,adc2_cali_handle, turbidity_channel, &j));
        turb += j;
        vTaskDelay(1);
    }
    turb /= 10;
    ESP_LOGI(tag, "turb ADC res=%d", turb);
    *turb_lvl = turb;
}
