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

#include "timelapse.h"

using namespace libconfig;
using namespace NeguPi;

TimeLapse::TimeLapse(PiFace* piface)
 :PiFaceStateMachine(piface),
  inTimeLapse_(false),
  delay_(0),
  imageLoopCount_(1),
  imageCount_(1)
{

  readConfig();

  previewArgsVector_.push_back("raspistill");
  previewArgsVector_.push_back("-w"); previewArgsVector_.push_back("960");
  previewArgsVector_.push_back("-h"); previewArgsVector_.push_back("540");
  previewArgsVector_.push_back("-t"); previewArgsVector_.push_back("1");
  previewArgsVector_.push_back("-n"); previewArgsVector_.push_back("-n");
  previewArgsVector_.push_back("-th"); previewArgsVector_.push_back("none");
  previewArgsVector_.push_back("-o"); previewArgsVector_.push_back("/home/pi/www/preview.jpg");

  previewArgs_ = new char *[previewArgsVector_.size() + imageOptions::numberOfOptions()*2 + 1];
  previewArgs_[previewArgsVector_.size() + imageOptions::numberOfOptions()*2] = 0;

  for (uint8_t i=0;i<previewArgsVector_.size();++i) {
    previewArgs_[i] = strdup(previewArgsVector_.at(i).c_str());
  }

  imageArgsVector_.push_back("raspistill");
  imageArgsVector_.push_back("-o"); imageArgsVector_.push_back(outputDir_ + "/image_XXX_YYYYYYYY.jpg ");
  imageArgsVector_.push_back("-w"); imageArgsVector_.push_back("1280");
  imageArgsVector_.push_back("-h"); imageArgsVector_.push_back("720");
  imageArgsVector_.push_back("-t"); imageArgsVector_.push_back("1");
  imageArgsVector_.push_back("-n"); imageArgsVector_.push_back("-n");
  imageArgsVector_.push_back("-th"); imageArgsVector_.push_back("none");

  imageArgs_ = new char *[imageArgsVector_.size() + imageOptions::numberOfOptions()*2 + 1];
  imageArgs_[imageArgsVector_.size() + imageOptions::numberOfOptions()*2] = 0;

  for (uint8_t i=0;i<imageArgsVector_.size();++i) {
    imageArgs_[i] = strdup(imageArgsVector_.at(i).c_str());
  }

  readImageOptions();
}

void TimeLapse::input0Changed(uint8_t state)
{

  if (state==0) {
    Log() << "take new preview picture";

    readImageOptions();

    pid_t child_pid = fork();
    if (child_pid == 0) {

      std::ostringstream oss;
      int i = 0;
      while (previewArgs_[i]!=0) {
	oss << previewArgs_[i] << " ";
	i++;
      }
      Log() << oss.str();

      execvp(previewArgs_[0], previewArgs_);
      /* The execvp function returns only if an error occurs.  */
      Log() << "an error occurred in execvp";
      abort();
    }
  }
}

void TimeLapse::input3Changed(uint8_t state)
{

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

void TimeLapse::heartBeat(int milliseconds)
{

  if (inTimeLapse_) {
    delay_ += milliseconds;
    if (delay_>timeout_) {

      sprintf(imageArgs_[2],
              "%s/image_%03d_%06d.jpg",
              outputDir_.c_str(), imageLoopCount_, imageCount_++);

      pid_t child_pid = fork();
      if (child_pid == 0) {
	
	std::ostringstream oss;
	int i = 0;
	while (imageArgs_[i]!=0) {
	  oss << imageArgs_[i] << " ";
	  i++;
	}
	Log() << oss.str();

        execvp(imageArgs_[0], imageArgs_);
        /* The execvp function returns only if an error occurs.  */
        Log() << "an error occurred in execvp";
        abort();
      }

      delay_ = 0;
    }
  }
}

void TimeLapse::checkOutputDirectory()
{

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

void TimeLapse::readImageOptions()
{
  imageOptions_.read();

  uint8_t i = previewArgsVector_.size();
  previewArgs_[i++] = strdup("-ISO");
  previewArgs_[i++] = strdup(imageOptions_.aISO.c_str());
  previewArgs_[i++] = strdup("-ex");
  previewArgs_[i++] = strdup(imageOptions_.aEX.c_str());
  if (imageOptions_.iSS!=0) {
    previewArgs_[i++] = strdup("-ss");
    previewArgs_[i++] = strdup(imageOptions_.aSS.c_str());
  }
  previewArgs_[i++] = strdup("-mm");
  previewArgs_[i++] = strdup(imageOptions_.aMM.c_str());
  previewArgs_[i++] = strdup("-ev");
  previewArgs_[i++] = strdup(imageOptions_.aEV.c_str());
  previewArgs_[i++] = strdup("-sh");
  previewArgs_[i++] = strdup(imageOptions_.aSH.c_str());
  previewArgs_[i++] = strdup("-co");
  previewArgs_[i++] = strdup(imageOptions_.aCO.c_str());
  previewArgs_[i++] = strdup("-br");
  previewArgs_[i++] = strdup(imageOptions_.aBR.c_str());
  previewArgs_[i++] = strdup("-sa");
  previewArgs_[i++] = strdup(imageOptions_.aSA.c_str());
  previewArgs_[i++] = 0;

  i = imageArgsVector_.size();
  imageArgs_[i++] = strdup("-ISO");
  imageArgs_[i++] = strdup(imageOptions_.aISO.c_str());
  imageArgs_[i++] = strdup("-ex");
  imageArgs_[i++] = strdup(imageOptions_.aEX.c_str());
  if (imageOptions_.iSS!=0) {
    imageArgs_[i++] = strdup("-ss");
    imageArgs_[i++] = strdup(imageOptions_.aSS.c_str());
  }
  imageArgs_[i++] = strdup("-mm");
  imageArgs_[i++] = strdup(imageOptions_.aMM.c_str());
  imageArgs_[i++] = strdup("-ev");
  imageArgs_[i++] = strdup(imageOptions_.aEV.c_str());
  imageArgs_[i++] = strdup("-sh");
  imageArgs_[i++] = strdup(imageOptions_.aSH.c_str());
  imageArgs_[i++] = strdup("-co");
  imageArgs_[i++] = strdup(imageOptions_.aCO.c_str());
  imageArgs_[i++] = strdup("-br");
  imageArgs_[i++] = strdup(imageOptions_.aBR.c_str());
  imageArgs_[i++] = strdup("-sa");
  imageArgs_[i++] = strdup(imageOptions_.aSA.c_str());
  imageArgs_[i++] = 0;
}

void TimeLapse::readConfig()
{
  bool writeCfg = false;
  std::string homePath = getenv("HOME");
  std::string cfgFile = homePath + "/.NeguPi.cfg";
  Config cfg;
  try {
    cfg.readFile(cfgFile.c_str());
  }
  catch (const FileIOException &fioex) {
    Log() << "NeguPi: I/O error while reading config file " << cfgFile;
    writeCfg = true;
  }
  catch (const ParseException &pex) {
    Log() << "Parse error at " << pex.getFile() << ":" << pex.getLine()
          << " - " << pex.getError();
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
      Log() << "NeguPi: configuration successfully written to " << cfgFile;
    }
    catch (const FileIOException &fioex) {
      Log() << "NeguPi: I/O error while writing config file " << cfgFile;
    }
    catch (...) {

    }
  }
}

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
