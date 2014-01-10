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

#include <NeguPiDaemon.h>
#include <NeguPiLogger.h>

#include <PiFace.h>
#include <PiFaceStateMachine.h>

using namespace NeguPi;

class StateMachine : public PiFaceStateMachine
{
public:
  StateMachine(PiFace* piface) :
    PiFaceStateMachine(piface) {
  }

  void input0Changed(uint8_t state) {
    Log() << "void input0Changed(uint8_t state) " << (int)state;

    if (state==0) {
      uint8_t bit = piface_->readBit(7, PiFace::Output);
      if (bit==0) {
        piface_->writeBit(7, PiFace::Output, 1);
      } else {
        piface_->writeBit(7, PiFace::Output, 0);
      }
    }
  }
  
  void input1Changed(uint8_t state) {
    Log() << "void input1Changed(uint8_t state) " << (int)state;

    if (state==0) {
      uint8_t bit = piface_->readBit(6, PiFace::Output);
      if (bit==0) {
        piface_->writeBit(6, PiFace::Output, 1);
      } else {
        piface_->writeBit(6, PiFace::Output, 0);
      }
    }
  }

  void input2Changed(uint8_t state) {
    Log() << "void input2Changed(uint8_t state) " << (int)state;

    if (state==0) {
      piface_->writeBit(5, PiFace::Output, 1);
    } else {
      piface_->writeBit(5, PiFace::Output, 0);
    }
  }

  void input3Changed(uint8_t state) {
    Log() << "void input3Changed(uint8_t state) " << (int)state;

    if (state==0) {
      piface_->writeBit(4, PiFace::Output, 1);
    } else {
      piface_->writeBit(4, PiFace::Output, 0);
    }
  }
};

int main(int argc, char * argv[])
{
  Daemonize("statemachine");
  Logger::instance(true);
  
  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace pf(hw_addr);

  pf.init();

  StateMachine sm(&pf);

  sm.run();
}
