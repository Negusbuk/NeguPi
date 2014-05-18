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

#include <fstream>
#include <cstdlib>

#include "imageoptions.h"

imageOptions::imageOptions()
{
  aISO = "0";
  iISO = 0;
  aEX = "antishake";
  aSS = "0";
  iSS = 0;
  aMM = "matrix";
  aEV = "0";
  aSH = "0";
  aCO = "0";
  aBR = "50";
  aSA = "0";
};

void imageOptions::read()
{
  std::string key, aValue;
  int iValue;
  std::ifstream ifile("/home/pi/www/setup.txt");
  while (ifile >> key >> aValue) {
    iValue = std::atoi(aValue.c_str());
    if (key=="iso") {
      aISO = aValue;
      iISO = iValue;
    }
    if (key=="ex") {
      aEX = aValue;
    }
    if (key=="ss") {
      aSS = aValue;
      iSS = iValue;
    }
    if (key=="mm") {
      aMM = aValue;
    }
    if (key=="ev") {
      aEV = aValue;
    }
    if (key=="sh") {
      aSH = aValue;
    }
    if (key=="co") {
      aCO = aValue;
    }
    if (key=="br") {
      aBR = aValue;
    }
    if (key=="sa") {
      aSA = aValue;
    }
  }
}
