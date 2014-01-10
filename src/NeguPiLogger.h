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

#ifndef NEGUPILOGGER_H
#define NEGUPILOGGER_H

#include <iostream>
#include <string>
#include <sstream>

namespace NeguPi {

  class Log
  {
  public:
    Log();
    ~Log();

    inline Log &operator<<(char t) { stream_ << t; return *this; }
    inline Log &operator<<(bool t) { stream_ << (t ? "true" : "false"); return *this; }
    inline Log &operator<<(signed short t) { stream_ << t; return *this; }
    inline Log &operator<<(unsigned short t) { stream_ << t; return *this; }
    inline Log &operator<<(signed int t) { stream_ << t; return *this; }
    inline Log &operator<<(unsigned int t) { stream_ << t; return *this; }
    inline Log &operator<<(signed long t) { stream_ << t; return *this; }
    inline Log &operator<<(unsigned long t) { stream_ << t; return *this; }
    inline Log &operator<<(float t) { stream_ << t; return *this; }
    inline Log &operator<<(double t) { stream_ << t; return *this; }
    inline Log &operator<<(const char* t) { stream_ << t; return *this; }
    inline Log &operator<<(const std::string& t) { stream_ << t; return *this; }
    inline Log &operator<<(const void * t) { stream_ << t; return *this; }

  protected:
    std::stringstream stream_;
    std::string buffer_;
  };

  class Logger
  {
  public:

    static Logger* instance(bool daemonized = false);
    void write(const std::string& message);

  protected:

    Logger(bool daemonized);
    static Logger* instance_;
    bool daemonized_;
  };
  
};

#endif
