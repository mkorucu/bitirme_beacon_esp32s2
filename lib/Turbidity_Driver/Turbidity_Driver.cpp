#include "Turbidity_Driver.h"

Turbidity_Driver::Turbidity_Driver(gpio_num_t en_gpio_num) : enable_pin(en_gpio_num)
{
    gpio_config_t pGPIOConfig = {
        .pin_bit_mask = (1ULL << enable_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&pGPIOConfig));
    disable();
}

void Turbidity_Driver::enable()
{
    ESP_ERROR_CHECK(gpio_set_level(enable_pin, 0));
    vTaskDelay(100/ portTICK_PERIOD_MS);
}

void Turbidity_Driver::disable()
{
    ESP_ERROR_CHECK(gpio_set_level(enable_pin, 1));
    vTaskDelay(100/ portTICK_PERIOD_MS);
}
