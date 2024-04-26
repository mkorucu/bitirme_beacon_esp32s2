#ifndef __PCB_DEFINES_H
#define __PCB_DEFINES_H

#include <driver/gpio.h>
#include <hal/adc_types.h>

#define BAT_ADC_PIN     GPIO_NUM_1
#define BAT_ADC_CHAN    ADC_CHANNEL_0
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

#define PH_EN_PIN       GPIO_NUM_16
#define PH_ADC_PIN      GPIO_NUM_17
#define PH_ADC_CHAN     ADC_CHANNEL_6

#define TURB_ADC_PIN    GPIO_NUM_18
#define TURB_ADC_CHAN   ADC_CHANNEL_7
#define TURB_EN_PIN     GPIO_NUM_21

#endif