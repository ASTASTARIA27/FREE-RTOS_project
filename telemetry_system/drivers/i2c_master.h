#ifndef I2C_MASTER_H
#define I2C_MASTER_H
#include <fcntl.h>      // For open()
#include <unistd.h>     // For read(), write(), close()
#include <sys/ioctl.h>  // For ioctl()
#include <linux/i2c-dev.h> // For I2C_SLAVE constant
#include <i2c/smbus.h>
#include <stdio.h>      // For printf/debugging
#include <stdint.h>

int fd;
//function declarations
int I2C_init();
int I2Cwrite(int fd,uint8_t device_addr,uint8_t reg_addr,uint8_t data);
int I2Cread(int fd, uint8_t device_addr, uint8_t *reg_addr, uint8_t *data);

#endif