#include "setup.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void distanceDisplay(void *arg)
{
    distance::gpioINFO *gpio = (distance::gpioINFO *)arg;
    float dis = 0;
    while (1)
    {
        char *distanceText = (char *)malloc(30);
        dis = measure(gpio);
        if (dis == -1)
        {
            return;
        }
        snprintf(distanceText, 20, "Distance: %.2f cm", dis);
        ESP_LOGI(TAG, "%s", distanceText);
        vTaskDelay(pdMS_TO_TICKS(500));
        free(distanceText);
    }
}

extern "C" void app_main()
{
    static distance::gpioINFO gpio = {
        .trig = GPIO_NUM_4,
        .echo = GPIO_NUM_5,
    };

    distance::gpioINIT(&gpio);

    xTaskCreatePinnedToCore(distanceDisplay, "displaying distance", 2048, &gpio, 5, NULL, 1);
}
