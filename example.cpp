#include "setup.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void distanceDisplay(void *arg)
{
    distance::gpioINFO *gpio = (distance::gpioINFO *)arg;
    while (1)
    {
        if (distance::measure(gpio))
        {
            displayAndWarning(gpio);
        }
        else
        {
            ESP_LOGE(TAG, "Error can't measure distance");
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

extern "C" void app_main()
{
    static distance::gpioINFO gpio = {
        .trig = GPIO_NUM_4,
        .echo = GPIO_NUM_5,
        .warning = GPIO_NUM_2,
    };

    distance::gpioINIT(&gpio);

    xTaskCreatePinnedToCore(distanceDisplay, "displaying distance", 2048, &gpio, 5, NULL, 1);
}
