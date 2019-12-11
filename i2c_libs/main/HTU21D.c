#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#define HTU21D_ADDR 0x40
#define TRIGGER_TEMP_MEASURE_NOHOLD 0xF3

void htu_21D(void *params)
{
    while (true)
    {

        // send the command
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (HTU21D_ADDR << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, TRIGGER_TEMP_MEASURE_NOHOLD, true);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);

        // wait for the sensor (50ms)
        vTaskDelay(50 / portTICK_RATE_MS);

        // receive the answer
        uint8_t msb, lsb, crc;
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (HTU21D_ADDR << 1) | I2C_MASTER_READ, true);
        i2c_master_read_byte(cmd, &msb, 0x00);
        i2c_master_read_byte(cmd, &lsb, 0x00);
        i2c_master_read_byte(cmd, &crc, 0x01);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
        i2c_cmd_link_delete(cmd);

        uint16_t raw_value = ((uint16_t)msb << 8) | (uint16_t)lsb;
        float temperature = (raw_value * 175.72 / 65536.0) - 46.85;
        float humidity = (raw_value * 125.0 / 65536.0) - 6.0;

        ESP_LOGW("HTU21D", "temperature=%f humidity=%f", temperature, humidity);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}