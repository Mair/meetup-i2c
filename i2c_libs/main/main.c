#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#define SDA_GPIO 16
#define SCL_GPIO 17

#define I2C_ADDRESS 0x68 // I2C address of MPU6050

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_PWR_MGMT_1 0x6B

void mpu_6050(void *params);
void htu_21D(void *params);
void show_screen(void *params);

void app_main(void *ignore)
{
    xTaskCreate(show_screen, "show_screen", 1024 * 9, NULL, 2, NULL);
    vTaskDelay(pdMS_TO_TICKS(1000));
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_GPIO;
    conf.scl_io_num = SCL_GPIO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 100000;
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    xTaskCreate(mpu_6050, "mpu_6050", 1024 * 9, NULL, 2, NULL);
    xTaskCreate(htu_21D, "htu_21D", 1024 * 9, NULL, 2, NULL);
   
  
}