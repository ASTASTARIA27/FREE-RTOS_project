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
    int sec,min,hr; //seconds minutes and hours
    sec = decTobcd(seconds);
    min = decTobcd(minutes);
    hr = decTobcd(hours);
    if(ioctl(fd,I2C_SLAVE,BASE_ADDRESS_DS3231) < 0) {
        perror("error");
        return -1;
    }
    I2Cwrite(fd,BASE_ADDRESS_DS3231,SECONDS,sec);
    I2Cwrite(fd,BASE_ADDRESS_DS3231,MINUTES,min);
    I2Cwrite(fd,BASE_ADDRESS_DS3231,HOURS,hr);
    return 0;
}

int get_time(int *seconds, int *minutes, int *hours) {
    uint8_t sec,min,hr;
    if(ioctl(fd,I2C_SLAVE,BASE_ADDRESS_DS3231) < 0) {
        perror("error");
        return -1;
    }
    if(I2Cread(fd,BASE_ADDRESS_DS3231,SECONDS,&sec) != 0) return -1;
    if(I2Cread(fd,BASE_ADDRESS_DS3231,MINUTES,&min) != 0) return -1;
    if(I2Cread(fd,BASE_ADDRESS_DS3231,HOURS,&hr) !=0) return -1;
    *seconds = bcdTodec(sec);
    *minutes = bcdTodec(min);
    *hours = bcdTodec(hr);
    return 0;
}