#include <stdio.h>
#include "DS3231.h"
//DS3231 dosent store "15 Seconds" as 0000 1111 (binary 15).It stores it as It stores it as 0001 0101 (BCD 15).
//val/10 to get the 10th pos and val%10 to get the ones pos
uint8_t decTobcd(uint8_t val) {
    return ((val/10)<<4 | (val%10));
}

uint8_t bcdTodec(uint8_t val) {
    return ((val>>4)*10) + (val%16);
}


int set_time(int seconds,int minutes,int hours) {
    uint8_t buffer[3];
    buffer[0] = decTobcd(seconds);
    buffer[1] = decTobcd(minutes);
    buffer[2] = decTobcd(hours);
    if(ioctl(fd,I2C_SLAVE,BASE_ADDRESS_DS3231) < 0) {
        perror("error");
        return -1;
    }
    I2Cwrite_mul(fd,BASE_ADDRESS_DS3231,SECONDS,buffer,3);
    return 0;
}

int get_time(int *seconds, int *minutes, int *hours) {
    uint8_t  buffer[3];
    if(ioctl(fd,I2C_SLAVE,BASE_ADDRESS_DS3231) < 0) {
        perror("error");
        return -1;
    }
    I2Cread_mul(fd,BASE_ADDRESS_DS3231,SECONDS,buffer,3);
    *seconds = bcdTodec(buffer[0]);
    *minutes = bcdTodec(buffer[1]);
    *hours = bcdTodec(buffer[2]);
    return 0;
}

int setDate(int Day, int Date, int Month, int Year) {
    uint8_t buffer[4];
    buffer[0] = decTobcd(Day);
    buffer[1] = decTobcd(Date);
    buffer[2] = decTobcd(Month);
    buffer[3] = decTobcd(Year);
    if(ioctl(fd,I2C_SLAVE,BASE_ADDRESS_DS3231) < 0) {
        perror("error");
        return -1;
    }
    I2Cwrite_mul(fd,BASE_ADDRESS_DS3231,DAY,buffer,4);
    return 0;
}

int getDate(int *Day, int *Date,int *Month, int *Year) {
    uint8_t buffer[4];
    if(ioctl(fd,I2C_SLAVE,BASE_ADDRESS_DS3231) < 0) {
        perror("error");
        return -1;
    }
    I2Cread_mul(fd,BASE_ADDRESS_DS3231,DAY,buffer,4);
    *Day = bcdTodec(buffer[0]);
    *Date = bcdTodec(buffer[1]);
    *Month = bcdTodec(buffer[2]& 0x7F); // 0x7F is 0111 1111, which ignores Bit 7(0 to 7)
    *Year = bcdTodec(buffer[3]);
    return 0;
}