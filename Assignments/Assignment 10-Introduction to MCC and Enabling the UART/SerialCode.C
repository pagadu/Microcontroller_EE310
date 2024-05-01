//---------------------
// Title: UART interfacing
//---------------------
// Program Details:This program continuously generates random numbers between 1 and 100, sends them via UART, and toggles the RB1 pin.
// The UART communication allows these numbers to be transmitted to terminal(Tera Term) for further processing or display.
// Inputs:
// Outputs: RB1
//          
// Date: May 1, 2024
// File Dependencies / Libraries:
// Compiler: xc8, 2.46
// Author: Alexander Pagaduan
// Versions:
//       V1.0: Initial Draft
// Useful links: 
//       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
//       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
//       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

#include <xc.h>
#include <stdio.h>
#include <time.h>
#include "mcc_generated_files/system/system.h"
int random_number(int min_num, int max_num)
{
    int result = 0, low_num = 0, hi_num = 0;
    
    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num +1;
    } else {
        low_num = max_num + 1;
        hi_num = min_num;
    }
    
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
        
    }
void main(void) {
    
    SYSTEM_Initialize();
    UART2_Initialize();
    srand(time(NULL));

    while(1)
    {
        //printf("Hello World...\r\n");
        //printf("Min:1 Max:40 RAND:%d\r\n", random_number(1,40));
        //printf("Min:100 Max:1000 RAND:%d\r\n", random_number(100,1000));
        printf("Min:1 Max:100 RAND:%d\r\n", random_number(1,100));
        __delay_ms(400);
        PORTB = 0;
        LATB = 0;
        ANSELB = 0;
        TRISB = 0b00000000;
        PORTBbits.RB1 ^= 1; // RB1
        
    }
}
