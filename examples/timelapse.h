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

#include <string>
#include <vector>

#include <PiFaceStateMachine.h>

#include <imageoptions.h>

class TimeLapse : public NeguPi::PiFaceStateMachine
{
public:

  TimeLapse(NeguPi::PiFace* piface);

  void input0Changed(uint8_t state);
  void input3Changed(uint8_t state);
  void heartBeat(int milliseconds);

protected:

  void checkOutputDirectory();
  void readImageOptions();
  void readConfig();

  std::string outputDir_;
  int timeout_;

  imageOptions imageOptions_;

  std::vector<std::string> previewArgsVector_;
  char** previewArgs_;

  std::vector<std::string> imageArgsVector_;
  char** imageArgs_;

  bool inTimeLapse_;
  int delay_;
  int imageLoopCount_;
  int imageCount_;
};
