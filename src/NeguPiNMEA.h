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

#ifndef NEGUPINMEA_H
#define NEGUPINMEA_H

#include <string>

namespace NeguPi {

  class NMEA;

  class GPSLocation
  {
    friend class NMEA;

  public:

    GPSLocation();
    GPSLocation(double latitude, double longitude);

    bool isValid() const { return valid_; }
    double latitude() const { return latitude_; }
    double longitude() const { return longitude_; }

    double distanceTo(const GPSLocation& other) const;
    double courseTo(const GPSLocation& other) const;

  protected:

    void setValid(bool v) { valid_ = v; }
    void setLatitude(double v) { latitude_ = v; }
    void setLongitude(double v) { longitude_ = v; }

    bool valid_;
    double latitude_;
    double longitude_;
  };

  class GPSDate
  {
    friend class NMEA;

  public:

    GPSDate();

    bool isValid() const { return valid_; }
    int day() const { return day_; }
    int month() const { return month_; }
    int year() const { return year_; }

  protected:

    void setValid(bool v) { valid_ = v; }
    void setDay(int v) { day_ = v; }
    void setMonth(int v) { month_ = v; }
    void setYear(int v) { year_ = v; }

    bool valid_;
    int day_;
    int month_;
    int year_;
  };

  class GPSTime
  {
    friend class NMEA;

  public:

    GPSTime();

    bool isValid() const { return valid_; }
    int hour() const { return hour_; }
    int minute() const { return minute_; }
    double second() const { return second_; }

  protected:

    void setValid(bool v) { valid_ = v; }
    void setHour(int v) { hour_ = v; }
    void setMinute(int v) { minute_ = v; }
    void setSecond(double v) { second_ = v; }

    bool valid_;
    int hour_;
    int minute_;
    double second_;
  };

  class GPSIntegerValue
  {
    friend class NMEA;

  public:

    GPSIntegerValue();

    bool isValid() const { return valid_; }
    int value() const { return value_; }

  protected:

    void setValid(bool v) { valid_ = v; }
    void setValue(int v) { value_ = v; }

    bool valid_;
    int value_;
  };

  class GPSDoubleValue
  {
    friend class NMEA;

  public:

    GPSDoubleValue();

    bool isValid() const { return valid_; }
    double value() const { return value_; }

  protected:

    void setValid(bool v) { valid_ = v; }
    void setValue(double v) { value_ = v; }

    bool valid_;
    double value_;
  };

  class GPSSpeed : public GPSDoubleValue
  {
    friend class NMEA;
  public:

    GPSSpeed() { }

    double knots() const { return value(); }
    double mph() const { return 1.15077945 * value(); }
    double mps() const { return 0.51444444 * value(); }
    double kmph() const { return 1.852 * value(); }
  };

  class GPSCourse : public GPSDoubleValue
  {
    friend class NMEA;
  public:

    GPSCourse() { }

    double deg() const { return value(); }
  };

  class GPSAltitude : public GPSDoubleValue
  {
    friend class NMEA;
  public:

    GPSAltitude() { }

    double meters() const { return value(); }
    double miles() const { return 0.00062137112 * value(); }
    double kilometers() const { return 0.001 * value(); }
    double feet() const { return 3.2808399 * value(); }
  };

  class GPSSatellites : public GPSIntegerValue
  {
    friend class NMEA;
  public:

    GPSSatellites() { }

    int count() const { return value(); }
  };

  class NMEA
  {
  public:

    NMEA();

    const GPSDate& date() const { return gpsDate_; }
    const GPSTime& time() const { return gpsTime_; }
    const GPSLocation& location() const { return gpsLocation_; }
    const GPSSpeed& speed() const { return gpsSpeed_; }
    const GPSCourse& course() const { return gpsCourse_; }
    const GPSSatellites& satellites() const { return gpsSatellites_; }
    const GPSDoubleValue& hdop() const { return gpsHDOP_; }
    const GPSAltitude& altitude() const { return gpsAltitude_; }

    bool process(const char c);

  protected:

    bool testChecksum(const std::string & sentence, uint8_t c);

    bool processSentence(std::string & sentence);

    bool processGPRMCSentence(std::string & sentence);
    bool processGPGGASentence(std::string & sentence);

    std::string buffer_;
    size_t sizeOfBuffer_;
    size_t startOfSentence_;
    size_t endOfSentence_;

    GPSDate gpsDate_;
    GPSTime gpsTime_;
    GPSLocation gpsLocation_;
    GPSSpeed gpsSpeed_;
    GPSCourse gpsCourse_;
    GPSSatellites gpsSatellites_;
    GPSDoubleValue gpsHDOP_;
    GPSAltitude gpsAltitude_;
  };
  
};

#endif
