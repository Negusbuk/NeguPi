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

#include <iostream>

#include "NeguPiNMEA.h"

namespace NeguPi {

  GPSLocation::GPSLocation()
  : valid_(false),
    latitude_(0.),
    longitude_(0.)
  {

  }

  GPSDate::GPSDate()
  : valid_(false),
    day_(0),
    month_(0),
    year_(0)
  {

  }

  GPSTime::GPSTime()
  : valid_(false),
    hour_(0),
    minute_(0),
    second_(0.)
  {

  }

  GPSIntegerValue::GPSIntegerValue()
  : valid_(false),
    value_(0)
  {

  }

  GPSDoubleValue::GPSDoubleValue()
  : valid_(false),
    value_(0.)
  {

  }

  NMEA::NMEA()
  : buffer_(""),
    sizeOfBuffer_(0),
    startOfSentence_(-1),
    endOfSentence_(-1)
  {

  }

  bool NMEA::process(const char c)
  {
    bool retval = false;

    buffer_ += c;

    switch(c) {
    case '$':
      startOfSentence_ = sizeOfBuffer_;
      break;
    case '*':
      endOfSentence_ = sizeOfBuffer_;
      break;
    default:
      break;
    }

    sizeOfBuffer_ = buffer_.length();

    if (startOfSentence_==-1 &&
        endOfSentence_!=-1 &&
        sizeOfBuffer_ >= endOfSentence_ + 3) {
      buffer_.erase(0, endOfSentence_+3);
      startOfSentence_ = -1;
      endOfSentence_ = -1;
      sizeOfBuffer_ = buffer_.length();
    }

    if (startOfSentence_!=-1 &&
        endOfSentence_!=-1 &&
        endOfSentence_ > startOfSentence_ &&
        sizeOfBuffer_ >= endOfSentence_ + 3) {

      std::string sentence = buffer_.substr(startOfSentence_+1, endOfSentence_-1);
      uint8_t checksum = std::stoi(buffer_.substr(endOfSentence_+1,
                                                  endOfSentence_+2),
                                   0, 0x10);

      // std::cout << sentence << " " << (int)checksum << std::endl;

      if (testChecksum(sentence, checksum)) {
        retval = processSentence(sentence);
      }

      buffer_.erase(0, endOfSentence_+3);
      startOfSentence_ = -1;
      endOfSentence_ = -1;
      sizeOfBuffer_ = buffer_.length();
    }

    return retval;
  }

  bool NMEA::processSentence(std::string & sentence)
  {
    size_t idx = sentence.find(',');

    std::string type = sentence.substr(0, idx);
    sentence.erase(0, idx+1);

    // std::cout << sentence << std::endl;

    if (type=="GPRMC") return processGPRMCSentence(sentence);
    if (type=="GPGGA") return processGPGGASentence(sentence);

    return false;
  }

  bool NMEA::processGPRMCSentence(std::string & sentence)
  {
    size_t idx;
    std::string sub;

    // parse time

    int hour, minute;
    float second;

    idx = sentence.find(',');

    hour = std::stoi(sentence.substr(0, 2));
    minute = std::stoi(sentence.substr(2, 2));
    second = std::stof(sentence.substr(4, idx-4));

    sentence.erase(0, idx+1);

    // parse status

    bool fix = false;

    idx = sentence.find(',');

    if (sentence[0]=='A') fix = true;

    sentence.erase(0, idx+1);

    // parse latitude

    double latitude = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()>0) {
      latitude = std::stod(sub) / 100.;
    }

    sentence.erase(0, idx+1);

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()==1 && sub[0]=='S') {
      latitude *= -1.0;
    }

    sentence.erase(0, idx+1);

    // parse longitude

    double longitude = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()>0) {
      longitude = std::stod(sub) / 100.;
    }

    sentence.erase(0, idx+1);

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()==1 && sub[0]=='W') {
      longitude *= -1.0;
    }

    sentence.erase(0, idx+1);

    // parse speed

    double speed = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()>0) {
      speed = std::stod(sub);
    }

    sentence.erase(0, idx+1);

    // parse course

    double course = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()>0) {
      course = std::stod(sub);
    }

    sentence.erase(0, idx+1);

    // parse date

    int day, month, year;

    idx = sentence.find(',');

    day = std::stoi(sentence.substr(0, 2));
    month = std::stoi(sentence.substr(2, 2));
    year = std::stof(sentence.substr(4, 2)) + 2000;

    sentence.erase(0, idx+1);

    gpsDate_.setValid(fix);
    gpsDate_.setDay(day);
    gpsDate_.setMonth(month);
    gpsDate_.setYear(year);

    gpsTime_.setValid(fix);
    gpsTime_.setHour(hour);
    gpsTime_.setMinute(minute);
    gpsTime_.setSecond(second);

    gpsLocation_.setValid(fix);
    gpsLocation_.setLatitude(latitude);
    gpsLocation_.setLongitude(longitude);

    gpsSpeed_.setValid(fix);
    gpsSpeed_.setValue(speed);

    gpsCourse_.setValid(fix);
    gpsCourse_.setValue(course);

    return fix;
  }

  bool NMEA::processGPGGASentence(std::string & sentence)
  {
    std::cout << sentence << std::endl;

    size_t idx;
    std::string sub;

    // parse time

    int hour, minute;
    float second;

    idx = sentence.find(',');

    hour = std::stoi(sentence.substr(0, 2));
    minute = std::stoi(sentence.substr(2, 2));
    second = std::stof(sentence.substr(4, idx-4));

    sentence.erase(0, idx+1);

    // parse latitude

    double latitude = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()>0) {
      latitude = std::stod(sub) / 100.;
    }

    sentence.erase(0, idx+1);

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()==1 && sub[0]=='S') {
      latitude *= -1.0;
    }

    sentence.erase(0, idx+1);

    // parse longitude

    double longitude = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()>0) {
      longitude = std::stod(sub) / 100.;
    }

    sentence.erase(0, idx+1);

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (sub.length()==1 && sub[0]=='W') {
      longitude *= -1.0;
    }

    sentence.erase(0, idx+1);

    // parse fix

    bool fix = false;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (idx>0) fix = std::stoi(sub);

    sentence.erase(0, idx+1);

    // parse number of satellites

    int satellites = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (idx>0) satellites = std::stoi(sub);
    if (satellites==0) fix = false;

    sentence.erase(0, idx+1);

    // parse HDOP

    double hdop = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (idx>0) hdop = std::stof(sub);

    sentence.erase(0, idx+1);

    // parse altitude

    double altitude = 0;

    idx = sentence.find(',');
    sub = sentence.substr(0, idx);
    if (idx>0) altitude = std::stof(sub);

    sentence.erase(0, idx+1);


    gpsTime_.setValid(fix);
    gpsTime_.setHour(hour);
    gpsTime_.setMinute(minute);
    gpsTime_.setSecond(second);

    gpsLocation_.setValid(fix);
    gpsLocation_.setLatitude(latitude);
    gpsLocation_.setLongitude(longitude);

    gpsSatellites_.setValid(fix);
    gpsSatellites_.setValue(satellites);

    gpsHDOP_.setValid(fix);
    gpsHDOP_.setValue(hdop);

    gpsAltitude_.setValid(fix);
    gpsAltitude_.setValue(altitude);

    return false;
  }

  bool NMEA::testChecksum(const std::string & sentence, uint8_t c)
  {
    uint8_t parity = 0;

    for (auto it=sentence.cbegin(); it!=sentence.cend(); ++it) {
      parity ^= (uint8_t)*it;
    }

    return (parity==c);
  }

};
