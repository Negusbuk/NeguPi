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

#ifndef VMCP23S17_H
#define VMCP23S17_H

#include <cstdint>

class VMCP23S17
{
public:

  enum MCP23S17_CMD {
    WRITE = 0x0,
    READ  = 0x1
  };

  enum MCP23S17_REG {
    IODIRA     = 0x00,  // I/O direction A
    IODIRB     = 0x01,  // I/O direction B
    IPOLA      = 0x02,  // I/O polarity A
    IPOLB      = 0x03,  // I/O polarity B
    GPINTENA   = 0x04,  // interupt enable A
    GPINTENB   = 0x05,  // interupt enable B
    DEFVALA    = 0x06,  // register default value A (interupts)
    DEFVALB    = 0x07,  // register default value B (interupts)
    INTCONA    = 0x08,  // interupt control A
    INTCONB    = 0x09,  // interupt control B
    IOCON      = 0x0A,  // I/O config (also 0x0B)
    GPPUA      = 0x0C,  // port A pullups
    GPPUB      = 0x0D,  // port B pullups
    INTFA      = 0x0E,  // interupt flag A (where the interupt came from)
    INTFB      = 0x0F,  // interupt flag B
    INTCAPA    = 0x10,  // interupt capture A (value at interupt is saved here)
    INTCAPB    = 0x11,  // interupt capture B
    GPIOA      = 0x12,  // port A
    GPIOB      = 0x13,  // port B
    OLATA      = 0x14,  // output latch A
    OLATB      = 0x15,  // output latch B
    NREGISTERS
  };

  enum MCP23S17_IO {
    BANK_OFF        = 0x00,  // addressing mode
    BANK_ON         = 0x80,
    INT_MIRROR_ON   = 0x40,  // interupt mirror (INTa|INTb)
    INT_MIRROR_OFF  = 0x00,
    SEQOP_OFF       = 0x20,  // incrementing address pointer
    SEQOP_ON        = 0x00,
    DISSLW_ON       = 0x10,  // slew rate
    DISSLW_OFF      = 0x00,
    HAEN_ON         = 0x08,  // hardware addressing
    HAEN_OFF        = 0x00,
    ODR_ON          = 0x04,  // open drain for interupts
    ODR_OFF         = 0x00,
    INTPOL_HIGH     = 0x02,  // interupt polarity
    INTPOL_LOW      = 0x00
  };

  explicit VMCP23S17(uint8_t bus, uint8_t cs);
  virtual ~VMCP23S17() { }
  
  virtual bool open() = 0;
  virtual bool close() = 0;

  virtual uint8_t readRegister(MCP23S17_REG reg, uint8_t hw_addr) = 0;
  virtual void writeRegister(MCP23S17_REG reg, uint8_t hw_addr, uint8_t data) = 0;

  virtual uint8_t readBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr) = 0;
  virtual void writeBit(uint8_t bit, MCP23S17_REG reg, uint8_t hw_addr, uint8_t data) = 0;

protected:

  uint8_t bus_;
  uint8_t cs_;
};

#endif // VMCP23S17_H
