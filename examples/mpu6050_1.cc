/****************************************************************************
 **
 **  Copyright (C) 2013 - 2019 Andreas Mussgiller
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **
 **
 ****************************************************************************/

#include <sys/types.h>

#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

#include <MPU6050.h>

#define MPU_ACCEL_XOUT1 0x3b
#define MPU_ACCEL_XOUT2 0x3c
#define MPU_ACCEL_YOUT1 0x3d
#define MPU_ACCEL_YOUT2 0x3e
#define MPU_ACCEL_ZOUT1 0x3f
#define MPU_ACCEL_ZOUT2 0x40

#define MPU_GYRO_XOUT1 0x43
#define MPU_GYRO_XOUT2 0x44
#define MPU_GYRO_YOUT1 0x45
#define MPU_GYRO_YOUT2 0x46
#define MPU_GYRO_ZOUT1 0x47
#define MPU_GYRO_ZOUT2 0x48

#define MPU_TEMP1 0x41
#define MPU_TEMP2 0x42

#define MPU_POWER1 0x6b
#define MPU_POWER2 0x6c

int main(int argc, char **argv)
{
  NeguPi::MPU6050 mpu6050("/dev/i2c-1", 0x68);

  if (!mpu6050.open()) return EXIT_FAILURE;

  int8_t power = mpu6050.readByte(MPU_POWER1);
  mpu6050.writeByte(MPU_POWER1, ~(1 << 6) & power);

  while(1) {
    int16_t temp1 = mpu6050.readWord(MPU_TEMP1);
    int16_t temp2 = mpu6050.getTemperature();

    int16_t xaccel = mpu6050.readWord(MPU_ACCEL_XOUT1);
    int16_t yaccel = mpu6050.readWord(MPU_ACCEL_YOUT1);
    int16_t zaccel = mpu6050.readWord(MPU_ACCEL_ZOUT1);

    int16_t xgyro = mpu6050.readWord(MPU_GYRO_XOUT1);
    int16_t ygyro = mpu6050.readWord(MPU_GYRO_YOUT1);
    int16_t zgyro = mpu6050.readWord(MPU_GYRO_ZOUT1);

    std::cout << "temp; accel x,y,z; gyro x,y,z: "
        << std::fixed << std::setw(6) << std::setprecision(2)
    << (float)temp1 / 340.0f + 36.53
    << std::fixed << std::setw(6) << std::setprecision(2)
    << (float)temp2 / 340.0f + 36.53
    << std::setw(6) << (int)xaccel
    << std::setw(6) << (int)yaccel
    << std::setw(6) << (int)zaccel
    << std::setw(6) << (int)xgyro
    << std::setw(6) << (int)ygyro
    << std::setw(6) << (int)zgyro << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  mpu6050.close();

  return 0;
}

