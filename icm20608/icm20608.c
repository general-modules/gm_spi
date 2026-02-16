/**
 * @file      icm20608.c
 * @brief     ICM20608 驱动源文件
 * @author    huenrong (sgyhy1028@outlook.com)
 * @date      2026-02-16 16:27:23
 *
 * @copyright Copyright (c) 2026 huenrong
 *
 */

#include <stdio.h>
#include <unistd.h>

#include "icm20608.h"
#include "gm_spi.h"

// ICM20608 设备路径
#define ICM20608_DEV_PATH "/dev/spidev2.0"

// ICM20608 设备片选脚 (GPIO1_IO20)
#define GPIO_ICM20608_CS 20

// 陀螺仪和加速度自测(出厂时设置, 用于与用户的自检输出值比较)
#define ICM20_SELF_TEST_X_GYRO  0x00
#define ICM20_SELF_TEST_Y_GYRO  0x01
#define ICM20_SELF_TEST_Z_GYRO  0x02
#define ICM20_SELF_TEST_X_ACCEL 0x0D
#define ICM20_SELF_TEST_Y_ACCEL 0x0E
#define ICM20_SELF_TEST_Z_ACCEL 0x0F

// 陀螺仪静态偏移
#define ICM20_XG_OFFS_USRH 0x13
#define ICM20_XG_OFFS_USRL 0x14
#define ICM20_YG_OFFS_USRH 0x15
#define ICM20_YG_OFFS_USRL 0x16
#define ICM20_ZG_OFFS_USRH 0x17
#define ICM20_ZG_OFFS_USRL 0x18

#define ICM20_SMPLRT_DIV    0x19
#define ICM20_CONFIG        0x1A
#define ICM20_GYRO_CONFIG   0x1B
#define ICM20_ACCEL_CONFIG  0x1C
#define ICM20_ACCEL_CONFIG2 0x1D
#define ICM20_LP_MODE_CFG   0x1E
#define ICM20_ACCEL_WOM_THR 0x1F
#define ICM20_FIFO_EN       0x23
#define ICM20_FSYNC_INT     0x36
#define ICM20_INT_PIN_CFG   0x37
#define ICM20_INT_ENABLE    0x38
#define ICM20_INT_STATUS    0x3A

// 加速度输出
#define ICM20_ACCEL_XOUT_H 0x3B
#define ICM20_ACCEL_XOUT_L 0x3C
#define ICM20_ACCEL_YOUT_H 0x3D
#define ICM20_ACCEL_YOUT_L 0x3E
#define ICM20_ACCEL_ZOUT_H 0x3F
#define ICM20_ACCEL_ZOUT_L 0x40

// 温度输出
#define ICM20_TEMP_OUT_H 0x41
#define ICM20_TEMP_OUT_L 0x42

// 陀螺仪输出
#define ICM20_GYRO_XOUT_H 0x43
#define ICM20_GYRO_XOUT_L 0x44
#define ICM20_GYRO_YOUT_H 0x45
#define ICM20_GYRO_YOUT_L 0x46
#define ICM20_GYRO_ZOUT_H 0x47
#define ICM20_GYRO_ZOUT_L 0x48

#define ICM20_SIGNAL_PATH_RESET 0x68
#define ICM20_ACCEL_INTEL_CTRL  0x69
#define ICM20_USER_CTRL         0x6A
#define ICM20_PWR_MGMT_1        0x6B
#define ICM20_PWR_MGMT_2        0x6C
#define ICM20_FIFO_COUNTH       0x72
#define ICM20_FIFO_COUNTL       0x73
#define ICM20_FIFO_R_W          0x74
#define ICM20_WHO_AM_I          0x75

// 加速度静态偏移
#define ICM20_XA_OFFSET_H 0x77
#define ICM20_XA_OFFSET_L 0x78
#define ICM20_YA_OFFSET_H 0x7A
#define ICM20_YA_OFFSET_L 0x7B
#define ICM20_ZA_OFFSET_H 0x7D
#define ICM20_ZA_OFFSET_L 0x7E

static gm_spi_t *s_gm_spi = NULL;

static int on_cs_control(bool enable)
{
    if (enable)
    {
    }
    else
    {
    }

    return 0;
}

int icm20608_init(void)
{
    // FIXME: 初始化 GPIO

    s_gm_spi = gm_spi_create();
    if (s_gm_spi == NULL)
    {
        printf("create gm_spi failed\n");

        return -1;
    }

    int ret = gm_spi_init(s_gm_spi, ICM20608_DEV_PATH, E_GM_SPI_MODE_0, 8000000, 8);
    if (ret != 0)
    {
        printf("gm_spi_init failed. ret: %d\n", ret);

        return -2;
    }

    ret = gm_spi_set_cs_control_cb(s_gm_spi, on_cs_control);
    if (ret != 0)
    {
        printf("gm_spi_set_cs_control_cb failed. ret: %d\n", ret);

        return -3;
    }

    uint8_t reg_addr = 0;
    uint8_t reg_data = 0;
    // 重置内部寄存器, 并恢复默认设置
    reg_addr = (ICM20_PWR_MGMT_1 & 0x7F);
    reg_data = 0x80;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("reset icm20608 failed. ret: %d\n", ret);

        return -4;
    }
    usleep(50 * 1000);

    // 自动选择最佳的可用时钟源–PLL(如果已准备好), 否则使用内部振荡器
    reg_addr = (ICM20_PWR_MGMT_1 & 0x7F);
    reg_data = 0x01;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 clock failed. ret: %d\n", ret);

        return -5;
    }
    usleep(50 * 1000);

    // 输出速率是内部采样率
    reg_addr = (ICM20_SMPLRT_DIV & 0x7F);
    reg_data = 0x00;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 sample rate failed. ret: %d\n", ret);

        return -6;
    }

    // 陀螺仪±2000dps量程
    reg_addr = (ICM20_GYRO_CONFIG & 0x7F);
    reg_data = 0x18;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 gyro range failed. ret: %d\n", ret);

        return -7;
    }

    // 加速度计±16G量程
    reg_addr = (ICM20_ACCEL_CONFIG & 0x7F);
    reg_data = 0x18;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 accel range failed. ret: %d\n", ret);

        return -8;
    }

    // 陀螺仪低通滤波BW=20Hz
    reg_addr = (ICM20_CONFIG & 0x7F);
    reg_data = 0x04;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 gyro low pass filter failed. ret: %d\n", ret);

        return -9;
    }

    // 加速度计低通滤波BW=21.2Hz
    reg_addr = (ICM20_ACCEL_CONFIG2 & 0x7F);
    reg_data = 0x04;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 accel low pass filter failed. ret: %d\n", ret);

        return -10;
    }

    // 打开加速度计和陀螺仪所有轴
    reg_addr = (ICM20_PWR_MGMT_2 & 0x7F);
    reg_data = 0x00;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 power failed. ret: %d\n", ret);

        return -11;
    }

    // 关闭低功耗
    reg_addr = (ICM20_LP_MODE_CFG & 0x7F);
    reg_data = 0x00;
    if (ret != 0)
    {
        printf("set icm20608 low power failed. ret: %d\n", ret);

        return -12;
    }

    // 关闭FIFO
    reg_addr = (ICM20_FIFO_EN & 0x7F);
    reg_data = 0x00;
    ret = gm_spi_write_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("set icm20608 fifo failed. ret: %d\n", ret);

        return -13;
    }

    return 0;
}

int icm20608_read_id(uint8_t *id)
{
    uint8_t reg_addr = (ICM20_WHO_AM_I | 0x80);
    uint8_t reg_data = 0;

    int ret = gm_spi_read_data_sub(s_gm_spi, reg_addr, &reg_data, 1);
    if (ret != 0)
    {
        printf("read icm20608 id failed. ret: %d\n", ret);

        return -1;
    }

    *id = reg_data;

    return 0;
}

int icm20608_read_accel(float *accel_x, float *accel_y, float *accel_z)
{
    uint8_t reg_addr = (ICM20_ACCEL_XOUT_H | 0x80);
    uint8_t reg_data[6] = {0};

    int ret = gm_spi_read_data_sub(s_gm_spi, reg_addr, reg_data, 6);
    if (ret != 0)
    {
        printf("read icm20608 accel failed. ret: %d\n", ret);

        return -1;
    }

    // 原始值
    *accel_x = (float)((int16_t)(reg_data[0] << 8 | reg_data[1]));
    *accel_y = (float)((int16_t)(reg_data[2] << 8 | reg_data[3]));
    *accel_z = (float)((int16_t)(reg_data[4] << 8 | reg_data[5]));

    // 转换为实际值
    *accel_x /= 2048.0;
    *accel_y /= 2048.0;
    *accel_z /= 2048.0;

    return 0;
}

int icm20608_read_temp(float *temp)
{
    uint8_t reg_addr = (ICM20_TEMP_OUT_H | 0x80);
    uint8_t reg_data[2] = {0};

    int ret = gm_spi_read_data_sub(s_gm_spi, reg_addr, reg_data, 2);
    if (ret != 0)
    {
        printf("read icm20608 temp failed. ret: %d\n", ret);

        return -1;
    }

    // 原始值
    *temp = (short)((reg_data[0] << 8) | reg_data[1]);

    // 转换为实际值
    *temp = (((*temp - 25) / 326.8) + 25);

    return 0;
}

int icm20608_read_gyro(float *gyro_x, float *gyro_y, float *gyro_z)
{
    uint8_t reg_addr = (ICM20_GYRO_XOUT_H | 0x80);
    uint8_t reg_data[6] = {0};

    int ret = gm_spi_read_data_sub(s_gm_spi, reg_addr, reg_data, 6);
    if (ret != 0)
    {
        printf("read icm20608 gyro failed. ret: %d\n", ret);

        return -1;
    }

    // 原始值
    *gyro_x = (float)((int16_t)(reg_data[0] << 8 | reg_data[1]));
    *gyro_y = (float)((int16_t)(reg_data[2] << 8 | reg_data[3]));
    *gyro_z = (float)((int16_t)(reg_data[4] << 8 | reg_data[5]));

    // 转换为实际值
    *gyro_x /= 16.4;
    *gyro_y /= 16.4;
    *gyro_z /= 16.4;

    return 0;
}
