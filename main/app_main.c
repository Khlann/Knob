/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "SCServo.h"
#include "SCSCL.h"
#include "Uart.h"

#include "app_audio.h"
#include "settings.h"
#include "lv_example_pub.h"
#include "bsp/esp-bsp.h"
#include "driver/uart.h"

static const char *TAG = "main";


#define MEMORY_MONITOR 0

#if MEMORY_MONITOR

#endif


esp_err_t bsp_board_init(void)
{
    ESP_ERROR_CHECK(bsp_led_init());
    ESP_ERROR_CHECK(bsp_spiffs_mount());
    return ESP_OK;
}

// #define UART_NUM UART_NUM_0
// #define UART_TX_PIN GPIO_NUM_20
// #define UART_RX_PIN GPIO_NUM_21
// #include "driver/uart.h"
// void Uart_SendBytes(const uint8_t* data, int len) {
//     uart_write_bytes(UART_NUM, (const char*)data, len);
// }
#define RX0_BUF_SIZE (1024)
#define TX0_BUF_SIZE (512)
#define TXD0_PIN (GPIO_NUM_21)
#define RXD0_PIN (GPIO_NUM_20)
void uart_init(void)
{
	//串口配置结构体


//串口参数配置->uart0
	uart_config_t uart0_config = {
		.baud_rate = 1000000,				   //波特率
		.data_bits = UART_DATA_8_BITS,		   //数据位
		.parity = UART_PARITY_DISABLE,		   //校验位
		.stop_bits = UART_STOP_BITS_1,		   //停止位
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE, //硬件流控
	};
	uart_param_config(UART_NUM_0, &uart0_config); //设置串口
	// IO映射-> T:IO1  R:IO3
	uart_set_pin(UART_NUM_0, TXD0_PIN, RXD0_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	//注册串口服务即使能+设置缓存区大小
	uart_driver_install(UART_NUM_0, RX0_BUF_SIZE * 2, TX0_BUF_SIZE * 2, 0, NULL, 0);
}
void app_main(void)
{
    ESP_LOGI(TAG, "Compile time: %s %s", __DATE__, __TIME__);
    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    // ESP_ERROR_CHECK(settings_read_parameter_from_nvs());

    //     uart_config_t uart_config = {
    //     .baud_rate = 115200,
    //     .data_bits = UART_DATA_8_BITS,
    //     .parity = UART_PARITY_DISABLE,
    //     .stop_bits = UART_STOP_BITS_1,
    //     .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    // };
    //     esp_err_t ret = uart_param_config(UART_NUM, &uart_config);
    // if (ret != ESP_OK) {
    //     ESP_LOGE("UART", "uart_param_config failed: %d", ret);
    //     return;
    // }
    //     ret = uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // if (ret != ESP_OK) {
    //     ESP_LOGE("UART", "uart_set_pin failed: %d", ret);
    //     return;
    // }

    // ret = uart_driver_install(UART_NUM, 1024 * 2, 0, 0, NULL, 0);
    // if (ret != ESP_OK) {
    //     ESP_LOGE("UART", "uart_driver_install failed: %d", ret);
    //     return;
    // }
    //Uart_Init(1000000);
    uart_init();
    const uint8_t data[] = "hello";
    while(1)
    {
        WritePos(1, 1000, 0, 1500);

        	//uart_write_bytes(UART_NUM_0, "uart0 test OK \r\n", strlen("uart0 test OK \r\n"));
            vTaskDelay(pdMS_TO_TICKS(754));
    }

    // while(1)
    // {
    // const uint8_t data[] = "hello";
    // 	//uart_write_bytes(UART_NUM_0, "uart0 test OK \r\n", strlen("uart0 test OK \r\n"));
    // //Uart_Send(data, sizeof(data));
    //     //WritePos(1, 1000, 0, 1500);//舵机(ID1),以最高速度V=1500步/秒,运行至P1=1000
    //     //delay(754);//[(P1-P0)/V]*1000+100
    // }
    bsp_display_start();

    ESP_LOGI(TAG, "Display LVGL demo");
    ui_obj_to_encoder_init();
    lv_create_home(&boot_Layer);
    lv_create_clock(&clock_screen_layer, TIME_ENTER_CLOCK_2MIN);
    bsp_display_unlock();

    vTaskDelay(pdMS_TO_TICKS(500));
    bsp_display_backlight_on();

    bsp_board_init();
    audio_play_start();

#if MEMORY_MONITOR
    sys_monitor_start();
#endif
}
