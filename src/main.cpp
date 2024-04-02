#include "Temp_Service.h"

extern "C"{
    void app_main();
}

void app_main()
{
    float temp = -1.0f;
    Temp_Service temp_service = Temp_Service(GPIO_NUM_2, GPIO_NUM_3);

    temp_service.measure(&temp);
}
