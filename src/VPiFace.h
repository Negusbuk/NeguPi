#ifndef VPIFACE_H
#define VPIFACE_H

#include <cstdint>

class VPiFace
{
 public:

  enum PiFaceGPIO {
    Output = 0x12,
    Input  = 0x13
  };

  explicit VPiFace(uint8_t hw_addr);
  virtual ~VPiFace() { }

  virtual bool init() = 0;

  virtual uint8_t readRegister(uint8_t reg) = 0;
  virtual void writeRegister(uint8_t data, uint8_t reg) = 0;
  virtual uint8_t readBit(uint8_t bit, uint8_t reg) = 0;
  virtual void writeBit(uint8_t data, uint8_t bit, uint8_t reg) = 0;
  
 protected:

  uint8_t hw_addr_;
};

#endif // VPIFACE_H
