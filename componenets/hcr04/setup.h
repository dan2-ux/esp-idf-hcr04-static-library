#pragma once

#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "DISTANCE";

namespace distance
{
    struct gpioINFO
    {
        gpio_num_t trig;
        gpio_num_t echo;
    };

    void gpioINIT(gpioINFO *gpio);
    float measure(gpioINFO *gpio);
}
