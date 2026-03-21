#include "adxl.h"

/*
The ADXL345 starts in "Standby" mode.
switch the standby mode using pwr cntrl register(0x2D) to wake up we need to use 
00001000 - which is 0x08
*/
int adxl_init() {
    uint8_t reg_addr = DEVID;
    uint8_t value;
    //used to verify the chip is actually connected or not
    I2Cread(fd,BASE_ADDRESS_ADXL,reg_addr,&value);
    if(value != 0XE5) {
        perror("error");
        return -1;
    }
    I2Cwrite(fd,BASE_ADDRESS_ADXL,POWER_CNTL,0x08); //writing value of 0x08 in the power control in the measurement mode
    return 0;
}

/*
The acceleration data is stored in 6 registers (0x32 to 0x37).
*/
int adxl_read(int fd,int16_t *x,int16_t *y,int16_t *z) {
    uint8_t x_lsb,x_msb,y_lsb,y_msb,z_lsb,z_msb;
    I2Cread(fd,BASE_ADDRESS_ADXL,DATAX0,&x_lsb);
    I2Cread(fd,BASE_ADDRESS_ADXL,DATAX1,&x_msb);
    I2Cread(fd,BASE_ADDRESS_ADXL,DATAY0,&y_lsb);
    I2Cread(fd,BASE_ADDRESS_ADXL,DATAY1,&y_msb);
    I2Cread(fd,BASE_ADDRESS_ADXL,DATAZ0,&z_lsb);
    I2Cread(fd,BASE_ADDRESS_ADXL,DATAZ1,&z_msb);
    *x = (int16_t)((x_msb)<<8 | x_lsb);
    *y = (int16_t)((y_msb)<<8 | y_lsb);
    *z = (int16_t)((z_msb)<<8 | z_lsb);
    return 0;
}