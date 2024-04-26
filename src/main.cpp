#include "pcb_defines.h"

#include <Temp_Driver.h>
#include <PompaDriver.h>
#include <Distance_Driver.h>
#include <Lora_Service.h>
#include "LiteJSON.h"
#include "esp_mac.h"
#include "ADC_Driver.h"
#include "PH_Driver.h"
extern "C"{
    void app_main();
}

void fill_mac_str(LiteJSON &json_str, uint8_t *mac)
{
    char mac_str[18];
    sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(mac));
    printf("mac address: %s\n", mac_str);
    json_str.add_pair("beacon_mac", mac_str);
}

void app_main()
{
    ADC_Driver adc_driver = ADC_Driver(BAT_ADC_CHAN, PH_ADC_CHAN, TURB_ADC_CHAN);
    PompaDriver pompaDriver = PompaDriver(POMPA_EN_PIN, POMPA_RIGHT_PIN, POMPA_LEFT_PIN);
    Temp_Driver temp_driver = Temp_Driver(TEMP_EN_PIN, TEMP_BUS_PIN);
    Distance_Driver distance_driver = Distance_Driver(DIST_EN_PIN, DIST_TRIG_PIN, DIST_ECHO_PIN);
    PH_Driver ph_driver = PH_Driver(PH_ADC_PIN);
    Lora_Service lora_service = Lora_Service(LORA_EN_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SCK_PIN, LORA_RST_PIN, LORA_CS_PIN);

    LiteJSON lite_json = LiteJSON(256); // {}

    float temp = 0.0f, dist = 0.0f, ph_lvl = -1.0f;
    uint32_t  data_index = 0;
    uint8_t mac_str[8], bat_lvl = 0, turb_lvl;
    esp_efuse_mac_get_default(mac_str);

    while(1)
    {
        //fill mac address
        fill_mac_str(lite_json, mac_str);

        //pull water
        pompaDriver.enable();
        vTaskDelay(100 / portTICK_PERIOD_MS);
        pompaDriver.rotateRight(1000);
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait 10 seconds for water to settle

        //measure temp
        temp_driver.init();
        temp_driver.measure(&temp);
        temp_driver.disable();
        lite_json.add_pair("temp", temp);

        //measure distance
        distance_driver.enable();
        distance_driver.measure_distance(&dist);
        distance_driver.disable();
        lite_json.add_pair("water_level", dist);

        //measure battery level
        adc_driver.measure_bat_level(&bat_lvl);
        lite_json.add_pair("bat_lvl", bat_lvl);

        //measure PH
        ph_driver.enable();
        adc_driver.measure_ph_level(&ph_lvl);
        ph_driver.disable();
        lite_json.add_pair("ph_lvl", ph_lvl);
        
        //sent data
        lora_service.enable();
        lora_service.init();
        lora_service.sent_data(lite_json.serialize());
        lora_service.disable();

        lite_json.clear();

        pompaDriver.rotateLeft(1000);
        vTaskDelay(1 * 1000 / portTICK_PERIOD_MS); // 1 saat
    }
}