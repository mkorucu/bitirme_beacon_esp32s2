#include "Distance_Driver.h"

Distance_Driver::Distance_Driver(gpio_num_t en_gpio, gpio_num_t trig_gpio, gpio_num_t echo_gpio) : en_pin(en_gpio), trig_pin(trig_gpio), echo_pin(echo_gpio)
{
    gpio_config_t pGPIOConfig = {
        .pin_bit_mask = (1ULL << en_pin),
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_ERROR_CHECK(gpio_config(&pGPIOConfig));

    ESP_ERROR_CHECK(enable());
    sensor = {
        .trigger_pin = trig_pin,
        .echo_pin = echo_pin
    };
    if (ultrasonic_init(&sensor) != ESP_OK)
    {
        ESP_LOGE(tag, "init failed.");
    }
    ESP_LOGI(tag, "Service was initialized.");
}

esp_err_t Distance_Driver::measure_distance(float *dist)
{
    esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, dist);

    switch (res)
    {
        case ESP_OK:
            *dist = *dist * 100;                
            ESP_LOGI(tag, "Distance: %0.1f cm\n", *dist);
            break;
        case ESP_ERR_ULTRASONIC_PING:
            ESP_LOGE(tag, "Cannot ping (device is in invalid state)\n");
            break;
        case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
            ESP_LOGE(tag, "Ping timeout (no device found)\n");
            break;
        case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
            ESP_LOGE(tag, "Echo timeout (i.e. distance too big)\n");
            break;
        default:
            ESP_LOGE(tag, "%s\n", esp_err_to_name(res));
    }
    if (res != ESP_OK)
        *dist = -99.0f;
    return res;
}

esp_err_t Distance_Driver::enable()
{
    return gpio_set_level(en_pin, 1);
}

esp_err_t Distance_Driver::disable()
{
    return gpio_set_level(en_pin, 0);
}
