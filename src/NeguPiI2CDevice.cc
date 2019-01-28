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

  I2CDevice::I2CDevice(const std::string& device, unsigned int address)
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

    if ((fd_ = ::open(device_, O_RDWR)) < 0) {
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

  int8_t I2CDevice::readByte(unsigned int reg)
  {
#ifndef NODEVICE
    return i2c_smbus_read_byte_data(fd_, reg);
#else
    return 0;
#endif
  }

  int16_t I2CDevice::readWord(unsigned int reg)
  {
#ifndef NODEVICE
    return i2c_smbus_read_byte_data(fd_, reg) << 8 |
           i2c_smbus_read_byte_data(fd_, reg + 0x01);
#else
    return 0;
#endif
  }

  void I2CDevice::writeByte(unsigned int reg, int8_t value)
  {
#ifndef NODEVICE
    i2c_smbus_write_byte_data(fd, reg, value);
#endif
  }

  void I2CDevice::writeWord(unsigned int reg, int16_t value)
  {
#ifndef NODEVICE
    i2c_smbus_write_byte_data(fd, reg + 0x01, value & 0xff);
    i2c_smbus_write_byte_data(fd, reg, (value >> 8) & 0xff);
#endif
  }

};
