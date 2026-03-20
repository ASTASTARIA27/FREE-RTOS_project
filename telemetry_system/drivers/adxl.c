#include "adxl.h"

/*
The ADXL345 starts in "Standby" mode.
switch the standby mode using pwr cntrl register(0x2D) to wake up we need to use 
00001000 - which is 0x08
*/
int adxl_init() {

}

/*
The acceleration data is stored in 6 registers (0x32 to 0x37).
*/
int adxl_read(int fd,int16_t *x,int16_t *y,int16_t *z) {

    return 0;
}