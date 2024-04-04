#include "Temp_Driver.h"
#include "pcb_defines.h"

extern "C"{
    void app_main();
}

#include <PompaDriver.h>
#include "pcb_defines.h"

void app_main()
{
    PompaDriver pompaDriver(POMPA_EN_PIN, POMPA_RIGHT_PIN, POMPA_LEFT_PIN);
    
    pompaDriver.enablePompa();
    while(1)
    {
        pompaDriver.rotateRight(1500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        pompaDriver.rotateLeft(1500);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}