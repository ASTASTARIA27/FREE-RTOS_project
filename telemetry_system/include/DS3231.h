#ifndef DS3231_H
#define DS3231_H
#include <fcntl.h>      // For open()
#include <unistd.h>     // For read(), write(), close()
#include <sys/ioctl.h>  // For ioctl()
#include <linux/i2c-dev.h> // For I2C_SLAVE constant
#include <i2c/smbus.h>
#include <stdio.h>      // For printf/debugging
#include <stdint.h>
#include "i2c_master.h"

//macros definition
#define BASE_ADDRESS_DS3231 0x68
#define SECONDS 0x00
#define MINUTES 0x01
#define HOURS 0x02
#define DAY 0x03
#define DATE 0x04
#define MONTH 0x05
#define YEAR 0x06
#define CONTROL 0x0E
#define CNTL_STATUS 0x0F
#define ALARM1_SECONDS 0x07
#define ALARM1_MINUTES 0x08
#define ALARM1_HOUR 0x09
#define ALARM1_DAY 0x0A

//function declarations
uint8_t decTobcd(uint8_t val);
uint8_t bcdTodec(uint8_t val);
int set_time(int fd,int seconds,int minutes,int hours);
int get_time(int fd,int *seconds, int *minutes, int *hours);
int setDate(int fd,int Day, int Date, int Month, int Year);
int getDate(int fd,int *Day, int *Date,int *Month, int *Year);
int setAlarm1(int fd,int seconds, int minutes, int hours, int day);
#endif