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

#ifndef PIFACESTATEMACHINE_H
#define PIFACESTATEMACHINE_H

#include <functional>

#include <VPiFace.h>

class PiFaceStateMachine
{
public:

  explicit PiFaceStateMachine(VPiFace* piface);
  virtual ~PiFaceStateMachine();
  
  void run();

  virtual void input0Changed(uint8_t state) { }
  virtual void input1Changed(uint8_t state) { }
  virtual void input2Changed(uint8_t state) { }
  virtual void input3Changed(uint8_t state) { }
  virtual void input4Changed(uint8_t state) { }
  virtual void input5Changed(uint8_t state) { }
  virtual void input6Changed(uint8_t state) { }
  virtual void input7Changed(uint8_t state) { }
  
protected:

  VPiFace* piface_;

  uint8_t lastInputState_;
  
  std::function<void(PiFaceStateMachine*, uint8_t)> functions_[8];
  void input0Changed_(uint8_t state) { this->input0Changed(state); }
  void input1Changed_(uint8_t state) { this->input1Changed(state); }
  void input2Changed_(uint8_t state) { this->input2Changed(state); }
  void input3Changed_(uint8_t state) { this->input3Changed(state); }
  void input4Changed_(uint8_t state) { this->input4Changed(state); }
  void input5Changed_(uint8_t state) { this->input5Changed(state); }
  void input6Changed_(uint8_t state) { this->input6Changed(state); }
  void input7Changed_(uint8_t state) { this->input7Changed(state); }
};

#endif // PIFACE_H
