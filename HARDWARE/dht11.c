#include "sys.h"
#include "delay.h"
#include "dht11.h"

static void set_out();

static void set_in();

static uint8_t DHT11_start();

static uint8_t DHT11_read_byte();

int32_t DHT11_read(uint8_t *buffer)
{
    uint8_t i;
    uint32_t t;


    if(DHT11_start() != 0) {
        // printf("\r\nDHT11启动失败\r\n");
        return 2;
    }

    for(i = 0; i < 5; i++) {
        buffer[i] = DHT11_read_byte();
    }

    if(buffer[4] != buffer[0] + buffer[1] + buffer[2] + buffer[3]) return 1;
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

static uint8_t DHT11_start() 
{

    uint32_t t;

		// @debug
	// printf("\r\n初始化DHT11模块\r\n");
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
    
    // DHT回应信号
    if(DHT_PORT_IN == 0) {
        // 1. 80us低电平
        t = 0;
        while(DHT_PORT_IN == 0) {
            t++;
            delay_us(2);
            if(t >= 80) {
                printf("\r\n低电平回应错误\r\n");
                return 1;
            }
        }
        // 2. 80us高电平
        t = 0;
        while(DHT_PORT_IN == 1) {
            t++;
            delay_us(2);
            if(t >= 80) return 1;
        }
        return 0;
    }
    return 1;
}

static uint8_t DHT11_read_byte() {
    uint8_t ret = 0;
    uint8_t i, t;
    
    for(i = 0; i < 8; i++) {
        ret <<= 1;
        t = 0;
        while(DHT_PORT_IN == 0) {
            t++;
            delay_us(2);
            if(t >= 50) return 1;
        }

        // 2. 26-28us高电平代表0
        //    70us高电平代表1
        delay_us(28);
        if(DHT_PORT_IN == 1) {
            ret |= 0X01;
        }
        else {
            ret &= ~0X01;
        }
        while(DHT_PORT_IN == 1);
    }
    return ret;
}