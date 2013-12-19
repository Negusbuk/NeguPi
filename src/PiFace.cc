#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#ifndef NODEVICE
#include <mcp23s17.h>
#endif

#include "PiFace.h"

uint8_t PiFace::usecount_ = 0;
int PiFace::mcp23s17_fd_ = 0;

PiFace::PiFace(uint8_t hw_addr)
:VPiFace(hw_addr)
{

}

PiFace::~PiFace()
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

bool PiFace::init()
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

uint8_t PiFace::readRegister(uint8_t reg)
{
#ifndef NODEVICE
  return mcp23s17_read_reg(reg, hw_addr_, mcp23s17_fd_);
#else
  return 0;
#endif
}

void PiFace::writeRegister(uint8_t data, uint8_t reg)
{
#ifndef NODEVICE
  mcp23s17_write_reg(data, reg, hw_addr_, mcp23s17_fd_);
#endif
}

uint8_t PiFace::readBit(uint8_t bit, uint8_t reg)
{
#ifndef NODEVICE
  return mcp23s17_read_bit(bit, reg, hw_addr_, mcp23s17_fd_);
#else
  return 0;
#endif
}

void PiFace::writeBit(uint8_t data, uint8_t bit, uint8_t reg)
{
#ifndef NODEVICE
  mcp23s17_write_bit(data, bit, reg, hw_addr_, mcp23s17_fd_);
#endif
}