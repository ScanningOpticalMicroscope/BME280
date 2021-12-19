#ifndef _BME280_H_
#define _BME280_H_
//------------------------------------------------
#include "stm32f1xx.h"
//------------------------------------------------
#define BME280_INIT_OK   (1)
#define BME280_INIT_FAIL (0)
//------------------------------------------------
int bme280_init_i2c(I2C_HandleTypeDef *hi2c, uint16_t address);
float bme280_get_temperature(void);
float bme280_get_pressure(void);
float bme280_get_humidity(void);
//------------------------------------------------

#endif /* _BME280_H_ */
