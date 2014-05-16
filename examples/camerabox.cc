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
#include <sys/wait.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
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

    Config conf;
    try {
      conf.readFile("/home/pi/.NeguPi");
    }
    catch (FileIOException &fioex) {
      std::cerr << "NeguPi: I/O error while reading config file." << std::endl;
    }
    catch (ParseException &pex) {
      std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                << " - " << pex.getError() << std::endl;
    }

    imageOptions_["ISO"] = "100";
    imageOptions_["awb"] = "off";
    imageOptions_["mm"] = "average";
    imageOptions_["ex"] = "auto";
    //imageOptions_["-ss"] = "100000";

    previewArgsVector_.push_back("raspistill");
    previewArgsVector_.push_back("--width"); previewArgsVector_.push_back("960");
    previewArgsVector_.push_back("--height"); previewArgsVector_.push_back("540");
    previewArgsVector_.push_back("--timeout"); previewArgsVector_.push_back("1");
    previewArgsVector_.push_back("--output"); previewArgsVector_.push_back("/home/pi/www/preview.jpg");

    previewArgs_ = new char *[previewArgsVector_.size() + 2*imageOptions_.size() + 1];
    previewArgs_[previewArgsVector_.size() + 2*imageOptions_.size()] = 0;

    for (uint8_t i=0;i<previewArgsVector_.size();++i) {
      previewArgs_[i] = strdup(previewArgsVector_.at(i).c_str());
    }

    imageArgsVector_.push_back("raspistill");
    imageArgsVector_.push_back("--width"); imageArgsVector_.push_back("960" /*"1280"*/);
    imageArgsVector_.push_back("--height"); imageArgsVector_.push_back("540" /*"720"*/);
    imageArgsVector_.push_back("--timeout"); imageArgsVector_.push_back("1");
    //args.push_back("--ISO"); args.push_back("100");
    //args.push_back("--ev"); args.push_back("0");
    //args.push_back("--exposure"); args.push_back("fixedfps");
    //args.push_back("--awb"); args.push_back("off");
    //args.push_back("--metering"); args.push_back("matrix");
    //args.push_back("--shutter"); args.push_back("10000");
    imageArgsVector_.push_back("--output"); imageArgsVector_.push_back(outputImageDir_ + "/image_XXX_YYYYYYYY.jpg ");

    imageArgs_ = new char *[imageArgsVector_.size() + 2*imageOptions_.size() + 1];
    imageArgs_[imageArgsVector_.size() + 2*imageOptions_.size()] = 0;
    nImageArgs_ = imageArgsVector_.size();

    for (uint8_t i=0;i<imageArgsVector_.size();++i) {
      imageArgs_[i] = strdup(imageArgsVector_.at(i).c_str());
    }

    readImageOptions();

    std::vector<std::string> args;
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

      readImageOptions();

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
        } else if (child_pid > 0) {
          int status;
          waitpid(child_pid, &status, 0);
        }

#ifndef NODEVICE
        cv::Mat image = cv::imread(imageArgs_[nImageArgs_-1], 1);
        cv::Mat gray_image;
        cv::cvtColor(image, gray_image, CV_BGR2GRAY);

        int bins = 256;
        int histSize[] = { bins };
        // Set ranges for histogram bins
        float lranges[] = { 0, 256 };
        const float* ranges[] = { lranges };
        // create matrix for histogram
        cv::Mat hist;
        int channels[] = { 0 };
        float sum = 0;

        cv::calcHist(&gray_image, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);
        for (int b = 0;b<bins;b++) {
          sum += hist.at<float>(b);
        }
        sum /= (gray_image.rows*gray_image.cols);
        Log() << "brightness: " << sum;
#endif

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

  void readImageOptions() {

    std::string key, value;
    std::ifstream ifile("/home/pi/www/setup.txt");
    while (ifile >> key >> value) {
      std::map<std::string,std::string>::iterator it = imageOptions_.find(key);
      if (it!=imageOptions_.end()) {
        it->second = value;
      }
    }

    uint8_t i=previewArgsVector_.size();
    for (std::map<std::string,std::string>::iterator it = imageOptions_.begin();
         it!=imageOptions_.end();
         ++it) {
      std::string key = std::string("-") + it->first.c_str();
      previewArgs_[i++] = strdup(key.c_str());
      previewArgs_[i++] = strdup(it->second.c_str());
    }

    i=imageArgsVector_.size();
    for (std::map<std::string,std::string>::iterator it = imageOptions_.begin();
         it!=imageOptions_.end();
         ++it) {
      std::string key = std::string("-") + it->first.c_str();
      imageArgs_[i++] = strdup(key.c_str());
      imageArgs_[i++] = strdup(it->second.c_str());
    }
  }

protected:

  std::map<std::string,std::string> imageOptions_;
  std::vector<std::string> previewArgsVector_;

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

int main(int argc, char * argv[])
{
  //Daemonize("camerabox");
  //Logger::instance(true);

  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace pf(hw_addr);

  pf.init();

  CameraBox cb(&pf);

  cb.input0Changed(0);
  //cb.run();
}
