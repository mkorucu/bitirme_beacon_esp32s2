#ifndef __LORA_H__
#define __LORA_H__

#ifdef __cplusplus
extern "C"{
#endif

#include "../pcb_defines/pcb_defines.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"

#define CONFIG_SPI2_HOST    1

// SPI Stuff
#if CONFIG_SPI2_HOST
#define HOST_ID SPI2_HOST
#elif CONFIG_SPI3_HOST
#define HOST_ID SPI3_HOST
#endif


void lora_reset(void);
void lora_explicit_header_mode(void);
void lora_implicit_header_mode(int size);
void lora_idle(void);
void lora_sleep(void); 
void lora_receive(void);
int lora_get_irq(void);
void lora_set_tx_power(int level);
void lora_set_frequency(long frequency);
void lora_set_spreading_factor(int sf);
int lora_get_spreading_factor(void);
void lora_set_dio_mapping(int dio, int mode);
int lora_get_dio_mapping(int dio);
void lora_set_bandwidth(int sbw);
int lora_get_bandwidth(void);
void lora_set_coding_rate(int denominator);
int lora_get_coding_rate(void);
void lora_set_preamble_length(long length);
long lora_get_preamble_length(void);
void lora_set_sync_word(int sw);
void lora_enable_crc(void);
void lora_disable_crc(void);
void spi_init(gpio_num_t miso, gpio_num_t mosi, gpio_num_t sck,gpio_num_t rst, gpio_num_t cs);
int lora_init(void);
void lora_send_packet(uint8_t *buf, int size);
int lora_receive_packet(uint8_t *buf, int size);
int lora_received(void);
int lora_packet_lost(void);
int lora_packet_rssi(void);
float lora_packet_snr(void);
void lora_close(void);
int lora_initialized(void);
void lora_dump_registers(void);

#ifdef __cplusplus
}
#endif

#endif