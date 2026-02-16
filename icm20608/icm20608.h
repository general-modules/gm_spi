/**
 * @file      icm20608.h
 * @brief     ICM20608 驱动头文件
 * @author    huenrong (sgyhy1028@outlook.com)
 * @date      2026-02-16 16:27:29
 *
 * @copyright Copyright (c) 2026 huenrong
 *
 */

#ifndef __ICM20608_H
#define __ICM20608_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief 初始化 ICM20608
 *
 * @return 0 : 成功
 * @return <0: 失败
 */
int icm20608_init(void);

/**
 * @brief 读取 ICM20608 ID
 *
 * @param[out] id: ICM20608 ID
 *
 * @return 0 : 成功
 * @return <0: 失败
 */
int icm20608_read_id(uint8_t *id);

/**
 * @brief 读取 ICM20608 加速度
 *
 * @param[out] accel_x: X 轴实际数据 (单位: g)
 * @param[out] accel_y: Y 轴实际数据 (单位: g)
 * @param[out] accel_z: Z 轴实际数据 (单位: g)
 *
 * @return 0 : 成功
 * @return <0: 失败
 */
int icm20608_read_accel(float *accel_x, float *accel_y, float *accel_z);

/**
 * @brief 读取 ICM20608 温度
 *
 * @param[out] temp: 温度 (单位: °C)
 *
 * @return 0 : 成功
 * @return <0: 失败
 */
int icm20608_read_temp(float *temp);

/**
 * @brief 读取 ICM20608 陀螺仪
 *
 * @param[out] gyro_x: X 轴实际数据 (单位: °/s)
 * @param[out] gyro_y: Y 轴实际数据 (单位: °/s)
 * @param[out] gyro_z: Z 轴实际数据 (单位: °/s)
 *
 * @return 0 : 成功
 * @return <0: 失败
 */
int icm20608_read_gyro(float *gyro_x, float *gyro_y, float *gyro_z);

#ifdef __cplusplus
}
#endif

#endif // __ICM20608_H
