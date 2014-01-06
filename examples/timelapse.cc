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

#include <iostream>
#include <vector>
#include <string>

#ifdef NODEVICE
#include <PiFaceFake.h>
typedef PiFaceFake PiFace_t;
#else
#include <PiFace.h>
typedef PiFace PiFace_t;
#endif

#include <PiFaceStateMachine.h>

class TimeLapse : public PiFaceStateMachine
{
public:
  TimeLapse(VPiFace* piface) :
    PiFaceStateMachine(piface) {

    std::vector<std::string> args;
    args.push_back("raspistill");
    args.push_back("--width 400");
    args.push_back("--height 300");
    args.push_back("--output /home/pi/preview.jpg");

    previewArgs_ = new char *[args.size() + 1];
    previewArgs_[args.size()] = 0;

    for (uint8_t i=0;i<args.size();++i) {
      previewArgs_[i] = strdup(args.at(i).c_str());
    }
  }

  void input0Changed(uint8_t state) {

    if (state==0) {
      std::cout << "take new preview picture" << std::endl;

      pid_t child_pid = fork();
      if (child_pid == 0) {
        execvp(previewArgs_[0], previewArgs_);
        /* The execvp function returns only if an error occurs.  */
        printf ("an error occurred in execl\n");
        abort();
      }
    }
  }
  
  void input1Changed(uint8_t state) {
    std::cout << "void input1Changed(uint8_t state) " << (int)state << std::endl;
  }

  void input2Changed(uint8_t state) {
    std::cout << "void input2Changed(uint8_t state) " << (int)state << std::endl;
  }

  void input3Changed(uint8_t state) {
    std::cout << "void input3Changed(uint8_t state) " << (int)state << std::endl;
  }

protected:

  char** previewArgs_;
};

int main(int argc, char * argv[])
{
  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace_t pf(hw_addr);

  pf.init();

  TimeLapse tl(&pf);

  tl.run();
}
