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

#ifndef NEGUPII2CDEVICE_H
#define NEGUPII2CDEVICE_H

#include <string>

namespace NeguPi {

  class I2CDevice
  {
  public:

    I2CDevice(const std::string& device, uint8_t address);

    bool open();
    bool close();

    int8_t readBit(uint8_t regAddr, uint8_t bitNum);
    int8_t readBits(uint8_t regAddr, uint8_t bitStart, uint8_t numBits);
    int8_t readByte(uint8_t regAddr);
    int16_t readWord(uint8_t regAddr);
    void readBytes(uint8_t regAddr, uint8_t length, uint8_t *data);

    bool writeBit(uint8_t regAddr,uint8_t bitNum, int8_t data);
    bool writeBits(uint8_t regAddr, uint8_t bitStart, uint8_t numBits, int8_t data);
    bool writeByte(uint8_t regAddr, int8_t data);
    bool writeWord(uint8_t regAddr, int16_t data);
    bool writeBytes(uint8_t regAddr, uint8_t length, uint8_t *data);

  private:

    std::string device_;
    uint8_t address_;

    int fd_;
    bool isOpen_;
  };
  
};

#endif
