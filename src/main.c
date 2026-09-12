#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"

#define LED_GPIO   48   // onboard RGB LED data pin
#define LED_COUNT  1    // just the one onboard LED

static const char *TAG = "rainbow_blink";

void app_main(void)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = LED_COUNT,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB, // v3.x field name
        .led_model = LED_MODEL_WS2812,
        .flags.invert_out = false,
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000, // 10 MHz tick
        .flags.with_dma = false,
    };

    led_strip_handle_t led_strip;
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    ESP_LOGI(TAG, "LED strip initialized, starting rainbow blink");

    uint8_t colors[][3] = {
        {255, 0,   0},   // red
        {255, 127, 0},   // orange
        {255, 255, 0},   // yellow
        {0,   255, 0},   // green
        {0,   0,   255}, // blue
        {75,  0,   130}, // indigo
        {148, 0,   211}, // violet
    };
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    while (1) {
        for (int i = 0; i < num_colors; i++) {
            led_strip_set_pixel(led_strip, 0, colors[i][0], colors[i][1], colors[i][2]);
            led_strip_refresh(led_strip);
            vTaskDelay(pdMS_TO_TICKS(400));

            led_strip_clear(led_strip);
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}
