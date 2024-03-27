extern "C" {void app_main();}

#include <Lora_Service.h>

void app_main()
{
    char buff[100];
    Lora_Service lora_service(LORA_EN_PIN);

    lora_service.init();
    for(int i = 0; ; i++)
    {
        sprintf(buff, "selam %d", i);
        lora_service.sent_data(buff);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}