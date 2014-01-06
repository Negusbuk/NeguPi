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

