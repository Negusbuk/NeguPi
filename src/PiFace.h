/****************************************************************************
 **
 **  Copyright (C) 2013, 2014 Andreas Mussgiller
 **
 **  based on libpifacedigital by Thomas Preston
 **           http://github.com/piface/libpifacedigital
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

#ifndef PIFACE_H
#define PIFACE_H

#include <cstdint>

#include <VMCP23S17.h>

namespace NeguPi {

  class PiFace
  {
  public:

    enum GPIO {
      Output = 0x12,
      Input  = 0x13
    };

    explicit PiFace(uint8_t hw_addr, uint8_t bus = 0, uint8_t cs = 0);
    virtual ~PiFace();

    bool init();

    uint8_t readRegister(uint8_t reg);
    void writeRegister(uint8_t reg, uint8_t data);
    uint8_t readBit(uint8_t bit, uint8_t reg);
    void writeBit(uint8_t bit, uint8_t reg, uint8_t data);

  protected:

    uint8_t hw_addr_;
    VMCP23S17* mcp23s17_;

    static uint8_t useCount_;
  };
  
};

#endif // PIFACE_H
