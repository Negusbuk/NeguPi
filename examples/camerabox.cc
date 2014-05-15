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

#include <libconfig.h++>

#include <NeguPiDaemon.h>
#include <NeguPiLogger.h>

#include <PiFace.h>
#include <PiFaceStateMachine.h>

using namespace libconfig;
using namespace NeguPi;

class CameraBox : public PiFaceStateMachine
{
public:

  CameraBox(PiFace* piface) :
    PiFaceStateMachine(piface),
    inImageLoop_(false),
    inClipLoop_(false),
    delayImage_(0),
    delayClip_(0),
    imageLoopCount_(1),
    imageCount_(1),
    clipLoopCount_(1),
    clipCount_(1) {

    readConfig();

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
    args.push_back("raspistill");
    args.push_back("--width"); args.push_back("960");
    args.push_back("--height"); args.push_back("540");
    args.push_back("--timeout"); args.push_back("1");
    args.push_back("--output"); args.push_back(outputImageDir_ + "/image_XXX_YYYYYYYY.jpg ");

    imageArgs_ = new char *[args.size() + 1];
    imageArgs_[args.size()] = 0;
    nImageArgs_ = args.size();

    for (uint8_t i=0;i<args.size();++i) {
      imageArgs_[i] = strdup(args.at(i).c_str());
    }

    args.clear();
    args.push_back("/home/pi/bin/videocapture.py");
    args.push_back(outputClipDir_ + "/clip_XXX_YYYYYYYY.h264 ");

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

  void input2Changed(uint8_t state) {

    if (state==0 && !inImageLoop_) {
      checkClipOutputDirectory();
      clipCount_ = 1;

      struct stat buffer;
      if (stat("/tmp/videocapture.lck", &buffer) != 0) {
        remove("/tmp/videocapture.lck");
      }

      inClipLoop_ = true;
      delayClip_ = 0;

      Log() << "inClipLoop_ = true";
      Log() << "starting clip sequence " << clipLoopCount_;
    } else {
      inClipLoop_ = false;
      Log() << "inClipLoop_ = false";
    }
  }

  void input3Changed(uint8_t state) {

    if (state==0 && !inClipLoop_) {
      checkImageOutputDirectory();
      imageCount_ = 1;

      inImageLoop_ = true;
      delayImage_ = 0;

      Log() << "inImageLoop_ = true";
      Log() << "starting image sequence " << imageLoopCount_;
    } else {
      inImageLoop_ = false;
      Log() << "inImageLoop_ = false";
    }
  }

  void heartBeat(int milliseconds) {

    if (inImageLoop_) {
      delayImage_ += milliseconds;
      if (delayImage_>timeoutImage_) {

        sprintf(imageArgs_[nImageArgs_-1],
                "%s/image_%03d_%06d.jpg",
                outputImageDir_.c_str(), imageLoopCount_, imageCount_++);

        pid_t child_pid = fork();
        if (child_pid == 0) {
          execvp(imageArgs_[0], imageArgs_);
          /* The execvp function returns only if an error occurs.  */
          printf ("an error occurred in execl\n");
          abort();
        }

        delayImage_ = 0;
      }
    }

    if (inClipLoop_) {
      delayClip_ += milliseconds;
      if (delayClip_>timeoutClip_) {
        
        struct stat buffer;
        if (stat("/tmp/videocapture.lck", &buffer) != 0) {

          sprintf(clipArgs_[nClipArgs_-1],
                  "%s/clip_%03d_%06d.h264",
                  outputClipDir_.c_str(), clipLoopCount_, clipCount_++);

          Log() << "starting new clip " << clipArgs_[nClipArgs_-1];

          pid_t child_pid = fork();
          if (child_pid == 0) {
            execvp(clipArgs_[0], clipArgs_);
            /* The execvp function returns only if an error occurs.  */
            printf ("an error occurred in execl\n");
            abort();
          }
        }

        delayClip_ = 0;
      }
    }
  }

  void checkImageOutputDirectory() {

    imageLoopCount_ = 0;

    DIR* dp = opendir(outputImageDir_.c_str());
    if (dp == NULL) {
      Log() << "Error(" << errno << ") opening " << outputImageDir_;
      exit(0);
    }

    struct dirent *dirp;
    std::string filename;
    int loop;
     while ((dirp = readdir(dp))) {
      filename = dirp->d_name;
      if (strncmp(filename.c_str(), "image_", 6)!=0) continue;

      filename = filename.substr(6, 6+3);
      loop = std::stoi(filename);
      if (loop>imageLoopCount_) imageLoopCount_ = loop;
    }

    imageLoopCount_++;

    closedir( dp );
  }

  void checkClipOutputDirectory() {

    clipLoopCount_ = 0;

    DIR* dp = opendir(outputClipDir_.c_str());
    if (dp == NULL) {
      Log() << "Error(" << errno << ") opening " << outputClipDir_;
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

  void readConfig() {

    bool writeCfg = false;
    std::string homePath = getenv("HOME");
    std::string cfgFile = homePath + "/.NeguPi.cfg";
    Config cfg;
    try {
      cfg.readFile(cfgFile.c_str());
    }
    catch (const FileIOException &fioex) {
      std::cerr << "NeguPi: I/O error while reading config file " << cfgFile << std::endl;
      writeCfg = true;
    }
    catch (const ParseException &pex) {
      std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
          << " - " << pex.getError() << std::endl;
      writeCfg = true;
    }
    catch (...) {
      writeCfg = true;
    }

    Setting &root = cfg.getRoot();
    if (!root.exists("camerabox")) {
      root.add("camerabox", Setting::TypeGroup);
      writeCfg = true;
    }
    Setting &settings = root["camerabox"];

    if (!settings.lookupValue("imageDir", outputImageDir_)) {
      outputImageDir_ = homePath + "/timelapse";
      settings.add("imageDir", Setting::TypeString) = outputImageDir_.c_str();
      writeCfg = true;
    }

    if (!settings.lookupValue("clipDir", outputClipDir_)) {
      outputClipDir_ = homePath + "/videocapture";
      settings.add("clipDir", Setting::TypeString) = outputClipDir_.c_str();
      writeCfg = true;
    }

    if (!settings.lookupValue("timeoutImage", timeoutImage_)) {
      timeoutImage_ = 2*1000;
      settings.add("timeoutImage", Setting::TypeInt) = timeoutImage_;
      writeCfg = true;
    }

    if (!settings.lookupValue("timeoutClip", timeoutClip_)) {
      timeoutClip_ = 2*1000;
      settings.add("timeoutClip", Setting::TypeInt) = timeoutClip_;
      writeCfg = true;
    }

    if (writeCfg) {
      try {
        cfg.writeFile(cfgFile.c_str());
        std::cerr << "NeguPi: configuration successfully written to " << cfgFile << std::endl;
      }
      catch (const FileIOException &fioex) {
        std::cerr << "NeguPi: I/O error while writing config file " << cfgFile << std::endl;
      }
      catch (...) {

      }
    }
  }

  std::string outputImageDir_;
  std::string outputClipDir_;
  int timeoutImage_;
  int timeoutClip_;

  char** previewArgs_;
  int nImageArgs_;
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

int main(int argc, char * argv[])
{
  Daemonize("camerabox");
  Logger::instance(true);

  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace pf(hw_addr);

  pf.init();

  CameraBox cb(&pf);

  cb.run();
}
