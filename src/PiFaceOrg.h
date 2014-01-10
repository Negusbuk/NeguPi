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

#ifndef PIFACEORG_H
#define PIFACEORG_H

#include <cstdint>

#include <VPiFace.h>

class PiFaceOrg : public VPiFace
{
public:

  explicit PiFaceOrg(uint8_t hw_addr);
  virtual ~PiFaceOrg();

  bool init();  

  uint8_t readRegister(uint8_t reg);
  void writeRegister(uint8_t data, uint8_t reg);
  uint8_t readBit(uint8_t bit, uint8_t reg);
  void writeBit(uint8_t data, uint8_t bit, uint8_t reg);
  
protected:

  static uint8_t usecount_;
  static int mcp23s17_fd_;
};

#endif // PIFACE_H
