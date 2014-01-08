/****************************************************************************
 **
 **  Copyright (C) 2013, 2014 Andreas Mussgiller
 **
 **  based on libmcp23s17 by Thomas Preston
 **           http://github.com/piface/libmcp23s17
 **
 **  Datasheet: http://ww1.microchip.com/downloads/en/devicedoc/21952b.pdf
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

#ifndef MCP23S17Fake_H
#define MCP23S17Fake_H

#include <cstdint>

#include <VMCP23S17.h>

class MCP23S17Fake : public VMCP23S17
{
public:

  explicit MCP23S17Fake(uint8_t bus, uint8_t cs);
  virtual ~MCP23S17Fake() { }

  virtual bool open();
  virtual bool close();

  virtual uint8_t readRegister(MCP23S17_REG reg, uint8_t hw_addr);
  virtual void writeRegister(MCP23S17_REG reg, uint8_t hw_addr, uint8_t data);

  virtual uint8_t readBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr);
  virtual void writeBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr, uint8_t data);

protected:

  uint8_t registers_[NREGISTERS];
};

#endif // MCP23S17Fake_H
