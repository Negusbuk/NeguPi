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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef NODEVICE
#include <linux/i2c-dev.h>
#endif

#include <iostream>

#include "NeguPiI2CDevice.h"

namespace NeguPi {

  I2CDevice::I2CDevice(const std::string& device, uint8_t address)
    : device_(device),
      address_(address),
      fd_(0),
      isOpen_(false)
  {

  }

  bool I2CDevice::open()
  {
#ifndef NODEVICE

    isOpen_ = false;

    if ((fd_ = ::open(device_.c_str(), O_RDWR)) < 0) {
      std::cerr << "Failed to open i2c port" << std::endl;
      return false;
    }

    if (ioctl(fd_, I2C_SLAVE, address_) < 0) {
      std::cerr << "Unable to get bus access to talk to slave" << std::endl;
      return false;
    }

    isOpen_ = true;

    return true;

#else
    return false;
#endif
  }

  bool I2CDevice::close()
  {
    if (!isOpen_) return false;

#ifndef NODEVICE

    ::close(fd_);

    isOpen_ = false;

    return true;

#else
    return false;
#endif
  }

  int8_t I2CDevice::readBit(uint8_t regAddr, uint8_t bitNum)
  {
    uint8_t b = readByte(regAddr);
    return (b & (1 << bitNum));
  }

  int8_t I2CDevice::readBits(uint8_t regAddr, uint8_t bitStart, uint8_t numBits)
  {
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t b = readByte(regAddr);
    uint8_t mask = ((1 << numBits) - 1) << (bitStart - numBits + 1);
    b &= mask;
    b >>= (bitStart - numBits + 1);
    return b;
  }

  int8_t I2CDevice::readByte(uint8_t regAddr)
  {
#ifndef NODEVICE
    return i2c_smbus_read_byte_data(fd_, regAddr);
#else
    return 0;
#endif
  }

  int16_t I2CDevice::readWord(uint8_t regAddr)
  {
#ifndef NODEVICE
    return i2c_smbus_read_byte_data(fd_, regAddr) << 8 |
           i2c_smbus_read_byte_data(fd_, regAddr + 0x01);
#else
    return 0;
#endif
  }

  void I2CDevice::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data)
  {
#ifndef NODEVICE
    i2c_smbus_read_i2c_block_data(fd_, regAddr, length, data);
#endif
  }

  bool I2CDevice::writeBit(uint8_t regAddr, uint8_t bitNum, int8_t data)
  {
    uint8_t b = readByte(regAddr);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return writeByte(regAddr, b);
  }

  bool I2CDevice::writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t numBits, int8_t data)
  {
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b = readByte(regAddr);
    uint8_t mask = ((1 << numBits) - 1) << (bitStart - numBits + 1);
    data <<= (bitStart - numBits + 1); // shift data into correct position
    data &= mask; // zero all non-important bits in data
    b &= ~(mask); // zero all important bits in existing byte
    b |= data; // combine data with existing byte
    return writeByte(regAddr, b);
  }

  bool I2CDevice::writeByte(uint8_t regAddr, int8_t data)
  {
#ifndef NODEVICE
    return i2c_smbus_write_byte_data(fd_, regAddr, data);
#else
    return false;
#endif
  }

  bool I2CDevice::writeWord(uint8_t regAddr, int16_t data)
  {
#ifndef NODEVICE
    return i2c_smbus_write_byte_data(fd_, regAddr + 0x01, data & 0xff) &
           i2c_smbus_write_byte_data(fd_, regAddr, (data >> 8) & 0xff);
#else
    return false;
#endif
  }

  bool I2CDevice::writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data)
  {
#ifndef NODEVICE
    return i2c_smbus_write_i2c_block_data(fd_, regAddr, length, data);
#else
    return false;
#endif
  }
};
