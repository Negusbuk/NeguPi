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

#include "MCP23S17Fake.h"

MCP23S17Fake::MCP23S17Fake(uint8_t bus, uint8_t cs)
:VMCP23S17(bus, cs)
{
  for (uint8_t i=0;i<NREGISTERS;++i) {
    registers_[i] = 0;
  }
}

bool MCP23S17Fake::open()
{
  return true;
}

bool MCP23S17Fake::close()
{
  return true;
}

uint8_t MCP23S17Fake::readRegister(MCP23S17_REG reg, uint8_t hw_addr)
{
  return registers_[reg];
}

void MCP23S17Fake::writeRegister(MCP23S17_REG reg, uint8_t hw_addr, uint8_t data)
{
  registers_[reg] = data;
}

uint8_t MCP23S17Fake::readBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr)
{
  return (registers_[reg] >> bit) & 1;
}

void MCP23S17Fake::writeBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr, uint8_t data)
{
  uint8_t reg_data = readRegister(reg, hw_addr);
  if (data) {
    reg_data |= 1 << bit; // set
  } else {
    reg_data &= 0xff ^ (1 << bit); // clear
  }

  writeRegister(reg, hw_addr, reg_data);
}
