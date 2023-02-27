#include "LM75A.h"


float lm75aGetValue()
{
    uint8_t rawData[2];
    bool neg = 0;
    int16_t data = 0;
    float temperatura = 0;

    i2c_cmd_handle_t cmd_handle = i2c_cmd_link_create();
    i2c_master_start(cmd_handle);
    /*
    ENDEREÇO DO SENSOR LM75A_ADDR 0x48 // BINARIO: 01001000

         ______    __    __    __    __
    SCL        |__|  |__|  |__|  |__|  |
         __    __       __ __ __ __   
    SDA    |__|  |__ __|  |__|  |__|__ __
         S      1  0 0   1 A2 A1 A0 W  A    
                [ Device Addres   ]        
         S -> START | W -> WRITE | A -> DEVICE
    Portanto, como o ultimo bit é o W, então o endereço "01001000" é deslocado 1 para esquerda
    */
    i2c_master_write_byte(cmd_handle,(LM75A_ADDR << 1) | I2C_MASTER_READ,true); // 0x48 <=> 01001000 -> 0x48 << 1 => 10010000
    i2c_master_read(cmd_handle,(uint8_t *) rawData,2,I2C_MASTER_ACK);
    i2c_master_stop(cmd_handle);
    i2c_master_cmd_begin(I2C_NUM_0, cmd_handle, pdMS_TO_TICKS(250));
    i2c_cmd_link_delete(cmd_handle);
    /*
     Temp data (11 bits)
     MSB                            LSB Not Used
     D10 D9 D8 D7 D6 D5 D4 D3 D2 D1 D0  X X X X X
    
    If the Temp data MSB bit D10 = 0 then the temperature is positive and    Temp value (°C) = + (Temp data) * 0.125 °C2.  
    If the Temp data MSB bit D10 = 1 then the temperature is negative and    Temp  value (°C) = – (2’s complement of Temp data) * 0.125 °C
    Ex:
    0001 1001 000  +0.125°C 
    0000 0000 000   0.000 °C
    1111 1111 111  –0.125 °C
    
    Para verificar se o MSB é  preciso deslocar o byte 0 rawData[0] em 8 bits para direita
    Ex:
    1111 1111 << 8 => 0000 0001 Temp Neg.
    0000 0000 << 8 => 0000 0000 Temp Pos.
    
    */

    if (rawData[0] >> 8 == 1)
    {
        neg = true;

        /*
         Operação lógica AND com byte rawData[0] AND 0b01111111
            |A  AND  B   S|
            |1  AND  0	 0|
            |1	AND  1	 1|
         Dessa maneira, como o bit MSB é 1 então: 1 and 0 = 0;
         Isso ajusta o byte para fazer os calculos, pois caso,
         esteja 1 no MSB o número passa a ser outro e gera uma
         leitura errada.
        */
        rawData[0] = rawData[0] & 0b01111111;
    }
    
    /* 
        Então, agora é necessário colocar os dados da var uint8_t (8 bits)
        para uma var int16_t (16 bits).
        1 - é colocado o primeiro byte na variável data deslocado em 8 bits para esqueda
            para que sobre espaço no final para o próximo byte;
            então a var data =  1111 1111 0000 0000
        2 - realiza a operação lógica E para adionar valor do próximo byte;
            então a var data =  1111 1111 1110 0000
        3 - Por fim, é deslocado todos os bits em 5 bits para direita para
            remover os bits não utilizados no final da variável
            então a var data =  1111 1111 111
    */

    data = (rawData[0] << 8 | rawData[1]) >> 5;
    temperatura = (data * 0.125);
    
    if(neg == true)
    {
        temperatura = temperatura*(-1);
    }

    return temperatura;

}