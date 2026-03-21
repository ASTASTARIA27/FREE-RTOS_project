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

