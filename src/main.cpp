#include "Temp_Service.h"
#include "pcb_defines.h"

extern "C"{
    void app_main();
}

void app_main()
{
    float temp = -1.0f;
    Temp_Service temp_service = Temp_Service(TEMP_EN_PIN, TEMP_BUS_PIN);

    esp_err_t ret = temp_service.measure(&temp);
}
