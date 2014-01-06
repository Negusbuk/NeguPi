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

#include <cstdio>
#include <unistd.h>
#include <iostream>

#include "PiFaceFake.h"

uint8_t PiFaceFake::inputState_ = 0;
uint8_t PiFaceFake::outputState_ = 0;

PiFaceFake::PiFaceFake(uint8_t hw_addr)
:VPiFace(hw_addr)
{

}

PiFaceFake::~PiFaceFake()
{
  std::cout << "PiFaceFake::~PiFaceFake()" << std::endl;
}

bool PiFaceFake::init()
{
  inputState_ = 4;
  outputState_ = 0;

  return true;
}

uint8_t PiFaceFake::readRegister(uint8_t reg)
{
  if (reg==VPiFace::Output)
    return outputState_;
  else
    return inputState_;
}

void PiFaceFake::writeRegister(uint8_t data, uint8_t reg)
{
  if (reg==VPiFace::Output)
    outputState_ = data;
  else
    inputState_ = data;
}

uint8_t PiFaceFake::readBit(uint8_t bit, uint8_t reg)
{
  uint8_t mask = 1<<bit;
  if (reg==VPiFace::Output)
    return ((outputState_&mask) == 0 ? 0 : 1);
  else
    return ((inputState_&mask) == 0 ? 0 : 1);
}

void PiFaceFake::writeBit(uint8_t data, uint8_t bit, uint8_t reg)
{
  uint8_t mask = (1<<bit);

  if (reg==VPiFace::Output) {
    if (data)
      outputState_ |= mask;
    else
      outputState_ &= ~mask;
  } else {
    if (data)
      inputState_ |= mask;
    else
      inputState_ &= ~mask;
  }
}
