#ifndef __PCB_DEFINES_H
#define __PCB_DEFINES_H

#include <driver/gpio.h>

#define BAT_ADC_PIN     GPIO_NUM_1

#define TEMP_EN_PIN     GPIO_NUM_2
#define TEMP_BUS_PIN    GPIO_NUM_3

#define DIST_EN_PIN     GPIO_NUM_4
#define DIST_TRIG_PIN   GPIO_NUM_5
#define DIST_ECHO_PIN   GPIO_NUM_6

#define LORA_EN_PIN     GPIO_NUM_7
#define LORA_RST_PIN    GPIO_NUM_8
#define LORA_CS_PIN     GPIO_NUM_9
#define LORA_MOSI_PIN   GPIO_NUM_10
#define LORA_MISO_PIN   GPIO_NUM_11
#define LORA_SCK_PIN    GPIO_NUM_12

#define POMPA_EN_PIN    GPIO_NUM_14
#define POMPA_RIGHT_PIN GPIO_NUM_13
#define POMPA_LEFT_PIN  GPIO_NUM_15


#endif