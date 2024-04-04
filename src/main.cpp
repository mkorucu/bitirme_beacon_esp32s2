#include "Temp_Driver.h"
#include "pcb_defines.h"

extern "C"{
    void app_main();
}

void app_main()
{
    float temp = -1.0f;
    Temp_Driver temp_driver = Temp_Driver(TEMP_EN_PIN, TEMP_BUS_PIN);
    temp_driver.enable();
    temp_driver.init();
    esp_err_t ret = temp_service.measure(&temp);
}
