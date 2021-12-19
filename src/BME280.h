#ifndef _BME280_H_
#define _BME280_H_

int   bme280_init(void);
float bme280_get_temperature(void);
float bme280_get_pressure(void);
float bme280_get_humidity(void);

#endif /* _BME280_H_ */
