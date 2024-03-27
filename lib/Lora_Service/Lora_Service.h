#ifndef LORA_SERVICE_H
#define LORA_SERVICE_H

#ifdef MQTT_SERVICE_H
#include "MQTT_Service.h"
#endif
#include "stdint.h"
#include <string.h>
#include "lora.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


class Lora_Service
{
public:
    #ifdef MQTT_SERVICE_H
    Lora_Service(MQTT_Service *MQTT_service);
    #endif
    Lora_Service(gpio_num_t enable_pin);
    ~Lora_Service();
    int init();
    void disable_lora();
    void sent_data(char *data);
    void start_receiver(char *topic_name);
private:
    #ifdef MQTT_SERVICE_H
    MQTT_Service *mqtt_service;
    #endif
    const gpio_num_t enable_pin;
    uint8_t buff[256];
    char buff_str[257];
    char topic_name[32];
    const char *tag = "LORA";
    void receiver_task();
    static void task_wrapper(void *args){((Lora_Service*)args)->receiver_task();};
};



#endif