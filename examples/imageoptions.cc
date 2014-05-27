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
#include <fstream>
#include <cstdlib>

#include <libconfig.h++>

#include <NeguPiLogger.h>

#include "imageoptions.h"

using namespace libconfig;
using namespace NeguPi;

imageOptions::imageOptions()
{
  bool writeCfg = false;
  std::string homePath = getenv("HOME");
  std::string cfgFile = homePath + "/.NeguPi.cfg";
  Config cfg;
  try {
    cfg.readFile(cfgFile.c_str());
  }
  catch (const FileIOException &fioex) {
    Log() << "NeguPi: I/O error while reading config file " << cfgFile;
    writeCfg = true;
  }
  catch (const ParseException &pex) {
    Log() << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                      << " - " << pex.getError();
    writeCfg = true;
  }
  catch (...) {
    writeCfg = true;
  }

  Setting &root = cfg.getRoot();
  if (!root.exists("imageoptions")) {
    root.add("imageoptions", Setting::TypeGroup);
    writeCfg = true;
  }
  Setting &settings = root["imageoptions"];

  if (!settings.lookupValue("imageDir", filename_)) {
    filename_ = homePath + "/www/setup.txt";
    settings.add("imageDir", Setting::TypeString) = filename_.c_str();
    writeCfg = true;
  }

  if (writeCfg) {
    try {
      cfg.writeFile(cfgFile.c_str());
      Log() << "NeguPi: configuration successfully written to " << cfgFile;
    }
    catch (const FileIOException &fioex) {
      Log() << "NeguPi: I/O error while writing config file " << cfgFile;
    }
    catch (...) {

    }
  }

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
  std::ifstream ifile(filename_);
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
