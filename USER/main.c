#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "dht11.h"

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
	
	LED1 = 0;
    delay_ms(9000);
    LED1 = 1;

    DHT_STA = DHT11_Start();
    printf("\r\nDHT11传感器的状态为:%d\r\n", DHT_STA);
    // KEY_Init();

    while(1) {
        if(USART_RX_STA&(1<<15)) {
            len = USART_RX_STA&0X3FFF;
						printf("\r\n您发送的消息为:\r\n");
						printf("\r\n");
            for(t=0; t < len; t++) {
                USART1->DR = USART_RX_BUF[t];
                while((USART1->SR&(1<<6)) == 0);
            }
            printf("\r\n\r\n");
            USART_RX_STA = 0;
        }
        else {
            times++;
            if(times%5000 == 0) {
                DHT_STA = DHT11_Start();
                if(DHT_STA != 0) {
                    printf("\r\nDHT11传感器的状态为:%d\r\n", DHT_STA);
                    continue;
                }
                DHT_STA = DHT11_Read(data);
                if(DHT_STA == 0) {
                    printf("\r\n 温度:%d.%d  湿度:%d.%d", data[2], data[3], data[0], data[1]);
                    printf("\r\n实际和:%d   校验和:%d\r\n", (uint8_t)(data[0]+data[1]+data[2]+data[3]), data[4]);
                }
            }
            if(times%90 == 0) {
                LED0 = !LED0;
                // LED1 = !LED1;
            } 
            delay_ms(1);
        }
    }
}