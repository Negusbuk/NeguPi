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

#ifndef NEGUPII2CDEVICE_H
#define NEGUPII2CDEVICE_H

#include <string>

namespace NeguPi {

  class I2CDevice
  {
  public:

    I2CDevice(const std::string& device, unsigned int address);

    bool open();
    bool close();

    int8_t readByte(unsigned int reg);
    int16_t readWord(unsigned int reg);

    void writeByte(unsigned int reg, int8_t value);
    void writeWord(unsigned int reg, int16_t value);

  private:

    std::string device_;
    unsigned int address_;

    int fd_;
    bool isOpen_;
  };
  
};

#endif
