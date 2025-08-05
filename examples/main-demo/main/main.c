#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "xl95x5.h"

#define XL9535_ADDR XL95X5_I2C_ADDRESS_BASE // Example address, adjust as needed

static const char *TAG = "xl95x5_example";
void io_task(void *param)
{
    i2c_dev_t xl9535;
    memset(&xl9535, 0, sizeof(xl9535));

    ESP_ERROR_CHECK(xl95x5_init_desc(&xl9535, XL9535_ADDR, 0, CONFIG_I2CDEV_DEFAULT_SDA_PIN, CONFIG_I2CDEV_DEFAULT_SCL_PIN));

    ESP_ERROR_CHECK(xl95x5_set_full_gpio_mode(&xl9535, 0xFF00)); // Configure GPIO0...GPIO7 as outputs, GPIO8...GPIO15 as inputs

    uint16_t levels = 0x00AA;
    uint8_t old_input_levels = 0x00;

    while (1)
    {
        // Set GPIO levels
        ESP_ERROR_CHECK(xl95x5_set_full_gpio_level(&xl9535, levels));
        ESP_LOGI(TAG, "Set GPIO levels: 0x%04X", levels);

        // Read back GPIO levels
        uint16_t read_levels;
        ESP_ERROR_CHECK(xl95x5_get_full_gpio_level(&xl9535, &read_levels));
        
        if(old_input_levels != ((read_levels >> 8) & 0xFF))
        {
            ESP_LOGI(TAG, "Input GPIO levels change: 0x%02X", (read_levels >> 8) & 0xFF);
            old_input_levels = (read_levels >> 8) & 0xFF; // Store only input levels
        }

        // Toggle levels for next iteration
        levels = ~levels & 0xFF; // Toggle outputs

        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

void app_main(void)
{
    ESP_ERROR_CHECK(i2cdev_init());

    xTaskCreate(io_task, "io_task", configMINIMAL_STACK_SIZE * 5, NULL, tskIDLE_PRIORITY + 5, NULL);
}