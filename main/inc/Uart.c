#include "driver/uart.h"
#include "Uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "string.h"
#define UART_NUM UART_NUM_0
#define UART_TX_PIN GPIO_NUM_20
#define UART_RX_PIN GPIO_NUM_21
 #define BUF_SIZE 1280


static QueueHandle_t uart_queue;

//UART 读数据缓冲区
uint8_t uartBuf[BUF_SIZE];
int head = 0;
int tail  = 0;

void Uart_Flush(void)
{
    head = tail = 0;
}

int16_t Uart_Read(void)
{
    if(head!=tail){
        uint8_t Data = uartBuf[head];
        head =  (head+1)%BUF_SIZE;
        return Data;
    }else{
        return -1;
    }
}

void Uart_Init(uint32_t baud_rate)
{
    uart_config_t uart_config = {
        .baud_rate = baud_rate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart_queue, 0);

    //串口参数配置->uart0

}

void Uart_Send(uint8_t *buf , uint8_t len)
{
    uart_write_bytes(UART_NUM_0, (const char*)buf, len);
}

void Uart_Receive()
{
    uint8_t data[BUF_SIZE];
    while (1) {
        int len = uart_read_bytes(UART_NUM_0, data, BUF_SIZE, 20 / portTICK_RATE_MS);
        if (len > 0) {
            ESP_LOGI("UART", "Received: %.*s", len, data);
            for(int i=0; i<len; i++){
                uartBuf[tail] = data[i];
                tail = (tail+1)%BUF_SIZE;
            }
        }
    }
}
