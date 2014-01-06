/****************************************************************************
 **
 **  Copyright (C) 2013, 2014 Andreas Mussgiller
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

#ifndef VPIFACE_H
#define VPIFACE_H

#include <cstdint>

class VPiFace
{
public:

  enum PiFaceGPIO {
    Output = 0x12,
    Input  = 0x13
  };

  explicit VPiFace(uint8_t hw_addr);
  virtual ~VPiFace() { }

  virtual bool init() = 0;

  virtual uint8_t readRegister(uint8_t reg) = 0;
  virtual void writeRegister(uint8_t data, uint8_t reg) = 0;
  virtual uint8_t readBit(uint8_t bit, uint8_t reg) = 0;
  virtual void writeBit(uint8_t data, uint8_t bit, uint8_t reg) = 0;
  
protected:

  uint8_t hw_addr_;
};

#endif // VPIFACE_H
