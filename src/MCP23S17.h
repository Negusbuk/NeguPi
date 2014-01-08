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

#ifndef MCP23S17_H
#define MCP23S17_H

#include <cstdint>

#include <VMCP23S17.h>

class MCP23S17 : public VMCP23S17
{
public:

  explicit MCP23S17(uint8_t bus, uint8_t cs);
  virtual ~MCP23S17() { }

  virtual bool open();
  virtual bool close();

  virtual uint8_t readRegister(MCP23S17_REG reg, uint8_t hw_addr);
  virtual void writeRegister(MCP23S17_REG reg, uint8_t hw_addr, uint8_t data);

  virtual uint8_t readBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr);
  virtual void writeBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr, uint8_t data);

protected:

  uint8_t getSPIControlByte(MCP23S17_CMD cmd, uint8_t hw_addr);

  int fd_;

  const uint8_t spi_mode_ = 0;
  const uint8_t spi_bpw_ = 8; // bits per word
  const uint32_t spi_speed_ = 10000000; // 10MHz
  const uint16_t spi_delay_ = 0;
  const char * spidev_[2][2] = {
      {"/dev/spidev0.0", "/dev/spidev0.1"},
      {"/dev/spidev1.0", "/dev/spidev1.1"},
  };
};

#endif // MCP23S17_H
