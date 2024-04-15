#include "pcb_defines.h"

#include <Temp_Driver.h>
#include <PompaDriver.h>
#include <Distance_Driver.h>
#include <Lora_Service.h>
#include "LiteJSON.h"
#include "esp_mac.h"

extern "C"{
    void app_main();
}

void fill_mac_str(LiteJSON &json_str, uint8_t *mac)
{
    char mac_str[18];
    sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(mac));
    printf("mac address: %s\n", mac_str);
    json_str.add_pair("mac", mac_str);
}

void app_main()
{
    PompaDriver pompaDriver = PompaDriver(POMPA_EN_PIN, POMPA_RIGHT_PIN, POMPA_LEFT_PIN);
    Temp_Driver temp_driver = Temp_Driver(TEMP_EN_PIN, TEMP_BUS_PIN);
    Distance_Driver distance_driver = Distance_Driver(DIST_EN_PIN, DIST_TRIG_PIN, DIST_ECHO_PIN);
    Lora_Service lora_service = Lora_Service(LORA_EN_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SCK_PIN, LORA_RST_PIN, LORA_CS_PIN);

    LiteJSON json_str = LiteJSON(256); // {}

    float temp = 0.0f, dist = 0.0f;

    uint8_t mac_str[8];
    esp_efuse_mac_get_default(mac_str);

    while(1)
    {
        //fill mac address
        fill_mac_str(json_str, mac_str);

        //pull water
        pompaDriver.enable();
        vTaskDelay(100 / portTICK_PERIOD_MS);
        pompaDriver.rotateRight(10000);
        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait 10 seconds for water to settle

        //measure temp
        temp_driver.init();
        temp_driver.measure(&temp);
        temp_driver.disable();
        json_str.add_pair("temp", temp);

        //measure distance
        distance_driver.enable();
        distance_driver.measure_distance(&dist);
        distance_driver.disable();
        json_str.add_pair("level", dist);

        //sent data
        lora_service.enable();
        lora_service.init();
        lora_service.sent_data(json_str.serialize());
        lora_service.disable();

        pompaDriver.rotateLeft(10000);
        vTaskDelay(10 * 1000 / portTICK_PERIOD_MS); // 1 saat
    }
}