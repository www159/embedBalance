#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "dht11.h"
#include "SG90.h"
int main()
{
    uint8_t t;
    uint8_t len;
    uint16_t times = 0;
    int32_t DHT_STA = 0;
    
    uint8_t data[5];

	
    Stm32_Clock_Init(9);
    delay_init(72);
    uart_init(72, 9600);
    LED_Init();
    SG90_init();
	
	LED1 = 0;
    delay_ms(9000);
    LED1 = 1;

    // KEY_Init();

    while(1) {
        if(USART_RX_STA&(1<<15)) {
            len = USART_RX_STA&0X3FFF;
						printf("\r\n您发送的消息为:\r\n");
            for(t=0; t < len; t++) {
                USART1->DR = USART_RX_BUF[t];
                while((USART1->SR&(1<<6)) == 0);
            }
            printf("\r\n\r\n");
            USART_RX_STA = 0;
        }
        else {
            times++;
            if(times%900 == 0) {
                DHT_STA = DHT11_read(data);
                if(DHT_STA == 0) {
                    printf("\r\n 温度:%d.%d  湿度:%d.%d", data[2], data[3], data[0], data[1]);
                }
                // SG90_reset();
                // printf("\r\nSG90复位\r\n");
                // delay_ms(600);
                // SG90_rotate(1);
                SG90_rotate(2); 
                delay_ms(600);
                SG90_rotate(1);
            }
            if(times%90 == 0) {
                LED0 = !LED0;
                // LED1 = !LED1;
            } 
            delay_ms(1);
        }
    }
}