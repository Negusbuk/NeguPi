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

NeguPi::MPU6050 mpu6050;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

void loop()
{
  if (!dmpReady) return;
  // get current FIFO count
  fifoCount = mpu6050.getFIFOCount();

  if (fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu6050.resetFIFO();
    printf("FIFO overflow!\n");

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (fifoCount >= 42) {
    // read a packet from FIFO
    mpu6050.getFIFOBytes(fifoBuffer, packetSize);

    // display real acceleration, adjusted to remove gravity
    NeguPi::Quaternion q = mpu6050.dmpGetQuaternion(fifoBuffer);
    NeguPi::VectorInt16 aa = mpu6050.dmpGetAccel(fifoBuffer);
    NeguPi::VectorFloat gravity = mpu6050.dmpGetGravity(q);
    NeguPi::VectorInt16 aaReal = mpu6050.dmpGetLinearAccel(aa, gravity);

    printf("areal %6d %6d %6d    ", aaReal.x(), aaReal.y(), aaReal.z());

    printf("\n");
  }
}

int main(int argc, char **argv)
{
  mpu6050 = NeguPi::MPU6050("/dev/i2c-1", 0x68);

  if (!mpu6050.open()) return EXIT_FAILURE;

  mpu6050.initialize();

  if (mpu6050.testConnection()) {
    std::cout << "MPU6050 connection successful" << std::endl;
  } else {
    std::cout << "MPU6050 connection failed" << std::endl;
  }

  devStatus = mpu6050.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu6050.setXGyroOffset(30);
  mpu6050.setYGyroOffset(260);
  mpu6050.setZGyroOffset(27);

  // mpu.setXAccelOffset(5343 - 39);
  // mpu.setYAccelOffset(-4872 + 2);
  // mpu.setZAccelOffset(9023 + 7);

  mpu6050.setRate(4);

  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    printf("Enabling DMP...\n");
    mpu6050.setDMPEnabled(true);

    // enable Arduino interrupt detection
    //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    //attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu6050.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    printf("DMP ready!\n");
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu6050.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    printf("DMP Initialization failed (code %d)\n", devStatus);
  }

  std::this_thread::sleep_for(std::chrono::microseconds(100000));

  while (1) loop();

  return 0;
}

