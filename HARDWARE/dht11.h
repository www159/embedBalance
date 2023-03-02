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

/**
 * @brief   读取dht11温度，湿度
 * @param data  数据缓冲区 格式为：[湿度整数，湿度小数，温度整数，温度小数，校验和]
 * @return  0   成功
 *      1   校验失败
 *      2   启动失败
*/
int32_t DHT11_read(uint8_t *data);

#endif