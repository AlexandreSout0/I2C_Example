
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/i2c.h"
#include "esp_types.h"
#include "esp_log.h"

#include "LM75A.h"

SemaphoreHandle_t mutexI2C0;

void setup()
{
    mutexI2C0 = xSemaphoreCreateMutex();
    i2c_config_t i2c0Config = 
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C0_SDA,
        .scl_io_num = I2C0_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };

    i2c_param_config(I2C_NUM_0, &i2c0Config);
    i2c_driver_install(I2C_NUM_0,I2C_MODE_MASTER,0,0,0);


}

void readTemperature(void * args)
{
    while(1)
    {
        if(xSemaphoreTake(mutexI2C0, 250/ portTICK_PERIOD_MS))
        {
            float temperature = lm75aGetValue();
            printf("Temperatura: %f \n", temperature);
            xSemaphoreGive(mutexI2C0);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


void app_main(void)
{
    setup();
    xTaskCreate(readTemperature, "Read Temperature",2048, NULL,2,NULL);


}