#include "Temp_Driver.h"

Temp_Driver::Temp_Driver(gpio_num_t enable_gpio, gpio_num_t data_bus): en_pin(enable_gpio), data_pin(data_bus), owb(NULL), device(NULL)
{
    gpio_config_t pGPIOConfig = {
        .pin_bit_mask = (1ULL << en_pin),
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_ERROR_CHECK(disable());
    ESP_ERROR_CHECK(gpio_config(&pGPIOConfig));
    
    ESP_ERROR_CHECK(gpio_pulldown_dis(data_bus));
    ESP_ERROR_CHECK(gpio_pullup_en(data_bus));
}

Temp_Driver::~Temp_Driver()
{
    ds18b20_free(&device);
    owb_uninitialize(owb);
}

esp_err_t Temp_Driver::init()
{
    ESP_ERROR_CHECK(enable());
    // Stable readings require a brief period before communication
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // Create a 1-Wire bus, using the RMT timeslot driver
    owb = owb_rmt_initialize(&rmt_driver_info, data_pin, RMT_CHANNEL_1, RMT_CHANNEL_0);
    owb_use_crc(owb, true);  // enable CRC check for ROM code

    // Find all connected devices
    ESP_LOGI(tag,"Searching device..");
    OneWireBus_SearchState search_state = {0};
    bool found = false;
    owb_search_first(owb, &search_state, &found);
    if(found)
    {
        ESP_LOGI(tag,"Found device");
    }
    else
    {
        ESP_LOGI(tag,"No DS18B20 devices detected!");
        return (ESP_FAIL);
    }
    
    // Create DS18B20 devices on the 1-Wire bus
    device = ds18b20_malloc();
    ds18b20_init_solo(device, owb);          // only one device on bus
    ds18b20_use_crc(device, true);           // enable CRC check on all reads
    ds18b20_set_resolution(device, DS18B20_RESOLUTION_12_BIT);
    ESP_LOGI("SERVICE", "INIT DONE.");
    
    return (ESP_OK);
}

esp_err_t Temp_Driver::enable()
{
    return (gpio_set_level(en_pin, 0));
}

esp_err_t Temp_Driver::disable()
{
    return (gpio_set_level(en_pin, 1));
}

esp_err_t Temp_Driver::measure(float *val)
{
    //Read temperatures more efficiently by starting conversions on all devices at the same time
    DS18B20_ERROR err;

    ds18b20_convert_all(owb);
    ds18b20_wait_for_conversion(device);
    
    err = ds18b20_read_temp(device, val);
    if (err != DS18B20_OK)
    {
        ESP_LOGI(tag,"error while reading!");
        return ESP_FAIL;
    }
    ESP_LOGI(tag,"Temperature readings (degrees C): %.2fC", *val);
    
    return ESP_OK;
}
