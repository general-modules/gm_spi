/**
 * @file      examples_icm20608.c
 * @brief     ICM20608 示例代码源文件
 * @author    huenrong (sgyhy1028@outlook.com)
 * @date      2026-02-16 16:27:38
 *
 * @copyright Copyright (c) 2026 huenrong
 *
 */

#include <stdio.h>
#include <unistd.h>

#include "icm20608.h"

/**
 * @brief 程序入口
 *
 * @param[in] argc: 参数个数
 * @param[in] argv: 参数列表
 *
 * @return 成功: 0
 * @return 失败: 其它
 */
int main(int argc, char *argv[])
{
    int ret = icm20608_init();
    if (ret != 0)
    {
        printf("icm20608 init failed. ret: %d\n", ret);

        return -1;
    }

    uint8_t icm20608_id = 0;
    ret = icm20608_read_id(&icm20608_id);
    if (ret != 0)
    {
        printf("icm20608 read id failed. ret: %d\n", ret);

        return -1;
    }
    printf("icm20608 id: 0x%02X\n", icm20608_id);

    while (true)
    {
        // ICM20608读取加速度值
        float accel_x = 0;
        float accel_y = 0;
        float accel_z = 0;
        ret = icm20608_read_accel(&accel_x, &accel_y, &accel_z);
        if (ret != 0)
        {
            printf("read icm20608 accel failed. ret: %d\n", ret);
        }

        // ICM20608读取温度值
        float temp = 0;
        ret = icm20608_read_temp(&temp);
        if (ret != 0)
        {
            printf("read icm20608 temp failed. ret: %d\n", ret);
        }

        // icm20608读取陀螺仪值
        float gyro_x = 0;
        float gyro_y = 0;
        float gyro_z = 0;
        ret = icm20608_read_gyro(&gyro_x, &gyro_y, &gyro_z);
        if (ret != 0)
        {
            printf("read icm20608 gyro failed. ret: %d\n", ret);
        }

        // 打印各参数
        printf("accel_x = %.2f (g), accel_x = %.2f (g), accel_x = %.2f (g)\n", accel_x, accel_y, accel_z);
        printf("gyro_x = %.2f (°/S), gyro_x = %.2f (°/S), gyro_x = %.2f (°/S)\n", gyro_x, gyro_y, gyro_z);
        printf("temp = %.2f (°C)\n\n", temp);
        printf("==========================================================\n\n");

        sleep(1);
    }

    return 0;
}
