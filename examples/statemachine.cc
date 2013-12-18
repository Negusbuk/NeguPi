#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <functional>

#ifdef NODEVICE
#include <PiFaceFake.h>
typedef PiFaceFake PiFace_t;
#else
#include <PiFace.h>
typedef PiFace PiFace_t;
#endif

#include <PiFaceStateMachine.h>

void bit0changed(uint8_t state)
{
  std::cout << "void bit0changed(uint8_t state) " << (int)state << std::endl;
}

void bit1changed(uint8_t state)
{
  std::cout << "void bit1changed(uint8_t state) " << (int)state << std::endl;
}

void bit2changed(uint8_t state)
{
  std::cout << "void bit2changed(uint8_t state) " << (int)state << std::endl;
}

void bit3changed(uint8_t state)
{
  std::cout << "void bit3changed(uint8_t state) " << (int)state << std::endl;
}

int main(int argc, char * argv[])
{
    int hw_addr = 0;
    if (argc > 1) {
        hw_addr = atoi(argv[1]);
    }

    PiFace_t pf(hw_addr);

    pf.init();

    PiFaceStateMachine sm(&pf);

    std::function<void(uint8_t)> f0 = bit0changed;
    sm.setCallback(0, f0);

    std::function<void(uint8_t)> f1 = bit1changed;
    sm.setCallback(1, f1);

    std::function<void(uint8_t)> f2 = bit2changed;
    sm.setCallback(2, f2);

    std::function<void(uint8_t)> f3 = bit3changed;
    sm.setCallback(3, f3);

    sm.run();
}
