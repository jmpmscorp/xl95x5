#include "xl95x5.h"
#include "esp_log.h"

#define I2C_FREQ_HZ (400000)

#define REG_INPUT_PORT0 (0x00)
#define REG_INPUT_PORT1 (0x01)
#define REG_OUTPUT_PORT0 (0x02)
#define REG_OUTPUT_PORT1 (0x03)
#define REG_POLARITY_PORT0 (0x04)
#define REG_POLARITY_PORT1 (0x05)
#define REG_MODE_CONFIG_PORT0 (0x06)
#define REG_MODE_CONFIG_PORT1 (0x07)

// clang-format off
#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)
// clang-format on

static esp_err_t read_reg_16(i2c_dev_t *dev, uint8_t reg, uint16_t *val)
{
    CHECK_ARG(dev && val);

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_read_reg(dev, reg, val, 2));
    I2C_DEV_GIVE_MUTEX(dev);

    return ESP_OK;
}

static esp_err_t write_reg_16(i2c_dev_t *dev, uint8_t reg, uint16_t val)
{
    CHECK_ARG(dev);

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_write_reg(dev, reg, &val, 2));
    I2C_DEV_GIVE_MUTEX(dev);

    return ESP_OK;
}

// static esp_err_t write_reg_bit_16(i2c_dev_t *dev, uint8_t reg, bool val, uint8_t bit)
// {
//     CHECK_ARG(dev);

//     uint16_t buf;

//     I2C_DEV_TAKE_MUTEX(dev);
//     I2C_DEV_CHECK(dev, i2c_dev_read_reg(dev, reg, &buf, 2));
//     buf = (buf & ~BIT(bit)) | (val ? BIT(bit) : 0);
//     I2C_DEV_CHECK(dev, i2c_dev_write_reg(dev, reg, &buf, 2));
//     I2C_DEV_GIVE_MUTEX(dev);

//     return ESP_OK;
// }

static esp_err_t read_reg_bit_8(i2c_dev_t *dev, uint8_t reg, bool *val, uint8_t bit)
{
    CHECK_ARG(dev && val);

    uint8_t buf;

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_read_reg(dev, reg, &buf, 1));
    I2C_DEV_GIVE_MUTEX(dev);

    *val = (buf & BIT(bit)) >> bit;

    return ESP_OK;
}

static esp_err_t write_reg_bit_8(i2c_dev_t *dev, uint8_t reg, bool val, uint8_t bit)
{
    CHECK_ARG(dev);

    uint8_t buf;

    I2C_DEV_TAKE_MUTEX(dev);
    I2C_DEV_CHECK(dev, i2c_dev_read_reg(dev, reg, &buf, 1));
    buf = (buf & ~BIT(bit)) | (val ? BIT(bit) : 0);
    I2C_DEV_CHECK(dev, i2c_dev_write_reg(dev, reg, &buf, 1));
    I2C_DEV_GIVE_MUTEX(dev);

    return ESP_OK;
}

esp_err_t xl95x5_init_desc(i2c_dev_t *dev, uint8_t addr, i2c_port_t port, gpio_num_t sda_gpio, gpio_num_t scl_gpio)
{
    CHECK_ARG(dev && addr >= XL95X5_I2C_ADDRESS_BASE && addr <= XL95X5_I2C_ADDRESS_MAX);

    dev->port = port;
    dev->addr = addr;
    dev->addr_bit_len = I2C_ADDR_BIT_LEN_7;
    dev->cfg.sda_io_num = sda_gpio;
    dev->cfg.scl_io_num = scl_gpio;
    dev->cfg.master.clk_speed = I2C_FREQ_HZ;

    return i2c_dev_create_mutex(dev);
}

esp_err_t xl95x5_free_desc(i2c_dev_t *dev)
{
    CHECK_ARG(dev);

    return i2c_dev_delete_mutex(dev);
}

esp_err_t xl95x5_get_full_gpio_level(i2c_dev_t *dev, uint16_t *levels)
{
    return read_reg_16(dev, REG_INPUT_PORT0, levels);
}

esp_err_t xl95x5_get_gpio_level(i2c_dev_t *dev, gpio_num_t gpio, uint8_t *level)
{
    CHECK_ARG(gpio >= GPIO_NUM_0 && gpio <= GPIO_NUM_15);

    return read_reg_bit_8(dev, (gpio < GPIO_NUM_8) ? REG_INPUT_PORT0 : REG_INPUT_PORT1, (bool *)level, gpio % 8);
}

esp_err_t xl95x5_set_full_gpio_level(i2c_dev_t *dev, uint16_t levels)
{
    return write_reg_16(dev, REG_OUTPUT_PORT0, levels);
}

esp_err_t xl95x5_set_gpio_level(i2c_dev_t *dev, gpio_num_t gpio, uint8_t level)
{
    CHECK_ARG(gpio >= GPIO_NUM_0 && gpio <= GPIO_NUM_15);

    return write_reg_bit_8(dev, (gpio < GPIO_NUM_8) ? REG_OUTPUT_PORT0 : REG_OUTPUT_PORT1, level, gpio % 8);
}

esp_err_t xl95x5_get_full_gpio_polarity(i2c_dev_t *dev, uint16_t *polarity)
{
    return read_reg_16(dev, REG_POLARITY_PORT0, polarity);
}

esp_err_t xl95x5_get_gpio_polarity(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_polarity_t *polarity)
{
    CHECK_ARG(gpio >= GPIO_NUM_0 && gpio <= GPIO_NUM_15 && polarity);

    return read_reg_bit_8(dev, (gpio < GPIO_NUM_8) ? REG_POLARITY_PORT0 : REG_POLARITY_PORT1, (bool *)polarity, gpio % 8);
}

esp_err_t xl95x5_set_full_gpio_polarity(i2c_dev_t *dev, uint16_t polarity)
{
    return write_reg_16(dev, REG_POLARITY_PORT0, polarity);
}

esp_err_t xl95x5_set_gpio_polarity(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_polarity_t polarity)
{
    CHECK_ARG(gpio >= GPIO_NUM_0 && gpio <= GPIO_NUM_15);

    return write_reg_bit_8(dev, (gpio < GPIO_NUM_8) ? REG_POLARITY_PORT0 : REG_POLARITY_PORT1, polarity, gpio % 8);
}

esp_err_t xl95x5_get_full_gpio_mode(i2c_dev_t *dev, uint16_t *mode)
{
    return read_reg_16(dev, REG_MODE_CONFIG_PORT0, mode);
}

esp_err_t xl95x5_get_gpio_mode(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_gpio_mode_t *mode)
{
    CHECK_ARG(gpio >= GPIO_NUM_0 && gpio <= GPIO_NUM_15 && mode);

    return read_reg_bit_8(dev, (gpio < GPIO_NUM_8) ? REG_MODE_CONFIG_PORT0 : REG_MODE_CONFIG_PORT1, (bool *)mode, gpio % 8);
}

esp_err_t xl95x5_set_full_gpio_mode(i2c_dev_t *dev, uint16_t mode)
{
    return write_reg_16(dev, REG_MODE_CONFIG_PORT0, mode);
}

esp_err_t xl95x5_set_gpio_mode(i2c_dev_t *dev, gpio_num_t gpio, xl95x5_gpio_mode_t mode)
{
    CHECK_ARG(gpio >= GPIO_NUM_0 && gpio <= GPIO_NUM_15);

    return write_reg_bit_8(dev, (gpio < GPIO_NUM_8) ? REG_MODE_CONFIG_PORT0 : REG_MODE_CONFIG_PORT1, mode, gpio % 8);
}