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

#include "PiFaceLogger.h"

PiFaceLog::PiFaceLog()
{

}

PiFaceLog::~PiFaceLog()
{
  PiFaceLogger::instance()->write(stream_.str());
}

PiFaceLogger* PiFaceLogger::instance_ = NULL;

PiFaceLogger::PiFaceLogger(bool daemonized)
  :daemonized_(daemonized)
{

}

PiFaceLogger* PiFaceLogger::instance(bool daemonized)
{
  if (instance_==NULL) {
    instance_ = new PiFaceLogger(daemonized);
  }
  
  return instance_;
}

void PiFaceLogger::write(const std::string& text)
{
      if (!daemonized_) {
         std::cout << text << std::endl;
      } else {
        syslog(LOG_NOTICE, "%s", text.c_str());
      }
}    

