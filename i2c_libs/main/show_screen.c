#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <driver/i2c.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <u8g2.h>
#include "u8g2_esp32_hal.h"

#define PIN_SDA 16
#define PIN_SCL 17

void show_screen(void *params)
{
	u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
	u8g2_esp32_hal.sda = PIN_SDA;
	u8g2_esp32_hal.scl = PIN_SCL;
	u8g2_esp32_hal_init(u8g2_esp32_hal);

	u8g2_t u8g2; // a structure which will contain all the data for one display
	u8g2_Setup_ssd1306_i2c_128x32_univision_f(
		&u8g2,
		U8G2_R0,
		u8g2_esp32_i2c_byte_cb,
		u8g2_esp32_gpio_and_delay_cb); // init u8g2 structure
	u8x8_SetI2CAddress(&u8g2.u8x8, 0x78);

	u8g2_InitDisplay(&u8g2);	 // send init sequence to the display, display is in sleep mode after this,
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
	u8g2_DrawStr(&u8g2, 2, 13, "Hello world!");
	ESP_LOGI("SCREEN", "u8g2_SendBuffer");
	u8g2_SendBuffer(&u8g2);
	vTaskDelete(NULL);
}