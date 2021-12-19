# BME280
Cross-platform library for BME280

## How to use
Include header file:
```c
#include "bme280.h"
```
Add initializing of the BME280
```c
int result = bme280_init();
  if (result != 0) {
    while(1) {
      HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
      HAL_Delay(200);
    }
  }
```

Implement the communication (HAL I2C example below)
```c
void bme280_write(uint16_t address, uint8_t *data_p, uint16_t length)
{
  HAL_I2C_Mem_Write(&hi2c1, 0x76 << 1, address, I2C_MEMADD_SIZE_8BIT, data_p, length, 100);
}
//------------------------------------------------
void bme280_read(uint16_t address, uint8_t *data_p, uint16_t length)
{
  HAL_I2C_Mem_Read(&hi2c1, 0x76 << 1, address, I2C_MEMADD_SIZE_8BIT, data_p, length, 100);
}
```

Read data from the BME280
```c
float temperature = bme280_get_temperature();
float humudity = bme280_get_humidity();
float pressure = bme280_get_pressure();
```
