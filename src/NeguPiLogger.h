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

    enum LogLevel {
        Debug       = 0,
        Spam        = 1,
        Message     = 2,
        Warning     = 3,
        Critical    = 4,
        Fatal       = 5
    };

    Log(LogLevel level = Message);
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

    LogLevel logLevel_;
    std::stringstream stream_;
    std::string buffer_;
  };

  class LogD : public Log
  {
  public:
    LogD() : Log(Log::Debug) { }
    ~LogD() {}
  };

  class LogS : public Log
  {
  public:
    LogS() : Log(Log::Spam) { }
    ~LogS() {}
  };

  class LogM : public Log
  {
  public:
    LogM() : Log(Log::Message) { }
    ~LogM() {}
  };

  class LogW : public Log
  {
  public:
    LogW() : Log(Log::Warning) { }
    ~LogW() {}
  };

  class LogC : public Log
  {
  public:
    LogC() : Log(Log::Critical) { }
    ~LogC() {}
  };

  class LogF : public Log
  {
  public:
    LogF() : Log(Log::Fatal) { }
    ~LogF() {}
  };

  class Logger
  {
  public:

    static Logger* instance(Log::LogLevel logLevel = Log::Message,
    		                bool daemonized = false);
    void write(Log::LogLevel level, const std::string& message);

  protected:

    Logger(Log::LogLevel level, bool daemonized);
    static Logger* instance_;
    Log::LogLevel logLevel_;
    bool daemonized_;
  };
  
};

#endif
