#include "hcr04.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace distance
{
    handle_t myHandle_t;
    void warningTask(TimerHandle_t xTimer)
    {
        gpioINFO *gpio = (gpioINFO *)pvTimerGetTimerID(xTimer);
        gpio_set_level(gpio->warning, 0);
    }

    void gpioINIT(gpioINFO *gpio)
    {
        if (gpio == nullptr)
        {
            return;
        }
        gpio_reset_pin(gpio->trig);
        gpio_reset_pin(gpio->echo);
        gpio_reset_pin(gpio->warning);

        gpio_set_direction(gpio->echo, GPIO_MODE_INPUT);
        gpio_set_direction(gpio->trig, GPIO_MODE_OUTPUT);
        gpio_set_direction(gpio->warning, GPIO_MODE_OUTPUT);
        distance::myHandle_t.q1 = xQueueCreate(5, sizeof(float));
        distance::myHandle_t.timer1 = xTimerCreate("timer for warning", pdMS_TO_TICKS(500), pdTRUE, (void *)gpio, warningTask);
    }

    bool measure(gpioINFO *gpio)
    {
        if (gpio == nullptr)
        {
            ESP_LOGE(TAG, "Pointer holding gpioINFO is error");
            return -1;
        }
        gpio_set_level(gpio->trig, 1);
        esp_rom_delay_us(10);
        gpio_set_level(gpio->trig, 0);
        esp_rom_delay_us(10);
        gpio_set_level(gpio->trig, 1);

        int64_t gpioStart = esp_timer_get_time();
        while (gpio_get_level(gpio->echo) == 0)
        {
            if (esp_timer_get_time() - gpioStart >= 30000)
            {
                ESP_LOGE(TAG, "Durantion reach at echo high");
                return -1;
            }
        }

        int64_t echoStart = esp_timer_get_time();
        while (gpio_get_level(gpio->echo) == 1)
        {
            if (esp_timer_get_time() - echoStart >= 30000)
            {
                ESP_LOGE(TAG, "Duration reach at echo low");
                return -1;
            }
        }

        int64_t echoEnd = esp_timer_get_time();
        float res = ((echoEnd - echoStart) * 0.0343) / 2;

        if (xQueueSend(distance::myHandle_t.q1, &res, 0))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void displayAndWarning(gpioINFO *gpio)
    {
        float res = 0;
        char *text = (char *)malloc(30);
        if (xQueueReceive(distance::myHandle_t.q1, &res, 0) == pdTRUE)
        {
            if (res < 10)
            {
                gpio_set_level(gpio->warning, 1);
                if (xTimerIsTimerActive(distance::myHandle_t.timer1) == pdTRUE)
                {
                    xTimerReset(distance::myHandle_t.timer1, 0);
                }
                xTimerStart(distance::myHandle_t.timer1, 0);
            }
            snprintf(text, 30, "Distance: %.2f cm", res);
            ESP_LOGI(TAG, "%s", text);
        }
        free(text);
    }
}
