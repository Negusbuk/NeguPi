/****************************************************************************
 **
 **  Copyright (C) 2013, 2019 Andreas Mussgiller
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

#include "NeguPiMillis.h"

namespace NeguPi {

  Millis* Millis::instance_ = NULL;
  std::chrono::time_point<std::chrono::high_resolution_clock> Millis::initialTP_;

  Millis::Millis()
  {

  }

  Millis* Millis::instance()
  {
    if (instance_==NULL) {
      instance_ = new Millis();
      initialTP_ = std::chrono::high_resolution_clock::now();
    }

    return instance_;
  }
  
  uint32_t Millis::get()
  {
	  auto TP = std::chrono::high_resolution_clock::now();
	  auto elapsed = TP - initialTP_;

	  return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
 }

};
