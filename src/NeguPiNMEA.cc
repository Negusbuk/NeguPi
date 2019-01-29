/****************************************************************************
 **
 **  Copyright (C) 2019 Andreas Mussgiller
 **  Based on and inspired by TinyGPS++ courtesy of Mikal Hart.
 **  "distanceTo" and "courseTo" methods courtesy of Maarten Lamers.
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
#include <cmath>

#include "NeguPiNMEA.h"

namespace NeguPi {

  GPSLocation::GPSLocation()
  : valid_(false),
    latitude_(0.),
    longitude_(0.)
  {

  }

  GPSLocation::GPSLocation(double latitude, double longitude)
  : valid_(true),
    latitude_(latitude),
    longitude_(longitude)
  {

  }

  double GPSLocation::distanceTo(const GPSLocation& other) const
  {
    // returns distance in meters to second location. Uses great-circle
    // distance computation for hypothetical sphere of radius 6372795 meters.
    // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
    // Courtesy of Maarten Lamers

    double delta = (this->longitude_ - other.longitude_) * M_PI / 180.;
    double sinDeltaLongitude = std::sin(delta);
    double cosDeltaLongitude = std::cos(delta);

    double thisLatitude = this->latitude_ * M_PI / 180.;
    double otherLatitude = other.latitude_ * M_PI / 180.;
    double sinThisLatitude = std::sin(thisLatitude);
    double cosThisLatitude = std::cos(thisLatitude);
    double sinOtherLatitude = std::sin(otherLatitude);
    double cosOtherLatitude = std::cos(otherLatitude);

    delta = (cosThisLatitude * sinOtherLatitude) - (sinThisLatitude * cosOtherLatitude * cosDeltaLongitude);
    delta = std::pow(delta, 2);
    delta += std::pow(cosOtherLatitude * sinDeltaLongitude, 2);
    delta = std::sqrt(delta);

    double denom = (sinThisLatitude * sinOtherLatitude) + (cosThisLatitude * cosOtherLatitude * cosDeltaLongitude);

    delta = std::atan2(delta, denom);

    return delta * 6372795.;
  }

  double GPSLocation::courseTo(const GPSLocation& other) const
  {
    // returns course in degrees (North=0, East=90, South=180, West=270)
    // to other position. Because Earth is no exact sphere, calculated
    // course may be off by a tiny fraction.
    // Courtesy of Maarten Lamers

    double delta = (other.longitude_ - this->longitude_) * M_PI / 180.;

    double thisLatitude = this->latitude_ * M_PI / 180.;
    double otherLatitude = other.latitude_ * M_PI / 180.;

    double a1 = std::sin(delta) * std::cos(otherLatitude);
    double a2 = std::sin(thisLatitude) * std::cos(otherLatitude) * std::cos(delta);

    a2 = std::cos(thisLatitude) * std::sin(otherLatitude) - a2;
    a2 = std::atan2(a1, a2);

    if (a2 < 0.) a2 += 2.0 * M_PI;

    return a2 * 180 / M_PI;
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

    switch(c) {
    case '$':
      startOfSentence_ = sizeOfBuffer_;
      break;
    case '*':
      endOfSentence_ = sizeOfBuffer_;
      break;
    case '\r':
    case '\n':
      return retval;
      break;
    default:
      break;
    }

    buffer_ += c;
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

    // std::cout << "processSentence() " << sentence << std::endl;

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
    // std::cout << sentence << std::endl;

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
