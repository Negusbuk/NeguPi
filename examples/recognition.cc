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

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <NeguPiDaemon.h>
#include <NeguPiLogger.h>

#include <PiFace.h>
#include <PiFaceStateMachine.h>

using namespace libconfig;
using namespace NeguPi;

class TimeLapse : public PiFaceStateMachine
{
public:

  TimeLapse(PiFace* piface) :
    PiFaceStateMachine(piface),
    inTimeLapse_(false),
    delay_(0),
    imageLoopCount_(1),
    imageCount_(1) {

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
    args.push_back("--output"); args.push_back(outputDir_ + "/image_XXX_YYYYYYYY.jpg ");

    /*
    args.push_back("touch");
    args.push_back(outputDir_ + "/image_XXX_YYYYYYYY.jpg ");
    */

    imageArgs_ = new char *[args.size() + 1];
    imageArgs_[args.size()] = 0;
    nImageArgs_ = args.size();

    for (uint8_t i=0;i<args.size();++i) {
      imageArgs_[i] = strdup(args.at(i).c_str());
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

        cv::Mat img = cv::imread("test.jpg");

        abort();
      }
    }
  }
  
  void input3Changed(uint8_t state) {

    if (state==0) {
      checkOutputDirectory();
      imageCount_ = 1;
      inTimeLapse_ = true;
      delay_ = 0;
      Log() << "inTimeLapse_ = true";
      Log() << "starting image sequence " << imageLoopCount_;
    } else {
      inTimeLapse_ = false;
      Log() << "inTimeLapse_ = false";
    }
  }

  void heartBeat(int milliseconds) {

    if (inTimeLapse_) {
      delay_ += milliseconds;
      if (delay_>timeout_) {
        // Log() << "beat " << delay_;

        sprintf(imageArgs_[nImageArgs_-1],
                "%s/image_%03d_%06d.jpg",
                outputDir_.c_str(), imageLoopCount_, imageCount_++);
        
        pid_t child_pid = fork();
        if (child_pid == 0) {
          execvp(imageArgs_[0], imageArgs_);
          /* The execvp function returns only if an error occurs.  */
          printf ("an error occurred in execl\n");
          abort();
        }

        delay_ = 0;
      }
    }
  }

  void checkOutputDirectory() {

    imageLoopCount_ = 0;

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
      if (strncmp(filename.c_str(), "image_", 6)!=0) continue;

      filename = filename.substr(6, 6+3);
      loop = std::stoi(filename);
      if (loop>imageLoopCount_) imageLoopCount_ = loop;
    }

    imageLoopCount_++;

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
    if (!root.exists("timelapse")) {
      root.add("timelapse", Setting::TypeGroup);
      writeCfg = true;
    }
    Setting &settings = root["timelapse"];

    if (!settings.lookupValue("imageDir", outputDir_)) {
      outputDir_ = homePath + "/timelapse";
      settings.add("imageDir", Setting::TypeString) = outputDir_.c_str();
      writeCfg = true;
    }

    if (!settings.lookupValue("timeout", timeout_)) {
      timeout_ = 2*1000;
      settings.add("timeout", Setting::TypeInt) = timeout_;
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

  std::string outputDir_;
  int timeout_;

  char** previewArgs_;
  int nImageArgs_;
  char** imageArgs_;
  bool inTimeLapse_;
  int delay_;
  int imageLoopCount_;
  int imageCount_;
};

int main(int argc, char * argv[])
{
  Daemonize("timelapse");
  Logger::instance(true);
  
  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace pf(hw_addr);

  pf.init();

  TimeLapse tl(&pf);

  tl.run();
}
