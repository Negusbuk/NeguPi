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

#include <iostream>
#include <chrono>
#include <thread>

#include "NeguPiLogger.h"
#include "PiFaceStateMachine.h"

namespace NeguPi {
  
  PiFaceStateMachine::PiFaceStateMachine(PiFace* piface)
  :piface_(piface),
   timeout_(250)
  {
    lastInputState_ = piface_->readRegister(PiFace::Input);

    functions_[0] = &PiFaceStateMachine::input0Changed_;
    functions_[1] = &PiFaceStateMachine::input1Changed_;
    functions_[2] = &PiFaceStateMachine::input2Changed_;
    functions_[3] = &PiFaceStateMachine::input3Changed_;
    functions_[4] = &PiFaceStateMachine::input4Changed_;
    functions_[5] = &PiFaceStateMachine::input5Changed_;
    functions_[6] = &PiFaceStateMachine::input6Changed_;
    functions_[7] = &PiFaceStateMachine::input7Changed_;
  }

  PiFaceStateMachine::~PiFaceStateMachine()
  {

  }
  
  void PiFaceStateMachine::run()
  {
    uint8_t state, bits, masks[8], data;

    uint8_t mask = 1;
    for (uint8_t i=0;i<8;++i) {
      masks[i] = mask;
      mask <<= 1;
    }
    
    auto lastTP = std::chrono::high_resolution_clock::now();
    
    while (1) {

      state = piface_->readRegister(PiFace::Input);
      
      if (state!=lastInputState_) {

        bits = state ^ lastInputState_;

        for (uint8_t i=0;i<8;++i) {

          if (bits & masks[i]) {
            data = (state & masks[i]) ? 1 : 0;
            // PiFaceLog() << "bit " << (int)i << ": " << (int)data;
            functions_[i](this, data);
          }

        }
      }

      lastInputState_ = state;

      std::this_thread::sleep_for( std::chrono::milliseconds(timeout_) );

      auto TP = std::chrono::high_resolution_clock::now();

      auto elapsed = TP - lastTP;
      int millis = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
      if (millis>1000) {
        this->heartBeat(millis);
        lastTP = TP;
      }
    }
  }

};
