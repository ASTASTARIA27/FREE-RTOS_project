
#ifndef ADXL_H
#define ADXL_H
#include <fcntl.h>      // For open()
#include <unistd.h>     // For read(), write(), close()
#include <sys/ioctl.h>  // For ioctl()
#include <linux/i2c-dev.h> // For I2C_SLAVE constant
#include <i2c/smbus.h>
#include <stdio.h>      // For printf/debugging
#include <stdint.h>
#include "i2c_master.h"

//Macros definitions
#define DEVID 0x00
#define POWER_CNTL 0x2D
#define DATAX0 0x32
#define DATAX1 0x33
#define DATAY0 0x34
#define DATAY1 0x35
#define DATAZ0 0x36
#define DATAZ1 0x37
#endif