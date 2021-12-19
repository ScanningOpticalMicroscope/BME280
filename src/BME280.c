#include "BME280.h"
//------------------------------------------------
#include <math.h>
//------------------------------------------------
#define BME280_be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
#define BME280_be24toword(a) ((((a)>>16)&0x000000ff)|((a)&0x0000ff00)|(((a)<<16)&0x00ff0000))
//------------------------------------------------
#define BME280_REG_ID           0xD0
#define BME280_ID               0x60
#define BME280_REG_SOFTRESET    0xE0
#define BME280_SOFTRESET_VALUE  0xB6
#define BME280_REG_CTRL_HUM     0xF2
#define BME280_REGISTER_STATUS  0xF3
#define BME280_REG_CTRL_MEAS    0xF4
#define BME280_STATUS_MEASURING 0x08
#define BME280_STATUS_IM_UPDATE 0x01
#define BME280_REG_CONFIG       0xF5
#define BME280_REGISTER_PRESS   0xF7
#define BME280_REGISTER_TEMP    0xFA
#define BME280_REGISTER_HUM     0xFD
//------------------------------------------------
#define BME280_REGISTER_DIG_T1  0x88
#define BME280_REGISTER_DIG_T2  0x8A
#define BME280_REGISTER_DIG_T3  0x8C
#define BME280_REGISTER_DIG_P1  0x8E
#define BME280_REGISTER_DIG_P2  0x90
#define BME280_REGISTER_DIG_P3  0x92
#define BME280_REGISTER_DIG_P4  0x94
#define BME280_REGISTER_DIG_P5  0x96
#define BME280_REGISTER_DIG_P6  0x98
#define BME280_REGISTER_DIG_P7  0x9A
#define BME280_REGISTER_DIG_P8  0x9C
#define BME280_REGISTER_DIG_P9  0x9E
#define BME280_REGISTER_DIG_H1  0xA1
#define BME280_REGISTER_DIG_H2  0xE1
#define BME280_REGISTER_DIG_H3  0xE3
#define BME280_REGISTER_DIG_H4  0xE4
#define BME280_REGISTER_DIG_H5  0xE5
#define BME280_REGISTER_DIG_H6  0xE7
//------------------------------------------------
typedef struct
{
  uint16_t  dig_T1;
  int16_t   dig_T2;
  int16_t   dig_T3;

  uint16_t  dig_P1;
  int16_t   dig_P2;
  int16_t   dig_P3;
  int16_t   dig_P4;
  int16_t   dig_P5;
  int16_t   dig_P6;
  int16_t   dig_P7;
  int16_t   dig_P8;
  int16_t   dig_P9;

  uint8_t   dig_H1;
  int16_t   dig_H2;
  uint8_t   dig_H3;
  int16_t   dig_H4;
  int16_t   dig_H5;
  int8_t    dig_H6;

} bme280_calibrate_t;

typedef struct {
  bme280_calibrate_t calibration_data;
  int32_t global_temperature;
} bme280_t;
//------------------------------------------------
static bme280_t bme280;
//------------------------------------------------
__weak void bme280_write(uint16_t address, uint8_t *data_p, uint16_t length)
{

}
//------------------------------------------------
__weak void bme280_read(uint16_t address, uint8_t *data_p, uint16_t length)
{

}
//------------------------------------------------
static void bme280_read_status(uint8_t *status_p)
{
  bme280_read(BME280_REGISTER_STATUS, status_p, 1);
  *status_p = *status_p & 0x09;
}
//------------------------------------------------
static void bme280_read_coefficients(void)
{
  uint8_t temp1 = 0, temp2 = 0;

  bme280_read(BME280_REGISTER_DIG_T1, (uint8_t*)&bme280.calibration_data.dig_T1, 2);
  bme280_read(BME280_REGISTER_DIG_T2, (uint8_t*)&bme280.calibration_data.dig_T2, 2);
  bme280_read(BME280_REGISTER_DIG_T3, (uint8_t*)&bme280.calibration_data.dig_T3, 2);

  bme280_read(BME280_REGISTER_DIG_P1, (uint8_t*)&bme280.calibration_data.dig_P1, 2);
  bme280_read(BME280_REGISTER_DIG_P2, (uint8_t*)&bme280.calibration_data.dig_P2, 2);
  bme280_read(BME280_REGISTER_DIG_P3, (uint8_t*)&bme280.calibration_data.dig_P3, 2);
  bme280_read(BME280_REGISTER_DIG_P4, (uint8_t*)&bme280.calibration_data.dig_P4, 2);
  bme280_read(BME280_REGISTER_DIG_P5, (uint8_t*)&bme280.calibration_data.dig_P5, 2);
  bme280_read(BME280_REGISTER_DIG_P6, (uint8_t*)&bme280.calibration_data.dig_P6, 2);
  bme280_read(BME280_REGISTER_DIG_P7, (uint8_t*)&bme280.calibration_data.dig_P7, 2);
  bme280_read(BME280_REGISTER_DIG_P8, (uint8_t*)&bme280.calibration_data.dig_P8, 2);
  bme280_read(BME280_REGISTER_DIG_P9, (uint8_t*)&bme280.calibration_data.dig_P9, 2);

  bme280_read(BME280_REGISTER_DIG_H1, &bme280.calibration_data.dig_H1, 1);
  bme280_read(BME280_REGISTER_DIG_H2, (uint8_t*)&bme280.calibration_data.dig_H2, 2);
  bme280_read(BME280_REGISTER_DIG_H3, &bme280.calibration_data.dig_H3, 1);

  bme280_read(BME280_REGISTER_DIG_H4 + 0, &temp1, 1);
  bme280_read(BME280_REGISTER_DIG_H4 + 1, &temp2, 1);
  bme280.calibration_data.dig_H4 = (temp1 << 4) | (temp2 & 0xF);

  bme280_read(BME280_REGISTER_DIG_H5 + 1, &temp1, 1);
  bme280_read(BME280_REGISTER_DIG_H5 + 0, &temp2, 1);
  bme280.calibration_data.dig_H5 = (temp1 << 4) | (temp2 >> 4);

  bme280_read(BME280_REGISTER_DIG_H6, &temp1, 1);
  bme280.calibration_data.dig_H6 = (int8_t)temp1;
}
//------------------------------------------------
void bme280_set_standby(uint8_t standby_duration) {
  uint8_t reg;
  bme280_read(BME280_REG_CONFIG, &reg, 1);
  reg = reg & ~BME280_STBY_MSK;
  reg |= standby_duration & BME280_STBY_MSK;
  bme280_write(BME280_REG_CONFIG, &reg, 1);
}
//------------------------------------------------
void bme280_set_filter(uint8_t filter) {
  uint8_t reg;
  bme280_read(BME280_REG_CONFIG, &reg, 1);
  reg = reg & ~BME280_FILTER_MSK;
  reg |= filter & BME280_FILTER_MSK;
  bme280_write(BME280_REG_CONFIG, &reg, 1);
}
//------------------------------------------------
void bme280_set_temperature_oversampling(uint8_t osrs)
{
  uint8_t reg;
  bme280_read(BME280_REG_CTRL_MEAS, &reg, 1);
  reg = reg & ~BME280_OSRS_T_MSK;
  reg |= osrs & BME280_OSRS_T_MSK;
  bme280_write(BME280_REG_CTRL_MEAS, &reg, 1);
}
//------------------------------------------------
void bme280_set_pressure_oversampling(uint8_t osrs)
{
  uint8_t reg;
  bme280_read(BME280_REG_CTRL_MEAS, &reg, 1);
  reg = reg & ~BME280_OSRS_P_MSK;
  reg |= osrs & BME280_OSRS_P_MSK;
  bme280_write(BME280_REG_CTRL_MEAS, &reg, 1);
}
//------------------------------------------------
void bme280_set_humidity_oversampling(uint8_t osrs)
{
  uint8_t reg;
  bme280_read(BME280_REG_CTRL_HUM, &reg, 1);
  reg = reg & ~BME280_OSRS_H_MSK;
  reg |= osrs & BME280_OSRS_H_MSK;
  bme280_write(BME280_REG_CTRL_HUM, &reg, 1);
  bme280_read(BME280_REG_CTRL_MEAS, &reg, 1);
  bme280_write(BME280_REG_CTRL_MEAS, &reg, 1);
}
//------------------------------------------------
void bme280_set_mode(uint8_t mode) {
  uint8_t reg;
  bme280_read(BME280_REG_CTRL_MEAS, &reg, 1);
  reg = reg & ~BME280_MODE_MSK;
  reg |= mode & BME280_MODE_MSK;
  bme280_write(BME280_REG_CTRL_MEAS, &reg, 1);
}
//------------------------------------------------
float bme280_get_temperature(void)
{
  float temper_float = 0.0f;
  uint32_t temper_raw;
  int32_t val1, val2;

  bme280_read(BME280_REGISTER_TEMP, (uint8_t*)&temper_raw, 3);
  temper_raw = BME280_be24toword(temper_raw) & 0x00FFFFFF;

  temper_raw >>= 4;
  val1 = ((((temper_raw>>3) - ((int32_t)bme280.calibration_data.dig_T1 <<1))) *
          ((int32_t)bme280.calibration_data.dig_T2)) >> 11;
  val2 = (((((temper_raw>>4) - ((int32_t)bme280.calibration_data.dig_T1)) *
            ((temper_raw>>4) - ((int32_t)bme280.calibration_data.dig_T1))) >> 12) *
          ((int32_t)bme280.calibration_data.dig_T3)) >> 14;

  bme280.global_temperature = val1 + val2;
  temper_float = ((bme280.global_temperature * 5 + 128) >> 8);
  temper_float /= 100;
  return temper_float;
}
//------------------------------------------------
float bme280_get_pressure(void)
{
  float press_float = 0.0f;
  uint32_t press_raw, pres_int;
  int64_t val1, val2, p;
  bme280_get_temperature();

  bme280_read(BME280_REGISTER_PRESS, (uint8_t*)&press_raw, 3);
  press_raw = BME280_be24toword(press_raw) & 0x00FFFFFF;

  press_raw >>= 4;
  val1 = ((int64_t) bme280.global_temperature) - 128000;
  val2 = val1 * val1 * (int64_t)bme280.calibration_data.dig_P6;
  val2 = val2 + ((val1 * (int64_t)bme280.calibration_data.dig_P5) << 17);
  val2 = val2 + ((int64_t)bme280.calibration_data.dig_P4 << 35);
  val1 = ((val1 * val1 * (int64_t)bme280.calibration_data.dig_P3) >> 8) + ((val1 * (int64_t)bme280.calibration_data.dig_P2) << 12);
  val1 = (((((int64_t)1) << 47) + val1)) * ((int64_t)bme280.calibration_data.dig_P1) >> 33;
  if (val1 == 0) {
    return 0;
  }
  p = 1048576 - press_raw;
  p = (((p << 31) - val2) * 3125) / val1;
  val1 = (((int64_t)bme280.calibration_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
  val2 = (((int64_t)bme280.calibration_data.dig_P8) * p) >> 19;
  p = ((p + val1 + val2) >> 8) + ((int64_t)bme280.calibration_data.dig_P7 << 4);
  pres_int = ((p >> 8) * 1000) + (((p & 0xff) * 390625) / 100000);
  press_float = pres_int / 100.0f;
  return press_float;
}
//------------------------------------------------
float bme280_get_humidity(void)
{
  float hum_float = 0.0f;
  int16_t hum_raw = 0;
  int32_t hum_raw_sign, v_x1_u32r;
  bme280_get_temperature();

  bme280_read(BME280_REGISTER_HUM, (uint8_t*)&hum_raw, 2);
  hum_raw = BME280_be16toword(hum_raw);

  hum_raw_sign = ((int32_t)hum_raw)&0x0000FFFF;
  v_x1_u32r = (bme280.global_temperature - ((int32_t)76800));
  v_x1_u32r = (((((hum_raw_sign << 14) - (((int32_t)bme280.calibration_data.dig_H4) << 20) -
                  (((int32_t)bme280.calibration_data.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
               (((((((v_x1_u32r * ((int32_t)bme280.calibration_data.dig_H6)) >> 10) *
                    (((v_x1_u32r * ((int32_t)bme280.calibration_data.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                  ((int32_t)2097152)) * ((int32_t)bme280.calibration_data.dig_H2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                             ((int32_t)bme280.calibration_data.dig_H1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  hum_float = (v_x1_u32r>>12);
  hum_float /= 1024.0f;
  return hum_float;
}
//------------------------------------------------
int bme280_init(void)
{
  uint8_t value = 0;
  uint32_t value32 = 0;

  bme280_read(BME280_REG_ID, &value, 1);
  if(value != BME280_ID) {
    return -1;
  }

  uint8_t data = BME280_SOFTRESET_VALUE;
  bme280_write(BME280_REG_SOFTRESET, &data, 1);

  uint8_t status = 0;
  do {
    bme280_read_status(&status);
  } while (status & BME280_STATUS_IM_UPDATE);

  bme280_read_coefficients();
  bme280_set_standby(BME280_STBY_1000);
  bme280_set_filter(BME280_FILTER_4);
  bme280_set_temperature_oversampling(BME280_OSRS_T_x4);
  bme280_set_pressure_oversampling(BME280_OSRS_P_x2);
  bme280_set_humidity_oversampling(BME280_OSRS_H_x1);

  bme280_read(BME280_REG_CTRL_MEAS, (uint8_t*)&value32, 1);

  uint8_t temp = 0;
  bme280_read(BME280_REG_CTRL_HUM, &temp, 1);
  value32 |= temp << 8;

  bme280_set_mode(BME280_MODE_NORMAL);

  return 0;
}
