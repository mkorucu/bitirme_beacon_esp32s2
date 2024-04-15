#include "PompaDriver.h"

PompaDriver::PompaDriver(gpio_num_t enable_pin, gpio_num_t right_rotate_pin, gpio_num_t left_rotate_pin) :pompaEnablePin(enable_pin), pompaRightPin(right_rotate_pin), pompaLeftPin(left_rotate_pin)
{
    gpio_config_t pGPIOConfig = {
        .pin_bit_mask = (1ULL << pompaEnablePin) | (1ULL << pompaRightPin) | (1ULL << pompaLeftPin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&pGPIOConfig));

    ESP_ERROR_CHECK(gpio_set_level(pompaEnablePin, 0));
    ESP_ERROR_CHECK(gpio_set_level(pompaRightPin, 0));
    ESP_ERROR_CHECK(gpio_set_level(pompaLeftPin, 0));
}

PompaDriver::~PompaDriver()
{
    ESP_ERROR_CHECK(gpio_set_level(pompaEnablePin, 0));
    ESP_ERROR_CHECK(gpio_set_level(pompaRightPin, 0));
    ESP_ERROR_CHECK(gpio_set_level(pompaLeftPin, 0));
}

void PompaDriver::enable()
{
    ESP_ERROR_CHECK(gpio_set_level(pompaEnablePin, 1));
}

void PompaDriver::disable()
{
    ESP_ERROR_CHECK(gpio_set_level(pompaEnablePin, 0));
}

void PompaDriver::rotateLeft(uint32_t ms)
{
    ESP_ERROR_CHECK(gpio_set_level(pompaLeftPin, 1));
    ESP_LOGI(tag, "ROTATING LEFT");
    vTaskDelay(ms / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(gpio_set_level(pompaLeftPin, 0));
}

void PompaDriver::rotateRight(uint32_t ms)
{
    ESP_ERROR_CHECK(gpio_set_level(pompaRightPin, 1));
    ESP_LOGI(tag, "ROTATING RIGHT");
    vTaskDelay(ms / portTICK_PERIOD_MS);
    ESP_ERROR_CHECK(gpio_set_level(pompaRightPin, 0));
}
