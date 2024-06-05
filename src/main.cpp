#include "pcb_defines.h"
#include <esp_sleep.h>

#include <Temp_Driver.h>
#include <PompaDriver.h>
#include <Distance_Driver.h>
#include <Lora_Service.h>
#include "LiteJSON.h"
#include "esp_mac.h"
#include "ADC_Driver.h"
#include "PH_Driver.h"
#include "Turbidity_Driver.h"

#define DEEP_SLEEP_SEC  300
#define PUMP_EN_SEC     3

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
    PH_Driver ph_driver = PH_Driver(PH_EN_PIN);
    Turbidity_Driver turbidity_driver = Turbidity_Driver(TURB_EN_PIN);
    Lora_Service lora_service = Lora_Service(LORA_EN_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SCK_PIN, LORA_RST_PIN, LORA_CS_PIN);

    LiteJSON lite_json = LiteJSON(250); // {}

    float temp = -99.0f, dist = -99.0f, ph_lvl = -1.0f;
    uint8_t mac_str[8], bat_lvl = 0;
    int turb_lvl = 0;
    esp_efuse_mac_get_default(mac_str);

    while(1)
    {
        //fill mac address
        fill_mac_str(lite_json, mac_str);

        //pull water
        pompaDriver.enable();
        pompaDriver.rotateRight(PUMP_EN_SEC * 1000);
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait x seconds for water to settle

        //measure temp
        temp_driver.enable();
        temp_driver.init();
        temp_driver.measure(&temp);
        temp_driver.disable();
        lite_json.add_pair("temp", temp);

        //measure distance
        distance_driver.enable();
        distance_driver.measure_distance(&dist);
        distance_driver.disable();
        lite_json.add_pair("dist", dist);

        //measure battery level
        adc_driver.measure_bat_level(&bat_lvl);
        lite_json.add_pair("bat", bat_lvl);

        //measure Turbidity
        turbidity_driver.enable();
        adc_driver.measure_turbidity(&turb_lvl);
        turbidity_driver.disable();
        lite_json.add_pair("turb", turb_lvl);

        //measure PH
        ph_driver.enable();
        adc_driver.measure_ph_level(&ph_lvl);
        ph_driver.disable();
        lite_json.add_pair("ph", ph_lvl);

        //sent data
        lora_service.enable();
        lora_service.init();
        lora_service.sent_data(lite_json.serialize());
        lora_service.disable();

        lite_json.clear();

        pompaDriver.rotateLeft(PUMP_EN_SEC * 1000);
        pompaDriver.disable();
        esp_deep_sleep(DEEP_SLEEP_SEC * 1000000);
    }
}