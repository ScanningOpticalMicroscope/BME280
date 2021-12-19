#ifndef _BME280_H_
#define _BME280_H_

#include <stdint.h>

#define BME280_MODE_SLEEP       0x00
#define BME280_MODE_FORCED      0x01
#define BME280_MODE_NORMAL      0x03

#define BME280_FILTER_OFF       0x00
#define BME280_FILTER_2         0x04
#define BME280_FILTER_4         0x08
#define BME280_FILTER_8         0x0C
#define BME280_FILTER_16        0x10

#define BME280_STBY_0_5         0x00
#define BME280_STBY_62_5        0x20
#define BME280_STBY_125         0x40
#define BME280_STBY_250         0x60
#define BME280_STBY_500         0x80
#define BME280_STBY_1000        0xA0
#define BME280_STBY_10          0xC0
#define BME280_STBY_20          0xE0

#define BME280_OSRS_T_SKIP      0x00
#define BME280_OSRS_T_x1        0x20
#define BME280_OSRS_T_x2        0x40
#define BME280_OSRS_T_x4        0x60
#define BME280_OSRS_T_x8        0x80
#define BME280_OSRS_T_x16       0xA0

#define BME280_OSRS_P_SKIP      0x00
#define BME280_OSRS_P_x1        0x04
#define BME280_OSRS_P_x2        0x08
#define BME280_OSRS_P_x4        0x0C
#define BME280_OSRS_P_x8        0x10
#define BME280_OSRS_P_x16       0x14

#define BME280_OSRS_H_SKIP      0x00
#define BME280_OSRS_H_x1        0x01
#define BME280_OSRS_H_x2        0x02
#define BME280_OSRS_H_x4        0x03
#define BME280_OSRS_H_x8        0x04
#define BME280_OSRS_H_x16       0x05

int   bme280_init(void);

float bme280_get_temperature(void);
float bme280_get_pressure(void);
float bme280_get_humidity(void);

void  bme280_set_mode(uint8_t mode);
void  bme280_set_filter(uint8_t filter);
void  bme280_set_standby(uint8_t standby_duration);

void  bme280_set_humidity_oversampling(uint8_t osrs);
void  bme280_set_pressure_oversampling(uint8_t osrs);
void  bme280_set_temperature_oversampling(uint8_t osrs);

#endif /* _BME280_H_ */
