/****************************************************************************
 **
 **  Copyright (C) 2013 - 2019 Andreas Mussgiller
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

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <mutex>

#include <NeguPiDaemon.h>
#include <NeguPiLogger.h>
#include <NeguPiMillis.h>
#include <NeguPi3D.h>

using namespace NeguPi;

class MPU6050Data
{
public:

  MPU6050Data() {
    clear();
  }

  void clear() {
    systemMillis_ = 0;
    mpuMillis_ = 0;
  }

public:

  int32_t systemMillis_;
  int32_t mpuMillis_;
  Vector3D<double> acceleration_;
  Vector3D<double> gravity_;
  Vector3D<double> euler_;
};

MPU6050Data data;

uint32_t theMillis;

void read_mpu(int mpufd)
{
  std::mutex mutex;

  char b;
  std::string buffer;
  std::size_t sizeOfBuffer = 0;
  std::size_t start = std::string::npos;
  std::size_t end = std::string::npos;
  uint32_t startMillis;

  std::size_t idx;

  while (1) {

    while (read(mpufd, &b, 1)==1) {

      switch (b) {
      case '\n': continue;
      case '\r': continue;
      case '$': start = sizeOfBuffer; startMillis = NeguPi::Millis::instance()->get(); break;
      case '*': end = sizeOfBuffer; break;
      default:
        break;
      }

      buffer += b;
      sizeOfBuffer = buffer.length();

      if (start==std::string::npos &&
          end!=std::string::npos &&
          sizeOfBuffer >= end) {
        buffer.erase(0, end+1);
        start = std::string::npos;
        end = std::string::npos;
        sizeOfBuffer = buffer.length();
      }

      if (start!=std::string::npos &&
          end!=std::string::npos &&
          end > start &&
          sizeOfBuffer >= end) {

        std::string sentence = buffer.substr(start+1, end-1);

        std::lock_guard<std::mutex> guard(mutex);

        data.systemMillis_ = startMillis - theMillis;

        idx = sentence.find(',');
        int32_t mpuMillis = std::stol(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.mpuMillis_ = mpuMillis;

        idx = sentence.find(',');
        double temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.acceleration_.x() = temp;

        idx = sentence.find(',');
        temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.acceleration_.y() = temp;

        idx = sentence.find(',');
        temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.acceleration_.z() = temp;

        idx = sentence.find(',');
        temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.gravity_.x() = temp;

        idx = sentence.find(',');
        temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.gravity_.y() = temp;

        idx = sentence.find(',');
        temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.gravity_.z() = temp;

        idx = sentence.find(',');
        temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.euler_.x() = temp;

        idx = sentence.find(',');
        temp = std::stod(sentence.substr(0, idx));
        sentence.erase(0, idx+1);
        data.euler_.y() = temp;

        temp = std::stod(sentence);
        sentence.erase(0, idx+1);
        data.euler_.z() = temp;

        std::cout << data.mpuMillis_ - data.systemMillis_ << " " << data.euler_.z() << std::endl;

        buffer.erase(0, end+1);
        start = std::string::npos;
        end = std::string::npos;
        sizeOfBuffer = buffer.length();
      }
    }
  }
}

int open_UART()
{
  LogM() << "open_UART";

  int mpufd = open("/dev/tty.usbserial-A104WN3B", O_RDWR | O_NOCTTY| O_NDELAY);		//Open in non blocking read/write mode
  if (mpufd == -1) {
    LogF() << "Error - Unable to open UART.  Ensure it is not in use by another application";
  } else {
    LogM() << "open";
  }

  //CONFIGURE THE UART
  //The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
  //	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
  //	CSIZE:- CS5, CS6, CS7, CS8
  //	CLOCAL - Ignore modem status lines
  //	CREAD - Enable receiver
  //	IGNPAR = Ignore characters with parity errors
  //	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
  //	PARENB - Parity enable
  //	PARODD - Odd parity (else even)
  struct termios attribs;

  /* get the current settings */
  tcgetattr(mpufd, &attribs);

  /* set the baudrate */
  cfsetospeed(&attribs, B115200); /* outut baudrate */
  cfsetispeed(&attribs, B115200); /* input baudrate */

  /* if there is need for it, set other settings here */

  /* eventually apply everything for your serialfd descriptor */
  tcsetattr(mpufd, TCSANOW, &attribs);

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  tcflush(mpufd, TCIFLUSH);

  return mpufd;
}

int main(int argc, char * argv[])
{
  Logger::instance(Log::Message, false);

  NeguPi::Millis::instance();
  theMillis = NeguPi::Millis::instance()->get();

  int mpufd = open_UART();
  if (mpufd==-1) {
    return EXIT_FAILURE;
  }

  std::thread mpu_thread(read_mpu, mpufd);

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  close(mpufd);

  return EXIT_SUCCESS;
}
