//---------------------
// Title: LcdInterfaceProject
//---------------------
// Program Details: This focuses on interfacing with a LCD(Liquid Crystal Display)
// and reads data from an analog sensor (aka photoresistor) using ADC.

// Inputs: RA0
// Outputs: RD0, RD1, RB0, RB1, RB2, RB3, RB4, RB5, RB6, RB7
//          
// Date: April 30, 2024
// File Dependencies / Libraries: SafeboxConfig.h, SafeboxFunction.h,
//                                SafeboxInitialization.h
// Compiler: xc8, 2.46
// Author: Alexander Pagaduan
// Versions:
//       V1.0: Initial Draft
// Useful links: 
//       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
//       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
//       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 


//---------------------
// INITIALIZATION
//---------------------
#include <xc.h> // must have this
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LcdInterfaceConfig.h"


#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

#define RS LATD0                   /* PORTD 0 pin is used for Register Select */
#define EN LATD1                   /* PORTD 1 pin is used for Enable */
#define ldata LATB                 /* PORTB is used for transmitting data to LCD */

#define LCD_Port TRISB              
#define LCD_Control TRISD

void LCD_Init(void);
void LCD_Command(char );
void LCD_Clear(void);
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int );


#define Vref 5.0 // voltage reference 
int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
char data[10];
void ADC_Init(void);
float lux_m = -99.24;
float lux_b = 393.00;
float lux_val = 0.0;


/*****************************Main Program*******************************/

/*void main(void)
{       
    //OSCCON=0x72;                   /* Use Internal Oscillator with Frequency 8MHZ */ 
  //  LCD_Init();                    /* Initialize 16x2 LCD */
 //   LCD_String_xy(1,0,"Alexander");    /* Display string at location(row,location). */
                                   /* This function passes string to display */
  //  LCD_String_xy(2,0,"Pagaduan");   /*Display string at location(row,location). */
                                   /* This function passes string to display */    
    
   // while(1);           
//}

void main() {
    //ADC Initialization
    ADC_Init();
    //LCD Initialization
    LCD_Init();
    LCD_Clear();
    while (1) {
        ADCON0bits.GO = 1; //Start conversion
        while (ADCON0bits.GO); //Wait for conversion done
        digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
        voltage= digital*((float)Vref/(float)(4096)); 
        
        //print on LCD 
        /*It is used to convert integer value to ASCII string*/
        //sprintf(data,"%.2f",voltage);
        //strcat(data," V");
        //LCD_String_xy(1,0,"TheInputVoltage:"); 
        //LCD_String_xy(2,0,data);/*Send string data for printing*/
        
        lux_val = (((lux_m) * ((float)voltage)) + lux_b);
                
        sprintf(data,"%f",lux_val);
        strcat(data," LUX");
        LCD_String_xy(1,0,"TheInputLight:"); 
        LCD_String_xy(2,0,data);/*Send string data for printing*/
      
        
        MSdelay(1000);
    }
}

/****************************Functions********************************/
void LCD_Init()
{
    MSdelay(15);           /* 15ms,16x2 LCD Power on delay */
    LCD_Port = 0x00;       /* Set PORTB as output PORT for LCD data(D0-D7) pins */
    LCD_Control = 0x00;    /* Set PORTD as output PORT LCD Control(RS,EN) Pins */
    LCD_Command(0x01);     /* clear display screen */
    LCD_Command(0x38);     /* uses 2 line and initialize 5*7 matrix of LCD */
    LCD_Command(0x0c);     /* display on cursor off */
    LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
}

void LCD_Clear()
{
        LCD_Command(0x01); /* clear display screen */
}

void LCD_Command(char cmd )
{
    ldata= cmd;            /* Send data to PORT as a command for LCD */   
    RS = 0;                /* Command Register is selected */
    EN = 1;                /* High-to-Low pulse on Enable pin to latch data */ 
    NOP();
    EN = 0;
    MSdelay(3); 
}

void LCD_Char(char dat)
{
    ldata= dat;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    EN=1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    EN=0;
    MSdelay(1);
}


void LCD_String(const char *msg)
{
    while((*msg)!=0)
    {       
      LCD_Char(*msg);
      msg++;    
        }
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);

}
/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}

void ADC_Init(void)
{
       //Setup ADC
    ADCON0bits.FM = 1;  //right justify
    ADCON0bits.CS = 1; //ADCRC Clock
    
    TRISAbits.TRISA0 = 1; //Set RA0 to input
    ANSELAbits.ANSELA0 = 1; //Set RA0 to analog
    // Added 
    ADPCH = 0x00; //Set RA0 as Analog channel in ADC ADPCH
    ADCLK = 0x00; //set ADC CLOCK Selection register to zero
    
    ADRESH = 0x00; // Clear ADC Result registers
    ADRESL = 0x00; 
    
    ADPREL = 0x00; // set precharge select to 0 in register ADPERL & ADPERH
    ADPREH = 0x00; 
    
    ADACQL = 0x00;  // set acquisition low and high byte to zero 
    ADACQH = 0x00;    
    
    ADCON0bits.ON = 1; //Turn ADC On 
}
