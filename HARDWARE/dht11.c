#include "sys.h"
#include "delay.h"
#include "dht11.h"

static void set_out();

static void set_in();

int32_t DHT11_Start() 
{

    uint32_t t;

		// @debug
		// printf("初始化DHT11模块\r\n");
    RCC->DHT_PORT_APBENR |= 1 << DHT_PORT_APBENR_OFST;
    
    // 设置IO输出
    set_out();

    // DHT启动请求
    // 1. 设置初始高电平
    DHT_PORT_OUT = 1;
    delay_us(50);
    // 2. 至少18ms低电平
    DHT_PORT_OUT = 0;
    delay_ms(20);
    // 3. 20-40us高电平
    DHT_PORT_OUT = 1;
    delay_us(30);

    // 设置IO输入
    set_in();

    while(DHT_PORT_IN == 0);

    // DHT启动应答
    // 1. 40-50us低电平
    t = 0;
    while(DHT_PORT_IN  == 1) {
        t++;
        delay_us(1);
        if(t >= 100) { // 200us未接受到低电平
            // @debug
            // printf("\r\n第一次应答失败\r\n");
            return -1;
        }
    }

    delay_us(40);
    // 2. 40-50us高电平
    t = 0;
    while(DHT_PORT_IN == 0) {
        t++;
        delay_us(1);
        if(t >= 80) { // 160us未接受到高电平
            return -1;
        }
    }

    return 0;
}

int32_t DHT11_Read(uint8_t *data)
{
    uint8_t i;
    uint32_t t;

    for(i = 0; i < 40; i++) {
        t = 0;

        while(DHT_PORT_IN == 0) {
            t++;
            delay_us(1);
            if(t >= 50) return -1;
        }

        delay_us(40);
        if(DHT_PORT_IN == 1) {
            data[i/8] |= 1<<(7-i%8);
            t = 0;
            while(DHT_PORT_IN == 1) {
                t++;
                delay_us(1);
                if(t >= 50) {
                    return -1;
                }
            }
        }
        else {
            data[i/8] &= ~(0x01<<(7-i));
        }
    }
    if(data[4] != data[0] + data[1] + data[2] + data[3]) return 0;

    return 0;
}
static void set_out() 
{
    // @debug
    // printf("\r\n设置输出为推挽输出\r\n");
    //推挽输出
    DHT_PORT->DHT_PORT_OFST &= DHT_PORT_MASK;
    DHT_PORT->DHT_PORT_OFST |= DHT_PORT_CNFMODE_OUT;
    DHT_PORT->ODR = DHT_PORT_ODR;
    // printf("\r\n设置输出成功\r\n");
}

static void set_in()
{
    //上拉输入
    // printf("\r\n设置输入为上拉输入\r\n");
    DHT_PORT->DHT_PORT_OFST &= DHT_PORT_MASK;
    DHT_PORT->DHT_PORT_OFST |= DHT_PORT_CNFMODE_IN;
    DHT_PORT->ODR = DHT_PORT_ODR;
    // printf("\r\n设置输入成功\r\n");
}