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

#include <PiFace.h>

#include <NeguPiDaemon.h>
#include <NeguPiLogger.h>

using namespace NeguPi;

int main(int argc, char * argv[])
{
  //Daemonize("blink");
  //Logger::instance(true);

  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace pf(hw_addr);

  pf.init();

  while (1){
    pf.writeBit(7, PiFace::Output, 1);
    uint8_t bit = pf.readBit(7, PiFace::Output);
    Log() << "Bit 7 is " << (int)bit;
    usleep(500000);
    pf.writeBit(7, PiFace::Output, 0
                );
    bit = pf.readBit(7, PiFace::Output);
    Log() << "Bit 7 is " << (int)bit;
    usleep(500000);
  }
}
