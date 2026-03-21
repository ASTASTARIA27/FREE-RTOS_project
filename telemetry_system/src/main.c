#include "i2c_master.h"
#include "adxl.h"
#include <unistd.h>

int main() {
    fd = I2C_init();
    if(fd < 0) return -1;
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
    return 0;
}