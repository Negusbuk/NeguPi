#ifndef PIFACE_H
#define PIFACE_H

#include <cstdint>

#include <VPiFace.h>

class PiFace : public VPiFace
{
public:

  explicit PiFace(uint8_t hw_addr);
  virtual ~PiFace();

  bool init();  

  uint8_t readRegister(uint8_t reg);
  void writeRegister(uint8_t data, uint8_t reg);
  uint8_t readBit(uint8_t bit, uint8_t reg);
  void writeBit(uint8_t data, uint8_t bit, uint8_t reg);
  
protected:

  static uint8_t usecount_;
  static int mcp23s17_fd_;
};

#endif // PIFACE_H
