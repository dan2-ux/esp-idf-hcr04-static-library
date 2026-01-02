#include "setup.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TimerHandle_t timer1;

void timerFunction(TimerHandle_t xTimer)
{
    gpio_set_level(GPIO_NUM_2, 0);
}

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
        if (dis < 10)
        {
            gpio_set_level(GPIO_NUM_2, 1);

            if (xTimerIsTimerActive(timer1) != false)
            {
                xTimerStop(timer1, portMAX_DELAY);
            }
            xTimerStart(timer1, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
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

    gpio_reset_pin(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    timer1 = xTimerCreate("turn led on when distance < 10", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, timerFunction);

    xTaskCreatePinnedToCore(distanceDisplay, "displaying distance", 2048, &gpio, 5, NULL, 1);
}
