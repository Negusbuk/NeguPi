#ifndef PIFACEFAKE_H
#define PIFACEFAKE_H

#include <cstdint>

#include <VPiFace.h>

class PiFaceFake : public VPiFace
{
public:

  explicit PiFaceFake(uint8_t hw_addr);
  virtual ~PiFaceFake();

  bool init();  

  uint8_t readRegister(uint8_t reg);
  void writeRegister(uint8_t data, uint8_t reg);
  uint8_t readBit(uint8_t bit, uint8_t reg);
  void writeBit(uint8_t data, uint8_t bit, uint8_t reg);
  
protected:

  static uint8_t inputState_;
  static uint8_t outputState_;
};

#endif // PIFACEFAKE_H
