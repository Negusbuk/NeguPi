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

class CameraBox : public NeguPi::PiFaceStateMachine
{
public:

  CameraBox(NeguPi::PiFace* piface);

  void input0Changed(uint8_t state);
  void input2Changed(uint8_t state);
  void input3Changed(uint8_t state);
  void heartBeat(int milliseconds);

protected:

  void checkImageOutputDirectory();
  void checkClipOutputDirectory();
  void readImageOptions();
  void readConfig();

  std::string outputImageDir_;
  std::string outputClipDir_;
  int timeoutImage_;
  int timeoutClip_;

  imageOptions imageOptions_;

  std::vector<std::string> previewArgsVector_;
  char** previewArgs_;

  std::vector<std::string> imageArgsVector_;
  char** imageArgs_;

  int nClipArgs_;
  char** clipArgs_;
  bool inImageLoop_;
  bool inClipLoop_;
  int delayImage_;
  int delayClip_;
  int imageLoopCount_;
  int imageCount_;
  int clipLoopCount_;
  int clipCount_;
};
