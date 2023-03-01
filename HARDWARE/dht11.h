#ifndef _DHT11_H_
#define _DHT11_H_

// GPIO抽象
#define DHT_PORT GPIOC
#define DHT_PORT_APBENR APB2ENR
#define DHT_PORT_APBENR_OFST 4
#define DHT_PORT_IN (PCin(4))
#define DHT_PORT_OUT    (PCout(4))
#define DHT_PORT_OFST   CRL
#define DHT_PORT_MASK   0XFFF0FFFF
#define DHT_PORT_CNFMODE_IN 0X00080000
#define DHT_PORT_CNFMODE_OUT    0X00030000
#define DHT_PORT_ODR    1<<4    //上拉输入
// #define DHT_PORT_ODR

// DHT11 相关
//extern int32_t DHT11_STA;uin

// uint8_t DHT11_Read_Byte();

// [湿度整数，湿度小数，温度整数，温度小数，校验和]
int32_t DHT11_read(uint8_t *data);

#endif