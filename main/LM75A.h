#ifndef __LM75_H
    #define __LM75A_H

    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "driver/i2c.h"
    #include "esp_types.h"

    #define I2C0_SDA 33
    #define I2C0_SCL 32
    #define LM75A_ADDR 0x48 // BINARIO: 1001000
    
    float lm75aGetValue();

#endif



