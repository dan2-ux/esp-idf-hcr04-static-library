#include "setup.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"

namespace distance
{
    void gpioINIT(gpioINFO *gpio)
    {
        if (gpio == nullptr)
        {
            return;
        }
        gpio_reset_pin(gpio->trig);
        gpio_reset_pin(gpio->echo);

        gpio_set_direction(gpio->echo, GPIO_MODE_INPUT);
        gpio_set_direction(gpio->trig, GPIO_MODE_OUTPUT);
    }

    float measure(gpioINFO *gpio)
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
        return ((echoEnd - echoStart) * 0.0343) / 2;
    }
}
