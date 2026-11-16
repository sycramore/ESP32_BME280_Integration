/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

/* i2c - Simple Example

   Simple I2C example that shows how to initialize I2C
   and read registers from a sensor connected over I2C.

   The sensor used in this example is a BME280 environmental sensor.
*/

#include <stddef.h>
#include <stdint.h>

#include "esp_log.h"
#include "driver/i2c_master.h"

static const char *TAG = "BME280";

#define I2C_MASTER_SCL_IO       7
#define I2C_MASTER_SDA_IO       6
#define I2C_MASTER_NUM          I2C_NUM_0
#define I2C_MASTER_FREQ_HZ      100000
#define I2C_MASTER_TIMEOUT_MS   1000

#define BME280_SENSOR_ADDR         0x76
#define BME280_WHO_AM_I_REG_ADDR   0xD0
#define BME280_EXPECTED_CHIP_ID    0x60

/**
 * @brief Read a sequence of bytes from BME280 registers.
 */
static esp_err_t bme280_register_read(
    i2c_master_dev_handle_t dev_handle,
    uint8_t reg_addr,
    uint8_t *data,
    size_t len)
{
    return i2c_master_transmit_receive(
        dev_handle,
        &reg_addr,
        1,
        data,
        len,
        I2C_MASTER_TIMEOUT_MS
    );
}

/**
 * @brief Initialize the I2C master bus and BME280 device.
 */
static void i2c_master_init(
    i2c_master_bus_handle_t *bus_handle,
    i2c_master_dev_handle_t *dev_handle)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(
        &bus_config,
        bus_handle
    ));

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = BME280_SENSOR_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(
        *bus_handle,
        &dev_config,
        dev_handle
    ));
}

void app_main(void)
{
    uint8_t chip_id;

    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;

    i2c_master_init(&bus_handle, &dev_handle);
    ESP_LOGI(TAG, "I2C initialized successfully");

    ESP_ERROR_CHECK(bme280_register_read(
        dev_handle,
        BME280_WHO_AM_I_REG_ADDR,
        &chip_id,
        1
    ));

    ESP_LOGI(TAG, "WHO_AM_I = 0x%02X", chip_id);

    if (chip_id == BME280_EXPECTED_CHIP_ID) {
        ESP_LOGI(TAG, "BME280 detected");
    } else {
        ESP_LOGE(TAG, "Unexpected chip ID: 0x%02X", chip_id);
    }

    ESP_ERROR_CHECK(i2c_master_bus_rm_device(dev_handle));
    ESP_ERROR_CHECK(i2c_del_master_bus(bus_handle));

    ESP_LOGI(TAG, "I2C de-initialized successfully");
}
