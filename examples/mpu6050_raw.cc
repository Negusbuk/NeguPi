/****************************************************************************
 **
 **  Copyright (C) 2019 Andreas Mussgiller
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

int main(int argc, char **argv)
{
  NeguPi::MPU6050 mpu6050("/dev/i2c-1", 0x68);

  if (!mpu6050.open()) return EXIT_FAILURE;

  mpu6050.setSleepEnabled(false);

  while(1) {
    int16_t temp = mpu6050.getTemperature();

    int16_t xaccel = mpu6050.getAccelerationX();
    int16_t yaccel = mpu6050.getAccelerationY();
    int16_t zaccel = mpu6050.getAccelerationZ();

    int16_t xgyro = mpu6050.getRotationX();
    int16_t ygyro = mpu6050.getRotationY();
    int16_t zgyro = mpu6050.getRotationZ();

    std::cout << "temp; accel x,y,z; gyro x,y,z: "
        << std::fixed << std::setw(6) << std::setprecision(2)
        << (float)temp / 340.0f + 36.53
		<< std::setw(6) << (int)xaccel
		<< std::setw(6) << (int)yaccel
		<< std::setw(6) << (int)zaccel
		<< std::setw(6) << (int)xgyro
		<< std::setw(6) << (int)ygyro
		<< std::setw(6) << (int)zgyro << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  mpu6050.setSleepEnabled(true);

  mpu6050.close();

  return 0;
}

