#include "i2c_master.h"
#include "adxl.h"
#include "DS3231.h"
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdbool.h>
#include <gpiod.h>
#define TRUE 1
#define FALSE 0
/*--------------*/
//key to the I2C bus
SemaphoreHandle_t xI2CMutex;


//creating the shutdown
SemaphoreHandle_t xShutdownSemaphore;

//creating task handles to assign them when creating tasks
TaskHandle_t xTelemetryHandle = NULL;
TaskHandle_t xClockHandle = NULL;


/*two main tasks are telling the scheduleer to stop or delete the tasks so they dont acccess I2C
while closing it
*/
void shutdown() {
    //deleting tasks based on priority, low first
    vTaskDelete(xClockHandle);
    vTaskDelete(xTelemetryHandle);
    //end scheduler
    vTaskEndScheduler();
    close(fd);
    exit(0);
}
//shutdown
void vshutdownTask (void *pvParameter) {
    while(1) {
        if(xSemaphoreTake(xShutdownSemaphore,portMAX_DELAY)==pdTRUE) {
            shutdown();
        }
    }
}

void vISRTask (void *pvParameter) {
    (void)pvParameter;
    /*for INterrunpt service routine*/
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    struct gpiod_line_event event;
    int result;
    //open the gpio chip
    chip = gpiod_chip_open_by_number(4);
    if(!chip) {
        perror("failed to open chip4\n");
        vTaskDelete(NULL);
    }
    //get the specific line
    line = gpiod_chip_get_line(chip,17);
    if(!line) {
        perror("failed to get in line\n");
        gpiod_chip_close(chip);
        vTaskDelete(NULL);
    }
    //request for rising edge events
    //telling the linux the transition from 3.3v to 0
    if(gpiod_line_request_rising_edge_events(line,"Shutdown") < 0) {
        perror("Request Events failed");
        gpiod_chip_close(chip);
        vTaskDelete(NULL);
    }
    while(1) {
        //wait for the event
        //we wait a timeout of 1s and loopback to check again
        struct timespec timeout = {1,0};
        result = gpiod_line_event_wait(line, &timeout);
        if(result > 0) {
            //read the event to clear it
            gpiod_line_event_read(line, &event);
            printf("ISR enabled signalling shutdown\n");
            //triggering the shutdown task
            xSemaphoreGive(xShutdownSemaphore);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

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
   // int count=0; use if if u want to see the shutdown
    while(1) {
        if(xSemaphoreTake(xI2CMutex,portMAX_DELAY)==pdTRUE){
            get_time(fd,&s,&m,&h);
            xSemaphoreGive(xI2CMutex);
            printf("H:%02d | M:%02d | S:%02d\n",h,m,s);
            /*use if u want to use shutdown
            count++;
            if(count==10) {
                xSemaphoreGive(xShutdownSemaphore);
            }
            */
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

    //creating the shutdown
    xShutdownSemaphore = xSemaphoreCreateBinary();
    //creating the Mutex
    xI2CMutex = xSemaphoreCreateMutex();
    if(xShutdownSemaphore!=NULL && xI2CMutex != NULL) {
        //Create the telemetry task
        xTaskCreate(vtelemetryTask,"Telemetry",2048,NULL,2,&xTelemetryHandle);
        xTaskCreate(vclockTask,"Clock",2048,NULL,1,&xClockHandle);
        xTaskCreate(vshutdownTask,"Shutdown",2048,NULL,3,NULL);
        xTaskCreate(vISRTask,"Button",2048,NULL,4,NULL);
        //Start the scheduler now (control goes to the freeRtos)
        vTaskStartScheduler();
    }
    for(;;);

    return 0;
}