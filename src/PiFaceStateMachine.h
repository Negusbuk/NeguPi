#ifndef PIFACESTATEMACHINE_H
#define PIFACESTATEMACHINE_H

#include <functional>

#include <VPiFace.h>

class PiFaceStateMachine
{
 public:

  explicit PiFaceStateMachine(VPiFace* piface);
  virtual ~PiFaceStateMachine();
  
  void run();
  void setCallback(uint8_t bit, std::function<void(uint8_t)>& f);

 protected:

  VPiFace* piface_;

  uint8_t lastInputState_;

  std::function<void(uint8_t)> functions_[8];
};

#endif // PIFACE_H
