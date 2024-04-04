#include "Lora_Service.h"

#ifdef MQTT_SERVICE_H
Lora_Service::Lora_Service(MQTT_Service *MQTT_service) : mqtt_service(MQTT_service) {}
#endif

Lora_Service::Lora_Service(gpio_num_t enable_pin) : enable_pin(enable_pin)
{
    gpio_config_t pGPIOConfig = {
    .pin_bit_mask = (1ULL << enable_pin),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&pGPIOConfig));

    ESP_ERROR_CHECK(disable());
}

Lora_Service::~Lora_Service()
{
    ESP_ERROR_CHECK(gpio_set_level(enable_pin, 1));
}

int Lora_Service::init()
{
    ESP_ERROR_CHECK(enable());
    vTaskDelay(100 / portTICK_PERIOD_MS);
    memset(buff, 0, 256);
    
    if (lora_init() == 0)
    {
        ESP_LOGE(pcTaskGetName(NULL), "Does not recognize the module");
        return -1;
    }
    lora_set_frequency(433e6); // 433MHz
    lora_enable_crc();
    lora_set_coding_rate(1);
    lora_set_bandwidth(7);
    lora_set_spreading_factor(7);
    return 0;
}

esp_err_t Lora_Service::enable()
{
    return (gpio_set_level(enable_pin, 0));
}

esp_err_t Lora_Service::disable()
{
    return (gpio_set_level(enable_pin, 1));
}

void Lora_Service::sent_data(char *data)
{
    memset(buff, 0, 256);
    memcpy(buff, data, strlen(data));
    lora_send_packet(buff, strlen(data));
    ESP_LOGI(tag, "%d byte packet sent...", strlen(data));
    int lost = lora_packet_lost();
    if (lost != 0)
    {
        ESP_LOGE(tag, "%d packets lost", lost);
    }
}

void Lora_Service::start_receiver(char *topic_name)
{
    strcpy(this->topic_name, topic_name);
    xTaskCreate(task_wrapper, "LoRa RX", 1024 * 3, this, 5, NULL);
}

void Lora_Service::receiver_task()
{
	
    while(1)
    {
		lora_receive(); // put into receive mode
		if (lora_received()) {
			int rxLen = lora_receive_packet(buff, sizeof(buff));
			ESP_LOGI(pcTaskGetName(NULL), "%d byte packet received:[%.*s]", rxLen, rxLen, buff);
			memcpy(buff_str, buff, 256);
            #ifdef MQTT_SERVICE_H
            mqtt_service->sent_data(topic_name, buff_str);
            #endif
		}
		vTaskDelay(1); // Avoid WatchDog alerts
	} // end while

}