#include <iostream>
#include <chrono>
#include <thread>

#include "PiFaceStateMachine.h"

void PiFaceStateMachineDummyCallback(uint8_t state)
{
  std::cout << (int)state << std::endl;
}

PiFaceStateMachine::PiFaceStateMachine(VPiFace* piface)
  :piface_(piface),
   lastInputState_(0)
{
  lastInputState_ = piface_->readRegister(VPiFace::Input);
  lastInputState_ = 2;

  for (int i=0;i<8;++i) {
    functions_[i] = PiFaceStateMachineDummyCallback;
  }
}

PiFaceStateMachine::~PiFaceStateMachine()
{

}

void PiFaceStateMachine::run()
{
  while (1) {
    uint8_t state = piface_->readRegister(VPiFace::Input);

    if (state!=lastInputState_) {
      uint mask = 1;
      for (int i=0;i<8;++i) {
        if ( (state&mask) != (lastInputState_&mask) ) {
          if (state&mask) {
            functions_[i](1);
          } else {
            functions_[i](0);
          }
        }
        mask <<= 1;
      }
    }

    lastInputState_ = state;

    std::this_thread::sleep_for( std::chrono::milliseconds(250) );
  }
}

void PiFaceStateMachine::setCallback(uint8_t bit, std::function<void(uint8_t)>& f)
{
  if (bit>=8) return;
  functions_[bit] = f;
}
