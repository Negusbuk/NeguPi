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
#include <sstream>
#include <chrono>
#include <iomanip>

#include "NeguPiLogger.h"

namespace NeguPi {

  Log::Log(LogLevel level)
  : logLevel_(level)
  {

  }

  Log::~Log()
  {
    Logger::instance()->write(logLevel_, stream_.str());
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
  
  void Logger::write(Log::LogLevel level, const std::string& message)
  {
    auto now = std::chrono::system_clock::now();
    auto dur = now.time_since_epoch();
    dur -= std::chrono::duration_cast<std::chrono::seconds>(dur);

    auto now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ssTp;

    ssTp << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    ssTp << "." << std::setw(3) << std::setfill('0') << static_cast<unsigned>(dur / std::chrono::milliseconds(1));
    ssTp << std::setfill(' ');
    ssTp << " ";

    switch (level) {
    case Log::Debug: ssTp << "D"; break;
    case Log::Spam: ssTp << "S"; break;
    case Log::Message: ssTp << "M"; break;
    case Log::Warning: ssTp << "W"; break;
    case Log::Critical: ssTp << "C"; break;
    case Log::Fatal: ssTp << "F"; break;
    }

    ssTp << " ";

    if (!daemonized_) {
      std::cout << ssTp.str() << std::endl;
    } else {
      syslog(LOG_NOTICE, "%s", ssTp.str().c_str());
    }
  }

};
