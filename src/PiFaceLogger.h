#ifndef PIFACELOGGER_H
#define PIFACELOGGER_H

#include <iostream>
#include <string>
#include <sstream>

class PiFaceLog
{
public:
  PiFaceLog();
  ~PiFaceLog();

    inline PiFaceLog &operator<<(char t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(bool t) { stream_ << (t ? "true" : "false"); return *this; }
    inline PiFaceLog &operator<<(signed short t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(unsigned short t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(signed int t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(unsigned int t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(signed long t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(unsigned long t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(float t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(double t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(const char* t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(const std::string& t) { stream_ << t; return *this; }
    inline PiFaceLog &operator<<(const void * t) { stream_ << t; return *this; }

protected:
  std::stringstream stream_;
  std::string buffer_;
};

class PiFaceLogger
{
  public:
  
  static PiFaceLogger* instance(bool daemonized = false);
  void write(const std::string& message);
  
  protected:
  
  PiFaceLogger(bool daemonized);
  static PiFaceLogger* instance_;
  bool daemonized_;
};

#endif
