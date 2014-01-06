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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#ifdef NODEVICE
#include <PiFaceFake.h>
typedef PiFaceFake PiFace_t;
#else
#include <PiFace.h>
typedef PiFace PiFace_t;
#endif

#include <PiFaceDaemon.h>
#include <PiFaceLogger.h>

int main(int argc, char * argv[])
{
  Daemonize("blink");
  PiFaceLogger::instance(true);

  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace_t pf(hw_addr);

  pf.init();

  while (1){
    pf.writeBit(1, 7, VPiFace::Output);
    uint8_t bit = pf.readBit(7, VPiFace::Output);
    PiFaceLog() << "Bit 7 is " << (int)bit;
    usleep(500000);
    pf.writeBit(0, 7, VPiFace::Output);
    bit = pf.readBit(7, VPiFace::Output);
    PiFaceLog() << "Bit 7 is " << (int)bit;
    usleep(500000);
  }
}
