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

int I2Cwrite_mul(int fd, uint8_t device_addr, uint8_t reg_addr, uint8_t *data, int len) {
    uint8_t buffer[len+1];
    buffer[0] = reg_addr;
    for(int i=0; i<len; i++) {
        buffer[len+i] = data[i];
    }
    if(ioctl(fd,I2C_SLAVE,device_addr) < 0) {
        perror("error");
        return -1;
    }
    if(write(fd,buffer,len+1)!=(len+1)) {
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

int I2Cread_mul(int fd,uint8_t device_addr, uint8_t reg_addr, uint8_t *data, int len) {
    if(ioctl(fd,I2C_SLAVE,device_addr) < 0) {
        perror("error");
        return -1;
    }
    //writing starting regi address to the chip
    if (write(fd, &reg_addr, 1) != 1) {
        perror("write reg_addr error");
        return -1;
    }
    // Read len bytes into the data pointer
    if (read(fd, data, len) != len) {
        perror("read burst error");
        return -1;
    }

    return 0;

}
