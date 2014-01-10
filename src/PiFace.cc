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

#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#ifdef NODEVICE
#include <MCP23S17Fake.h>
typedef NeguPi::MCP23S17Fake MCP23S17_t;
#else
#include <MCP23S17.h>
typedef NeguPi::MCP23S17 MCP23S17_t;
#endif

#include "PiFace.h"

namespace NeguPi {

  uint8_t PiFace::useCount_ = 0;

  PiFace::PiFace(uint8_t hw_addr, uint8_t bus, uint8_t cs)
  :hw_addr_(hw_addr)
  {
    mcp23s17_ = new MCP23S17_t(bus, cs);
  }

  PiFace::~PiFace()
  {
    mcp23s17_->close();
    delete mcp23s17_;
    useCount_--;
  }

  bool PiFace::init()
  {
    mcp23s17_->open();

    if (useCount_==0) {

      const uint8_t ioconfig = VMCP23S17::BANK_OFF |
          VMCP23S17::INT_MIRROR_OFF |
          VMCP23S17::SEQOP_OFF |
          VMCP23S17::DISSLW_OFF |
          VMCP23S17::HAEN_ON |
          VMCP23S17::ODR_OFF |
          VMCP23S17::INTPOL_LOW;
      mcp23s17_->writeRegister(VMCP23S17::IOCON, hw_addr_, ioconfig);

      // I/O direction
      mcp23s17_->writeRegister(VMCP23S17::IODIRA, hw_addr_, 0x00);
      mcp23s17_->writeRegister(VMCP23S17::IODIRB, hw_addr_, 0xff);

      // GPIOB pull ups
      mcp23s17_->writeRegister(VMCP23S17::GPPUB, hw_addr_, 0xff);

      // enable interrupts
      mcp23s17_->writeRegister(VMCP23S17::GPINTENB, hw_addr_, 0xff);

      useCount_++;
    }

    return true;
  }

  uint8_t PiFace::readRegister(uint8_t reg)
  {
    return mcp23s17_->readRegister(reg, hw_addr_);
  }

  void PiFace::writeRegister(uint8_t reg, uint8_t data)
  {
    mcp23s17_->writeRegister(reg, hw_addr_, data);
  }

  uint8_t PiFace::readBit(uint8_t bit, uint8_t reg)
  {
    return mcp23s17_->readBit(bit, reg, hw_addr_);
  }

  void PiFace::writeBit(uint8_t bit, uint8_t reg, uint8_t data)
  {
    mcp23s17_->writeBit(bit, reg, hw_addr_, data);
  }

};
