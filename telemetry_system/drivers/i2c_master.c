#include "i2c_master.h"

int fd;
//Opening handle for i2c device
int I2C_init() {
    fd = open("/dev/i2c-1",O_RDWR);
    if(fd < 0) {
        perror("error");
        return -1;
    }
    return fd;
}

//write fn has fd device address, register address and data to send
int I2Cwrite(int fd,uint8_t device_addr,uint8_t reg_addr,uint8_t data) {
    uint8_t buffer[2];
    buffer[0] = reg_addr;
    buffer[1] = data;
    if(ioctl(fd,I2C_SLAVE,device_addr) < 0) {
        perror("ioctl");
        return -1;
    }
    if(write(fd,buffer,2) != 2) {
        perror("error");
        return -1;
    }
    return 0;
}

//request and response
int I2Cread(int fd, uint8_t device_addr, uint8_t reg_addr, uint8_t *data) {

    if(ioctl(fd,I2C_SLAVE,device_addr) < 0) {
        perror("ioctl");
        return -1;
    }
    if(write(fd,&reg_addr,1) != 1) {
        perror("error");
        return -1;
    }
     if(read(fd,data,1) != 1) {
        perror("error");
        return -1;
    }
    return 0;
}
