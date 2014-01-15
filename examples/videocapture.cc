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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include <NeguPiDaemon.h>
#include <NeguPiLogger.h>

#include <PiFace.h>
#include <PiFaceStateMachine.h>

using namespace NeguPi;

class VideoCapture : public PiFaceStateMachine
{
public:

  VideoCapture(PiFace* piface) :
    PiFaceStateMachine(piface),
    inVideoCapture_(false),
    timeout_(2*1000),
    clipLoopCount_(1),
    clipCount_(1) {

    outputDir_ = "/home/pi/videocapture";

    std::vector<std::string> args;
    args.push_back("raspistill");
    args.push_back("--width"); args.push_back("960");
    args.push_back("--height"); args.push_back("540");
    args.push_back("--timeout"); args.push_back("1");
    args.push_back("--output"); args.push_back("/home/pi/www/preview.jpg");

    previewArgs_ = new char *[args.size() + 1];
    previewArgs_[args.size()] = 0;

    for (uint8_t i=0;i<args.size();++i) {
      previewArgs_[i] = strdup(args.at(i).c_str());
    }

    args.clear();
    args.push_back("videocapture.py");
    args.push_back(outputDir_ + "/clip_XXX_YYYYYYYY.h264 ");

    clipArgs_ = new char *[args.size() + 1];
    clipArgs_[args.size()] = 0;
    nClipArgs_ = args.size();

    for (uint8_t i=0;i<args.size();++i) {
      clipArgs_[i] = strdup(args.at(i).c_str());
    }

  }

  void input0Changed(uint8_t state) {

    if (state==0) {
      Log() << "take new preview picture";

      pid_t child_pid = fork();
      if (child_pid == 0) {
        execvp(previewArgs_[0], previewArgs_);
        /* The execvp function returns only if an error occurs.  */
        printf ("an error occurred in execl\n");
        abort();
      }
    }
  }
  
  void input3Changed(uint8_t state) {

    if (state==0) {
      checkOutputDirectory();
      clipCount_ = 1;

      struct stat buffer;
      if (stat("/tmp/videocapture.lck", &buffer) != 0) {
        remove("/tmp/videocapture.lck");
      }

      inVideoCapture_ = true;
      delay_ = 0;
      Log() << "inVideoCapture_ = true";
      Log() << "starting clip sequence " << clipLoopCount_;
    } else {
      inVideoCapture_ = false;
      Log() << "inVideoCapture_ = false";
    }
  }

  void heartBeat(int milliseconds) {

    if (inVideoCapture_) {
      delay_ += milliseconds;

      if (delay_>timeout_) {
        // Log() << "beat " << delay_;

        sprintf(clipArgs_[nClipArgs_-1],
                "%s/clip_%03d_%06d.h264",
                outputDir_.c_str(), clipLoopCount_, clipCount_++);
        
        struct stat buffer;
        if (stat("/tmp/videocapture.lck", &buffer) != 0) {

          Log() << "starting new clip " << clipArgs_[nClipArgs_-1];

          pid_t child_pid = fork();
          if (child_pid == 0) {
            execvp(clipArgs_[0], clipArgs_);
            /* The execvp function returns only if an error occurs.  */
            printf ("an error occurred in execl\n");
            abort();
          }
        }

        delay_ = 0;
      }
    }
  }

  void checkOutputDirectory() {

    clipLoopCount_ = 0;

    DIR* dp = opendir(outputDir_.c_str());
    if (dp == NULL) {
      Log() << "Error(" << errno << ") opening " << outputDir_;
      exit(0);
    }

    struct dirent *dirp;
    std::string filename;
    int loop;
     while ((dirp = readdir(dp))) {
      filename = dirp->d_name;
      if (strncmp(filename.c_str(), "clip_", 5)!=0) continue;

      filename = filename.substr(5, 5+3);
      loop = std::stoi(filename);
      if (loop>clipLoopCount_) clipLoopCount_ = loop;
    }

    clipLoopCount_++;

    closedir( dp );
  }

protected:

  char** previewArgs_;
  int nClipArgs_;
  char** clipArgs_;
  bool inVideoCapture_;
  std::string outputDir_;
  int timeout_;
  int delay_;
  int clipLoopCount_;
  int clipCount_;
};

int main(int argc, char * argv[])
{
  Daemonize("videocapture");
  Logger::instance(true);
  
  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace pf(hw_addr);

  pf.init();

  VideoCapture vc(&pf);

  vc.run();
}
