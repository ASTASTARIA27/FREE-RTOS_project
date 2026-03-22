#include "i2c_master.h"
#include "adxl.h"
#include "DS3231.h"
#include <unistd.h>

int main() {
    fd = I2C_init();
    if(fd < 0) return -1;
    //adxl
    if(adxl_init() < 0) {
        printf("ADXL not found");
        return -1;
    }
    printf("ADXL initialized succesfully");

    //simple loop to read the data
    int16_t x,y,z;
    for(int i=0; i<10; i++) {
        adxl_read(fd,&x,&y,&z);
        printf("X:%d | Y:%d | Z:%d\n",x,y,z);
        usleep(100000); //sleep 100ms
    }
    //DS3231
    uint8_t status;
    I2Cread(fd,BASE_ADDRESS_DS3231,CNTL_STATUS,&status);
    status &= ~0x80; 
    I2Cwrite(fd,BASE_ADDRESS_DS3231,CNTL_STATUS,status);
    if (set_time(fd,0, 50, 12) == 0) {
    printf("Time set successfully!\n");
    } else {
        printf("Failed to set time!\n");
    }
    //simple loop to read data
    int a,b,c;
    for(int i=0; i<10; i++) {
        get_time(fd,&a,&b,&c);
        printf("H:%02d | M:%02d | S:%02d\n",c,b,a);
        usleep(1000000);
    }
    return 0;
}