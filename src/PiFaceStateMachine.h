#ifndef PIFACESTATEMACHINE_H
#define PIFACESTATEMACHINE_H

#include <functional>
#include <chrono>

#include <VPiFace.h>

class PiFaceStateMachine
{
public:

  explicit PiFaceStateMachine(VPiFace* piface);
  virtual ~PiFaceStateMachine();
  
  void run();

  virtual void input0Changed(uint8_t state) { }
  virtual void input1Changed(uint8_t state) { }
  virtual void input2Changed(uint8_t state) { }
  virtual void input3Changed(uint8_t state) { }
  virtual void input4Changed(uint8_t state) { }
  virtual void input5Changed(uint8_t state) { }
  virtual void input6Changed(uint8_t state) { }
  virtual void input7Changed(uint8_t state) { }
  
  virtual void heartBeat(int milliseconds) { }

protected:

  VPiFace* piface_;

  uint8_t lastInputState_;
  unsigned int timeout_;
  
  std::function<void(PiFaceStateMachine*, uint8_t)> functions_[8];
  void input0Changed_(uint8_t state) { this->input0Changed(state); }
  void input1Changed_(uint8_t state) { this->input1Changed(state); }
  void input2Changed_(uint8_t state) { this->input2Changed(state); }
  void input3Changed_(uint8_t state) { this->input3Changed(state); }
  void input4Changed_(uint8_t state) { this->input4Changed(state); }
  void input5Changed_(uint8_t state) { this->input5Changed(state); }
  void input6Changed_(uint8_t state) { this->input6Changed(state); }
  void input7Changed_(uint8_t state) { this->input7Changed(state); }
};

#endif // PIFACE_H
