#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef NODEVICE
#include <PiFaceFake.h>
typedef PiFaceFake PiFace_t;
#else
#include <PiFace.h>
typedef PiFace PiFace_t;
#endif

int main(int argc, char * argv[])
{
  int hw_addr = 0;
  if (argc > 1) {
    hw_addr = atoi(argv[1]);
  }

  PiFace_t pf(hw_addr);

  pf.init();

  while (1){
    pf.writeBit(1, 7, VPiFace::Output);
    uint8_t bit = pf.readBit(7, VPiFace::Output);
    printf("Bit 7 is %d\n", bit);
    usleep(500000);
    pf.writeBit(0, 7, VPiFace::Output);
    bit = pf.readBit(7, VPiFace::Output);
    printf("Bit 7 is %d\n", bit);
    usleep(500000);
  }
}
