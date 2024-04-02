#include "pcb_defines.h"
#include "Distance_Service.h"

extern "C"{
    void app_main();
}

void app_main()
{
    float distance = -1;
    Distance_Service distance_service = Distance_Service(DIST_EN_PIN, DIST_TRIG_PIN, DIST_ECHO_PIN);

    esp_err_t ret = distance_service.measure_distance(&distance);
    distance_service.disable();
}