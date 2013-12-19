#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#ifdef NODEVICE
#include <PiFaceFake.h>
typedef PiFaceFake PiFace_t;
#else
#include <PiFace.h>
typedef PiFace PiFace_t;
#endif

#include <PiFaceStateMachine.h>

class StateMachine : public PiFaceStateMachine
{
public:
  StateMachine(VPiFace* piface) :
    PiFaceStateMachine(piface) {
  }

  void input0Changed(uint8_t state) {
    std::cout << "void input0Changed(uint8_t state) " << (int)state << std::endl;

    if (state==0) {
      uint8_t bit = piface_->readBit(7, VPiFace::Output);
      if (bit==0) {
        piface_->writeBit(1, 7, VPiFace::Output);
      } else {
        piface_->writeBit(0, 7, VPiFace::Output);
      }
    }
  }
  
  void input1Changed(uint8_t state) {
    std::cout << "void input1Changed(uint8_t state) " << (int)state << std::endl;

    if (state==0) {
      uint8_t bit = piface_->readBit(6, VPiFace::Output);
      if (bit==0) {
	piface_->writeBit(1, 6, VPiFace::Output);
      } else {
	piface_->writeBit(0, 6, VPiFace::Output);
      }
    }
  }

  void input2Changed(uint8_t state) {
    std::cout << "void input2Changed(uint8_t state) " << (int)state << std::endl;

    if (state==0) {
      piface_->writeBit(1, 5, VPiFace::Output);
    } else {
      piface_->writeBit(0, 5, VPiFace::Output);
    }
  }

  void input3Changed(uint8_t state) {
    std::cout << "void input3Changed(uint8_t state) " << (int)state << std::endl;

    if (state==0) {
      piface_->writeBit(1, 4, VPiFace::Output);
    } else {
      piface_->writeBit(0, 4, VPiFace::Output);
    }
  }
};

int main(int argc, char * argv[])
{
  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace_t pf(hw_addr);

  pf.init();

  StateMachine sm(&pf);

  sm.run();
}
