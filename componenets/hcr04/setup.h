#pragma once

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DISTANCE";

namespace distance
{
    struct gpioINFO
    {
        gpio_num_t trig;
        gpio_num_t echo;
        gpio_num_t warning;
    };

    struct handle_t
    {
        TimerHandle_t timer1;
        QueueHandle_t q1;
    };

    void gpioINIT(gpioINFO *gpio);
    bool measure(gpioINFO *gpio);
    void displayAndWarning(gpioINFO *gpio);
}
