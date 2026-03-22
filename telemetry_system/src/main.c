#include "i2c_master.h"
#include "adxl.h"
#include "DS3231.h"
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

SemaphoreHandle_t xI2CMutex; //key to the I2C bus
//for adxl
//try to take the key and wait forever(portMAX_Delay) if someone else has it
void vtelemetryTask (void *pvParameter) {
    int16_t x,y,z;
    TickType_t xLastTickTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(100);
    while(1) {
        if(xSemaphoreTake(xI2CMutex,portMAX_DELAY)==pdTRUE) {
            adxl_read(fd,&x,&y,&z); //use the bus safely
            xSemaphoreGive(xI2CMutex); //give the key immediately
            printf("X:%d | Y:%d | Z:%d\n",x,y,z);
        }
        //sleep for 100ms
        //vTaskDelay(pdMS_TO_TICKS(100));
        vTaskDelayUntil(&xLastTickTime,xFrequency); //looks at lasttick time and calculates how much time its need to hit the next 100ms perfectly
    
    }
}

//forDS3231
//pv pointer to void
void vclockTask (void *pvParameter) {
    int h,m,s;
    while(1) {
        if(xSemaphoreTake(xI2CMutex,portMAX_DELAY)==pdTRUE){
            get_time(fd,&s,&m,&h);
            xSemaphoreGive(xI2CMutex);
            printf("H:%02d | M:%02d | S:%02d\n",h,m,s);
        }
        //sleep for 1 second
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    fd = I2C_init();
    if(fd < 0) return -1;
    //adxl
    if(adxl_init() < 0) {
        printf("ADXL not found");
        return -1;
    }
    printf("ADXL initialized succesfully");

    //DS3231
    //DIsabling Oscillator stop flags so clock runs
    uint8_t status;
    I2Cread(fd,BASE_ADDRESS_DS3231,CNTL_STATUS,&status);
    status &= ~0x80; 
    I2Cwrite(fd,BASE_ADDRESS_DS3231,CNTL_STATUS,status);

    //creating the Mutex
    xI2CMutex = xSemaphoreCreateMutex();
    if(xI2CMutex != NULL) {
        //Create the telemetry task
        xTaskCreate(vtelemetryTask,"Telemetry",2048,NULL,2,NULL);
        xTaskCreate(vclockTask,"Clock",2048,NULL,1,NULL);
        //Start the scheduler now (control goes to the freeRtos)
        vTaskStartScheduler();
    }
    for(;;);

    return 0;
}