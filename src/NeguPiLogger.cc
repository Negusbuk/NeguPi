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

#include <syslog.h>

#include <iostream>

#include "NeguPiLogger.h"

namespace NeguPi {

  Log::Log()
  {

  }

  Log::~Log()
  {
    Logger::instance()->write(stream_.str());
  }

  Logger* Logger::instance_ = NULL;

  Logger::Logger(bool daemonized)
  :daemonized_(daemonized)
  {

  }

  Logger* Logger::instance(bool daemonized)
  {
    if (instance_==NULL) {
      instance_ = new Logger(daemonized);
    }

    return instance_;
  }
  
  void Logger::write(const std::string& text)
  {
    if (!daemonized_) {
      std::cout << text << std::endl;
    } else {
      syslog(LOG_NOTICE, "%s", text.c_str());
    }
  }

};
