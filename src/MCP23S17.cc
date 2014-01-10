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

#ifndef NODEVICE
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#endif

#include "NeguPiLogger.h"
#include "MCP23S17.h"

namespace NeguPi {

  uint8_t MCP23S17::useCount_ = 0;

  MCP23S17::MCP23S17(uint8_t bus, uint8_t cs)
  :VMCP23S17(bus, cs),
   fd_(-1)
  {

  }

  bool MCP23S17::open()
  {
#ifndef NODEVICE

    if ((fd = open(spidev_[bus][chip_select], O_RDWR)) < 0) {
      Log() << "MCP23S17::open: ERROR Could not open SPI device "
          << spidev[bus][chip_select];
      return false;
    }

    if (useCount_==0) {

      // initialise
      if (ioctl(fd, SPI_IOC_WR_MODE, &spi_mode_) < 0) {
        Log() << "MCP23S17::open: ERROR Could not set SPI mode " << spi_mode_;
        return false;
      }
      if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bpw_) < 0) {
        Log() << "MCP23S17::open: ERROR Could not set SPI bits per word " << spi_bpw_;
        return false;
      }
      if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed_) < 0) {
        Log() << "MCP23S17::open: ERROR Could not set SPI speed " << spi_speed_;
        return false;
      }

      useCount_++;
    }

    fd_ = fd;

    return true;

#else
    return true;
#endif
  }

  bool MCP23S17::close()
  {
#ifndef NODEVICE
    close(fd_);
    useCount_--;
#endif
    return true;
  }

  uint8_t MCP23S17::readRegister(uint8_t reg, uint8_t hw_addr)
  {
#ifndef NODEVICE

    uint8_t control_byte = getSPIControlByte(READ_CMD, hw_addr);
    uint8_t tx_buf[3] = {control_byte, reg, 0};
    uint8_t rx_buf[sizeof tx_buf];

    struct spi_ioc_transfer spi;
    spi.tx_buf = (unsigned long) tx_buf;
    spi.rx_buf = (unsigned long) rx_buf;
    spi.len = sizeof tx_buf;
    spi.delay_usecs = spi_delay;
    spi.speed_hz = spi_speed;
    spi.bits_per_word = spi_bpw;

    // do the SPI transaction
    if ((ioctl(fd_, SPI_IOC_MESSAGE(1), &spi) < 0)) {
      Log() << "MCP23S17::readRegister: There was a error during the SPI transaction";
      return -1;
    }

    // return the data
    return rx_buf[2];

#else
    return 0;
#endif
  }

  void MCP23S17::writeRegister(uint8_t reg, uint8_t hw_addr, uint8_t data)
  {
#ifndef NODEVICE

    uint8_t control_byte = getSPIControlByte(WRITE_CMD, hw_addr);
    uint8_t tx_buf[3] = {control_byte, reg, data};
    uint8_t rx_buf[sizeof tx_buf];

    struct spi_ioc_transfer spi;
    spi.tx_buf = (unsigned long) tx_buf;
    spi.rx_buf = (unsigned long) rx_buf;
    spi.len = sizeof tx_buf;
    spi.delay_usecs = spi_delay;
    spi.speed_hz = spi_speed;
    spi.bits_per_word = spi_bpw;

    // do the SPI transaction
    if ((ioctl(fd_, SPI_IOC_MESSAGE(1), &spi) < 0)) {
      Log() << "MCP23S17::writeRegister: There was a error during the SPI transaction";
    }

#endif
  }

  uint8_t MCP23S17::readBit(uint8_t bit, uint8_t reg, uint8_t hw_addr)
  {
    return (readRegister(reg, hw_addr) >> bit) & 1;
  }

  void MCP23S17::writeBit(uint8_t bit, uint8_t reg, uint8_t hw_addr, uint8_t data)
  {
    uint8_t reg_data = readRegister(reg, hw_addr);
    if (data) {
      reg_data |= 1 << bit; // set
    } else {
      reg_data &= 0xff ^ (1 << bit); // clear
    }

    writeRegister(reg, hw_addr, reg_data);
  }

  /**
   * Returns an SPI control byte.
   *
   * The MCP23S17 is a slave SPI device. The slave address contains four
   * fixed bits (0b0100) and three user-defined hardware address bits
   * (if enabled via IOCON.HAEN; pins A2, A1 and A0) with the
   * read/write command bit filling out the rest of the control byte::
   *
   *     +--------------------+
   *     |0|1|0|0|A2|A1|A0|R/W|
   *     +--------------------+
   *     |fixed  |hw_addr |R/W|
   *     +--------------------+
   *     |7|6|5|4|3 |2 |1 | 0 |
   *     +--------------------+
   *
   */
  uint8_t MCP23S17::getSPIControlByte(VMCP23S17::COMMAND cmd, uint8_t hw_addr)
  {
    hw_addr = (hw_addr << 1) & 0xE;
    uint8_t rw_cmd = cmd;
    rw_cmd &= 1; // just 1 bit long
    return 0x40 | hw_addr | cmd;
  }

};
