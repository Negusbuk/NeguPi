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

#ifndef NODEVICE
#include <mcp23s17.h>
#endif

#include "PiFaceOrg.h"

uint8_t PiFaceOrg::usecount_ = 0;
int PiFaceOrg::mcp23s17_fd_ = 0;

PiFaceOrg::PiFaceOrg(uint8_t hw_addr)
:VPiFace(hw_addr)
{

}

PiFaceOrg::~PiFaceOrg()
{
  if (usecount_>0) {

    usecount_--;

#ifndef NODEVICE
    if (usecount_==0) { 
      const uint8_t intenb = mcp23s17_read_reg(GPINTENB, hw_addr_, mcp23s17_fd_);
      if (intenb) {
        mcp23s17_write_reg(0, GPINTENB, hw_addr_, mcp23s17_fd_);
        // now do some other interrupt stuff...
        // TODO
      }

      close(mcp23s17_fd_);
      mcp23s17_fd_ = 0;
    }
#endif
  }
}

bool PiFaceOrg::init()
{
  if (usecount_==0) {
#ifndef NODEVICE
    if ((mcp23s17_fd_ = mcp23s17_open(0, 0)) < 0) {
      return false;
    }
#endif
  }
  usecount_++;

#ifndef NODEVICE
  const uint8_t ioconfig = BANK_OFF |
      INT_MIRROR_OFF | \
      SEQOP_OFF | \
      DISSLW_OFF | \
      HAEN_ON | \
      ODR_OFF | \
      INTPOL_LOW;
  mcp23s17_write_reg(ioconfig, IOCON, hw_addr_, mcp23s17_fd_);

  // I/O direction
  mcp23s17_write_reg(0x00, IODIRA, hw_addr_, mcp23s17_fd_);
  mcp23s17_write_reg(0xff, IODIRB, hw_addr_, mcp23s17_fd_);

  // GPIOB pull ups
  mcp23s17_write_reg(0xff, GPPUB, hw_addr_, mcp23s17_fd_);

  // enable interrupts
  mcp23s17_write_reg(0xff, GPINTENB, hw_addr_, mcp23s17_fd_);
#endif

  return true;
}

uint8_t PiFaceOrg::readRegister(uint8_t reg)
{
#ifndef NODEVICE
  return mcp23s17_read_reg(reg, hw_addr_, mcp23s17_fd_);
#else
  return 0;
#endif
}

void PiFaceOrg::writeRegister(uint8_t data, uint8_t reg)
{
#ifndef NODEVICE
  mcp23s17_write_reg(data, reg, hw_addr_, mcp23s17_fd_);
#endif
}

uint8_t PiFaceOrg::readBit(uint8_t bit, uint8_t reg)
{
#ifndef NODEVICE
  return mcp23s17_read_bit(bit, reg, hw_addr_, mcp23s17_fd_);
#else
  return 0;
#endif
}

void PiFaceOrg::writeBit(uint8_t data, uint8_t bit, uint8_t reg)
{
#ifndef NODEVICE
  mcp23s17_write_bit(data, bit, reg, hw_addr_, mcp23s17_fd_);
#endif
}
