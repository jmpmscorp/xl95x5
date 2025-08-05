/*
 *
 * ESP-IDF driver for XL9535/XL9555 16-bit I/O expanders for I2C-bus
 *
 * Copyright (c) 2025 Jose Manuel Perez <unclerus@gmail.com>
 *
 * ISC Licensed as described in the file LICENSE
 */

#ifndef __XL95X5_H__
#define __XL95X5_H__

#include <stdint.h>
#include <esp_err.h>
#include <i2cdev.h>

// #include "local.h" // local header files at the end.

#ifdef __cplusplus
extern "C"
{
#endif

#define XL95X5_I2C_ADDRESS_BASE (0x20) /*!< Default I2C address for XL9535/XL9555 */
#define XL95X5_I2C_ADDRESS_MAX (0x20 + 8)
#define EXAMPLE_CHIP_ID 0x58 /*!< The chip ID, 0x58 */

    typedef enum
    {
        XL95X5_PORT_0 = 0, /*!< Port 0 */
        XL95X5_PORT_1,     /*!< Port 1 */
        XL95X5_PORT_MAX,   /*!< Maximum number of ports */
    } xl95x5_port_t;

    typedef enum {
        XL95X5_GPIO_OUTPUT = 0, /*!< GPIO configured as output */
        XL95X5_GPIO_INPUT,      /*!< GPIO configured as input */
    } xl95x5_gpio_mode_t;

    typedef enum {
        XL95X5_POLARITY_NOT_INVERTED = 0, /*!< Polarity not inverted */
        XL95X5_POLARITY_INVERTED,         /*!< Polarity inverted */
    } xl95x5_polarity_t;

    /**
     * @brief Initialize device descriptor
     *
     * Device SCL frequency is 400kHz.
     *
     * @param[in] dev Pointer to device descriptor
     * @param[in] port I2C port number
     * @param[in] addr I2C address
     * @param[in] sda_gpio SDA GPIO
     * @param[in] scl_gpio SCL GPIO
     * @return `ESP_OK` on success
     */

    esp_err_t xl95x5_init_desc(i2c_dev_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio);

    /**
     * @brief Free device descriptor
     *
     * @param[in] dev Pointer to device descriptor
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_free_desc(i2c_dev_t *dev);

    /**
     * @brief Read all(16) gpio level
     *
     * @param[in] dev Pointer to device descriptor
     * @param[out] id Pointer to store level status
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_get_full_gpio_level(i2c_dev_t *dev, uint16_t *levels);

    /**
     * @brief Read individual gpio level
     *
     * @param[in] dev Pointer to device descriptor
     * @param[in] gpio GPIO0...GPIO7 for PORT0, GPIO8...GPIO15 for PORT1
     * @param[out] id Buffer to store gpios level
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_get_gpio_level(i2c_dev_t *dev, gpio_num_t gpio, uint8_t *level);

    /**
     * @brief Set all(16) gpio level
     *
     * @note If GPIO is configured as input(default at reset), corresponding bitfield value will be ignored.
     *
     * @param[in] dev Pointer to device descriptor
     * @param[out] id New gpios level to set
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_set_full_gpio_level(i2c_dev_t *dev, uint16_t levels);

    /**
     * @brief Set individual gpio status
     *
     * @note If GPIO is configured as input(default at reset), value will be ignored.
     *
     * @param[in] dev Pointer to device descriptor
     * @param[in] gpio GPIO0...GPIO7 for PORT0, GPIO8...GPIO15 for PORT1
     * @param[in] level New gpio level to set
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_set_gpio_level(i2c_dev_t *dev, gpio_num_t gpio, uint8_t level);

    /**
     * @brief Read all(16) gpio polarity
     *
     * 0 - Not inverted, 1 - Inverted
     *
     * @param[in] dev Pointer to device descriptor
     * @param[out] polarity Buffer to store gpios polarity status
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_get_full_gpio_polarity(i2c_dev_t *dev, uint16_t *polarity);

    /**
     * @brief Read individual gpio polarity
     * 
     * @param[in] dev Pointer to device descriptor
     * @param[in] gpio GPIO0...GPIO7 for PORT0, GPIO8...GPIO15 for PORT1
     * @param[out] polarity gpio polarity value. 0 - Not inverted, 1 - Inverted
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_get_gpio_polarity(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_polarity_t *polarity);

    /**
     * @brief Set all(16) gpios polarity
     *
     * 0 - Not inverted, 1 - Inverted
     *
     * @param[in] dev Pointer to device descriptor
     * @param[out] polarity New GPIOs polarity to set
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_set_full_gpio_polarity(i2c_dev_t *dev, uint16_t polarity);


    /**
     * @brief Set individual gpio polarity
     *
     * 
     *
     * @param[in] dev Pointer to device descriptor
     * @param[in] gpio GPIO0...GPIO7 for PORT0, GPIO8...GPIO15 for PORT1
     * @param[in] polarity New gpio polarity to set. 0 - Not inverted, 1 - Inverted
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_set_gpio_polarity(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_polarity_t polarity);

    /**
     * @brief Read all(16) gpio mode (input/output)
     *
     * 0 - Output, 1 - Input
     *
     * @param[in] dev Pointer to device descriptor
     * @param[out] mode Buffer to store gpios mode
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_get_full_gpio_mode(i2c_dev_t *dev, uint16_t *mode);

    /**
     * @brief Read individual gpio mode (input/output)
     * 
     * @param[in] dev Pointer to device descriptor
     * @param[in] gpio GPIO0...GPIO7 for PORT0, GPIO8...GPIO15 for PORT1
     * @param[out] mode gpio mode. 0 - Output, 1 - Input
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_get_gpio_mode(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_gpio_mode_t *mode);

    /**
     * @brief Set all(16) gpios mode
     *
     * 0 - Output, 1 - Input
     *
     * @param[in] dev Pointer to device descriptor
     * @param[out] mode New gpios mode to set
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_set_full_gpio_mode(i2c_dev_t *dev, uint16_t mode);


    /**
     * @brief Set individual gpio mode
     *
     * @param[in] dev Pointer to device descriptor
     * @param[in] gpio GPIO0...GPIO7 for PORT0, GPIO8...GPIO15 for PORT1
     * @param[in] mode New gpio mode to set. 0 - Output, 1 - Input
     * @return `ESP_OK` on success
     */
    esp_err_t xl95x5_set_gpio_mode(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_gpio_mode_t mode);

#ifdef __cplusplus
}
#endif

#endif // __XL95X5_H__
