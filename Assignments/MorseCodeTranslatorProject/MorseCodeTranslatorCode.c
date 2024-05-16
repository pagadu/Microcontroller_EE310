 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0alexa
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

//---------------------
// INITIALIZATION
//---------------------
#include "mcc_generated_files/system/system.h"
#include<stdio.h>
#include<stdlib.h>
#include <xc.h>
#include "LcdInterfaceConfig.h"


//#define PIC1
#ifndef PIC1
#define PIC2
#endif // PIC1


// Define macros for GPIO ports
#define BUTTON_PORT PORTA
#define BUTTON_TRIS TRISA

#define KEYPAD_PORT PORTB
#define KEYPAD_TRIS TRISB

#define MOTION_PORT PORTC
#define MOTION_TRIS TRISC

//#define MOTION_THRESH 10 // MORE SENSITIVE
#define MOTION_THRESH 100 // LESS SENSITIVE

// Define macros for GPIO ports
#define MISC_PORT  PORTA
#define MISC_TRIS  TRISA
#define MISC_LAT   LATA
#define MISC_ANSEL ANSELA


// Define macros for keypad buttons
#define ROWS 4
#define COLS 4

//This is not used but it is a good visual of the keypad
const char keypad[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//This is equivalent to the one above but is in decimal.
//This is used
const int keypad_dec[ROWS][COLS] = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {14, 0, 15, 13}
};

//This is equivalent to the one above but is in hex-decimal.
//This is used
const int keypad_dis[ROWS][COLS] = {
    {0x06, 0x5B, 0x4F, 0x77},
    {0x66, 0x6D, 0x7D, 0x7C},
    {0x07, 0x7F, 0x67, 0x39},
    {0x79, 0x3f, 0x71, 0x5E}
};


//---------------------
// FUNCTIONS
//---------------------
int readKeypad();
int readButtons();
int readMotion();
void displayResult(int result);
void configure_ports();
void delay(int num1, int num2);
void motor(int num1, int num2);
void buzzer(int num1, int num2);
void melody(int num1, int num2);

// Global variables
volatile uint8_t X_Input_REG = 0; //first input
volatile uint8_t Y_Input_REG = 0; //second input
volatile char Operation_REG = ' '; // operand var
volatile int Display_Result_REG = 0; // Result value to be placed on LEDs
volatile int button_pressed = 0; // Used to wait for button to be depressed

void updateMisc(int input_val);

int reset = 0; // reset flag if "*" is pressed
int key1 = 0; // stores the first input
int op = 0; // stores operand
int key2 = 0; // stores the second input
int result = 0; // result flag if "#" is pressed
int button1 = 0; // stores the first input
int motion1 = 0; // stores the first input
int uart1_char = 0;
int uart2_char = 0;
char data[16];
int MISC_bit7;
int MISC_bit6;
int MISC_bit5;
int MISC_bit4;
int MISC_bit3;
int MISC_bit2;
int MISC_bit1;
int MISC_bit0;
int MISC_val;

int MorseCode[5];
int MorseCodeLength = 0;
char MorseAscii[32];
int MorseAsciiLength = 0;

int MorseCodeAscii_A[] = {1, 2};
int MorseCodeAscii_A_len = 2;

int MorseCodeAscii_B[] = {2, 1, 1, 1};
int MorseCodeAscii_B_len = 4;

int MorseCodeAscii_C[] = {2, 1, 2, 1};
int MorseCodeAscii_C_len = 4;

int MorseCodeAscii_D[] = {2, 1, 1};
int MorseCodeAscii_D_len = 3;

int MorseCodeAscii_E[] = {1};
int MorseCodeAscii_E_len = 1;

int MorseCodeAscii_F[] = {1, 1, 2, 1};
int MorseCodeAscii_F_len = 4;

int MorseCodeAscii_G[] = {2, 2, 1};
int MorseCodeAscii_G_len = 3;

int MorseCodeAscii_H[] = {1, 1, 1, 1};
int MorseCodeAscii_H_len = 4;

int MorseCodeAscii_I[] = {1, 1};
int MorseCodeAscii_I_len = 2;

int MorseCodeAscii_J[] = {1, 2, 2, 2};
int MorseCodeAscii_J_len = 4;

int MorseCodeAscii_K[] = {2, 1, 2};
int MorseCodeAscii_K_len = 3;

int MorseCodeAscii_L[] = {1, 2, 1, 1};
int MorseCodeAscii_L_len = 4;

int MorseCodeAscii_M[] = {2, 2};
int MorseCodeAscii_M_len = 2;

int MorseCodeAscii_N[] = {2, 1};
int MorseCodeAscii_N_len = 2;

int MorseCodeAscii_O[] = {2, 2, 2};
int MorseCodeAscii_O_len = 3;

int MorseCodeAscii_P[] = {1, 2, 2, 1};
int MorseCodeAscii_P_len = 4;

int MorseCodeAscii_Q[] = {2, 2, 1, 2};
int MorseCodeAscii_Q_len = 4;

int MorseCodeAscii_R[] = {1, 2, 1};
int MorseCodeAscii_R_len = 3;

int MorseCodeAscii_S[] = {1, 1, 1};
int MorseCodeAscii_S_len = 3;

int MorseCodeAscii_T[] = {2};
int MorseCodeAscii_T_len = 1;

int MorseCodeAscii_U[] = {1, 1, 2};
int MorseCodeAscii_U_len = 3;

int MorseCodeAscii_V[] = {1, 1, 1, 2};
int MorseCodeAscii_V_len = 4;

int MorseCodeAscii_W[] = {1, 2, 2};
int MorseCodeAscii_W_len = 3;

int MorseCodeAscii_X[] = {2, 1, 1, 2};
int MorseCodeAscii_X_len = 4;

int MorseCodeAscii_Y[] = {2, 1, 2, 2};
int MorseCodeAscii_Y_len = 4;

int MorseCodeAscii_Z[] = {2, 2, 1, 1};
int MorseCodeAscii_Z_len = 4;

int MorseCodeAscii_1[] = {1, 2, 2, 2, 2};
int MorseCodeAscii_1_len = 5;

int MorseCodeAscii_2[] = {1, 1, 2, 2, 2};
int MorseCodeAscii_2_len = 5;

int MorseCodeAscii_3[] = {1, 1, 1, 2, 2};
int MorseCodeAscii_3_len = 5;

int MorseCodeAscii_4[] = {1, 1, 1, 1, 2};
int MorseCodeAscii_4_len = 5;

int MorseCodeAscii_5[] = {1, 1, 1, 1, 1};
int MorseCodeAscii_5_len = 5;

int MorseCodeAscii_6[] = {2, 1, 1, 1, 1};
int MorseCodeAscii_6_len = 5;

int MorseCodeAscii_7[] = {2, 2, 1, 1, 1};
int MorseCodeAscii_7_len = 5;

int MorseCodeAscii_8[] = {2, 2, 2, 1, 1};
int MorseCodeAscii_8_len = 5;

int MorseCodeAscii_9[] = {2, 2, 2, 2, 1};
int MorseCodeAscii_9_len = 5;

int MorseCodeAscii_0[] = {2, 2, 2, 2, 2};
int MorseCodeAscii_0_len = 5;

//Plconst aceholder if we want to add delays
//Didn't need to use it
void delay(int num1, int num2) {
   int cnt1, cnt2;
   for (cnt1=0; cnt1<num1; cnt1++)
       for (cnt2=0; cnt2<num2; cnt2++)
       {}
}

//Reads the keypad
int readKeypad() {
    int row, col; 
    int keypad_val;
    
    //Cycle through the columns
    for (col = 0; col < COLS; col++) {
        //clear all columns
        //KEYPAD_PORT = ((0b00000000) & 0b11110000);
        PORTBbits.RB4 = 0;
        PORTBbits.RB5 = 0;
        PORTBbits.RB6 = 0;
        PORTBbits.RB7 = 0;
        delay(1, 10);
        
        //Set only 1 column based on COL variable
        //KEYPAD_PORT = ((0b00010000 << col) & 0b11110000);
        PORTBbits.RB4 = (col == 0);
        PORTBbits.RB5 = (col == 1);
        PORTBbits.RB6 = (col == 2);
        PORTBbits.RB7 = (col == 3);
        delay(1, 10);
        
        //Read keypad rows 
        keypad_val = KEYPAD_PORT & 0b00001111;
        
        //Cycle through the rows
        for (row = 0; row < ROWS; row++) {
            //check to see if row is detected
            if ((keypad_val) == (0b00000001 << row)) {
                //Detected button pressed
                //LED_PORT = ((keypad_dec[row][col]) & 0b00001111); // DEBUG
                button_pressed = 1;
                
                //KEYPAD_PORT = 0b11110000;
                delay(1,10);
                
                // Wait for button to be depressed
                while (button_pressed == 1) { 
                    keypad_val = KEYPAD_PORT & 0b00001111;
                    //if keypad is zero then the button was depressed
                    if (keypad_val == 0) {
                        button_pressed = 0;
                    }
                    delay(1,10);
                }
                
                delay(1,10);
                //returns keypad decimal value
                return keypad_dec[row][col];
            }
        }
    }
    //reserved 16 as no button pressed
    return 16; // No key pressed
}
//Reads the buttons
int readButtons() {

        if(PORTAbits.RA0 == 1){
            while (PORTAbits.RA0 == 1) {}
            return 20;
        }
        else if (PORTAbits.RA1 == 1){
            while (PORTAbits.RA1 == 1) {}
            return 21;
        }
        else if (PORTAbits.RA2 == 1){
            while (PORTAbits.RA2 == 1) {}
            return 22;
        }
        else if (PORTAbits.RA3 == 1){
            while (PORTAbits.RA3 == 1) {}
            return 23;
        }
        return 24;
}
    
//Reads the MotionDetector
int readMotion() {
        int cnt1;
        int cnt2;
        int cnt3;
        int cnt4;
        int motion1_cnt;
        int motion2_cnt;
        int motion3_cnt;
        int motion4_cnt;
        int diff_cnt;
        
        delay(1,10);
        PORTAbits.RA5 = 0;
        delay(1,10);
        PORTAbits.RA5 = 1;
        delay(1,10);
        PORTAbits.RA5 = 0;    
        cnt1 = 0;
        cnt2 = 0;
        while ((PORTAbits.RA4 == 0) && (cnt1 < 10000)) {
            cnt1++;
            //delay(1,10);
        }
        while ((PORTAbits.RA4 == 1) && (cnt2 < 10000)) {
            cnt2++;
            //delay(1,10);
        }
        //printf("cnt1 = %d \r\n", cnt1);
        //printf("cnt2 = %d \r\n", cnt2);
        
        delay(1,10);
        PORTAbits.RA5 = 0;
        delay(1,10);
        PORTAbits.RA5 = 1;
        delay(1,10);
        PORTAbits.RA5 = 0;    
        cnt3 = 0;
        cnt4 = 0;
        while ((PORTAbits.RA4 == 0) && (cnt3 < 10000)) {
            cnt3++;
            //delay(1,10);
        }
        while ((PORTAbits.RA4 == 1) && (cnt4 < 10000)) {
            cnt4++;
            //delay(1,10);
        }
        //printf("cnt3 = %d \r\n", cnt3);
        //printf("cnt4 = %d \r\n", cnt4);

        motion1_cnt = cnt1;
        motion2_cnt = cnt2;
        motion3_cnt = cnt3;
        motion4_cnt = cnt4;
       
        if (motion2_cnt == motion4_cnt) {
            return 31;
        }
        if (motion2_cnt > motion4_cnt) {
            if (motion2_cnt > motion4_cnt + MOTION_THRESH) {
                return 30;
            }
        }
        if (motion4_cnt > motion2_cnt) {
            if (motion4_cnt > motion2_cnt + MOTION_THRESH) {
                return 30;
            }
        }
        return 31;
}
    

void displayResult(int result) {
    // Convert result to binary and display on LEDs
    //LED_PORT = (unsigned char) result;
    //delay(1,10);
}

void updateMisc(int input_val) {
    // Convert result to binary and display on LEDs
    MISC_PORT = (unsigned char) input_val;
    delay(1,10);
}

void motor(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
       for (cnt2=0; cnt2<num2; cnt2++) {
           PORTAbits.RA4 = ((cnt2 < 1)) ;
       }
    }
}

void buzzer(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
       for (cnt2=0; cnt2<num2; cnt2++) {
           PORTAbits.RA4 = cnt1 & 0b00000001;
       }
    }
}

void melody(int num1, int num2) {
    int cnt1, cnt2;
    for (cnt1=0; cnt1<num1; cnt1++) {
        buzzer(10,100);
        delay (10,100);
        buzzer(4,100);
        delay (4,100);
        buzzer(20,100);
        delay (4,100);
    }
}


/////////////////////////////////////////////
//CONFIGURE PORTS
void configure_ports_pic1() {
    // Initialize ports for keypad and LEDs
    PORTA = 0;
    LATA = 0;
    ANSELA = 0;
    BUTTON_TRIS = 0b00011111; // Set all pins as output
    
    PORTB = 0;
    LATB = 0;
    ANSELB = 0;
    KEYPAD_TRIS = 0b00001111; // Set first 4 pins as input
    
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    MOTION_TRIS = 0b00001001; // Set all pins as output
}

void configure_ports_pic2() {
    // Initialize ports for 7-SEGMENT DISPLAY
    MISC_PORT = 0;
    MISC_LAT = 0;
    MISC_ANSEL = 0;
    MISC_TRIS = 0b00000000; // Outputs for 7 Segment Display
    
    PORTC = 0;
    LATC = 0;
    ANSELC = 0;
    TRISC = 0b00000000; // Set all pins as output
}

bool MorseodeMatch(int in_array1[], int in_array2[], int length1, int length2)
{
    //printf("MorseodeMatch: sizeof(in_array1)=%d sizeof(in_array2)=%d length1=%d length2=%d \r\n", sizeof(in_array1), sizeof(in_array2), length1, length2);
    if (length1 != length2) {
        printf("MorseodeMatch: length mismatch \r\n");
        return false;
    }
    for (int cnt = 0; cnt < length2; cnt++) {
        if (in_array1[cnt] != in_array2[cnt]) {
            printf("MorseodeMatch: content mismatch \r\n");
            return false;
        }
    }
    // If all elements were same.
    return true;
}
 
int main(void)
{
    SYSTEM_Initialize();
    
#ifdef PIC1
    configure_ports_pic1();
#endif // PIC1
    
#ifdef PIC2
    configure_ports_pic2();
#endif // PIC2
    
    UART2_Initialize();
    UART1_Initialize();
    
#ifdef PIC2
    LCD_Init();
    LCD_Clear();
    updateMisc(0x00);
#endif // PIC2
    
    key1 = 0;
    button1 = 0;
    motion1 = 0;
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    //INTERRUPT_GlobalInterruptEnable(); 

    // Disable the Global Interrupts 
    //INTERRUPT_GlobalInterruptDisable(); 

    printf("\n\nStart Main Loop \r\n");
    UART1_Read();
    UART2_Read();
    
    while(1)
    {
#ifdef PIC1
        key1 = readKeypad();
        if (key1 < 16){
            printf("Read Keypad = %d \r\n", key1);
              
            if((UART1_IsTxReady())) {
                UART1_Write(key1);
                printf("Send to UART1_TX keypad =%d \r\n", key1);
            }
        }
        
        button1 = readButtons();
        if ((button1 >= 20) && (button1 < 24)) {
            printf("Read Buttons = %d \r\n", button1);
              
            if((UART1_IsTxReady())) {
                UART1_Write(button1);
                printf("Send to UART1_TX button =%d \r\n", button1);
            }
        }
        motion1 = readMotion();
        if ((motion1 >= 30) && (motion1 < 31)) {
            printf("Read Motion = %d \r\n", motion1);
                   
            if((UART1_IsTxReady())) {
                UART1_Write(motion1);
                printf("Send to UART1_TX motion =%d \r\n", motion1);
            }
        }
#endif // PIC1
        
        if((UART2_IsRxReady())) {
            uart2_char = UART2_Read();
            printf("uart2_char =%d \r\n", uart2_char);

            if((UART1_IsTxReady())) {
                UART1_Write(uart2_char);
                printf("Send to UART1_TX uart2_char =%d \r\n", uart2_char);
            }

#ifdef PIC2
            sprintf(data,"%d",uart2_char);
            LCD_Clear();
            LCD_String_xy(1,0,"UART2_CHAR:"); 
            LCD_String_xy(2,0,data);/*Send string data for printing*/
            //MSdelay(1000);  
#endif // PIC2  
        }
        
        if((UART1_IsRxReady())) {
            uart1_char = UART1_Read();
            printf("Receive from UART1_RX uart1_char =%d \r\n", uart1_char);
            
#ifdef PIC2
            
            if((uart1_char >= 0) && (uart1_char < 16)) {
                //sprintf(data,"%d",uart1_char);
                //LCD_Clear();
                //LCD_String_xy(1,0,"UART1_CHAR:"); 
                //LCD_String_xy(2,0,data);/*Send string data for printing*/
                if (MorseAsciiLength == 32) {
                    for (int morse_cnt=0; morse_cnt<32; morse_cnt++) {
                        MorseAscii[morse_cnt] = MorseAscii[morse_cnt + 1];
                    }
                    MorseAsciiLength = 31;
                }
                if (uart1_char == 0) {
                    MorseAscii[MorseAsciiLength] = '0';
                }
                else if (uart1_char == 1) {
                    MorseAscii[MorseAsciiLength] = '1';
                }
                else if (uart1_char == 2) {
                    MorseAscii[MorseAsciiLength] = '2';
                }
                else if (uart1_char == 3) {
                    MorseAscii[MorseAsciiLength] = '3';
                }
                else if (uart1_char == 4) {
                    MorseAscii[MorseAsciiLength] = '4';    
                }
                else if (uart1_char == 5) {
                    MorseAscii[MorseAsciiLength] = '5';
                }
                else if (uart1_char == 6) {
                    MorseAscii[MorseAsciiLength] = '6';
                }
                else if (uart1_char == 7) {
                    MorseAscii[MorseAsciiLength] = '7';
                }
                else if (uart1_char == 8) {
                    MorseAscii[MorseAsciiLength] = '8';
                }
                else if (uart1_char == 9) {
                    MorseAscii[MorseAsciiLength] = '9';
                }
                else if (uart1_char == 10) {
                    MorseAscii[MorseAsciiLength] = 'A';
                }
                else if (uart1_char == 11) {
                    MorseAscii[MorseAsciiLength] = 'B';
                }
                else if (uart1_char == 12) {
                    MorseAscii[MorseAsciiLength] = 'C';
                }
                else if (uart1_char == 13) {
                    MorseAscii[MorseAsciiLength] = 'D';
                }
                else if (uart1_char == 14) {
                    MorseAscii[MorseAsciiLength] = '*';
                }
                else if (uart1_char == 15) {
                    MorseAscii[MorseAsciiLength] = '#';
                }
                MorseAsciiLength++;
                
                    LCD_Clear();
                    
                    sprintf(data,"");
                    for (int morse_cnt=0; morse_cnt<16; morse_cnt++) {
                        sprintf(data,"%s%c",data,MorseAscii[morse_cnt]);
                    }
                    printf("data_0_15=%s \r\n", data);
                    LCD_String_xy(1,0,data);
                    
                    sprintf(data,"");
                    for (int morse_cnt=16; morse_cnt<32; morse_cnt++) {
                        sprintf(data,"%s%c",data,MorseAscii[morse_cnt]);
                    }
                    printf("data_16_32=%s \r\n", data);
                    LCD_String_xy(2,0,data);/*Send string data for printing*/ 
                
            }
            if((uart1_char >= 20) && (uart1_char < 24)) {
                //sprintf(data,"%d",uart1_char);
                //LCD_Clear();
                //LCD_String_xy(1,0,"UART1_CHAR:"); 
                //LCD_String_xy(2,0,data);/*Send string data for printing*/
                
                // LED
                if (uart1_char == 20) {
                    updateMisc(0x01);
                }
                if (uart1_char == 21) {
                    updateMisc(0x02);
                }
                if (uart1_char == 22) {
                    updateMisc(0x04);
                }
                if (uart1_char == 23) {
                    updateMisc(0x08);
                }
                delay(1,1000);
                updateMisc(0x00);
                
                // BUZZER
                if (uart1_char == 20) {
                    buzzer(10,100);
                }
                if (uart1_char == 21) {
                    buzzer(10,100);
                    buzzer(10,100);
                    buzzer(10,100);
                }
                if (uart1_char == 22) {
                    //buzzer(10,100);
                }
                if (uart1_char == 23) {
                    //buzzer(10,100);
                }
                
                // ACTION
                
                if ((uart1_char >= 20) && (uart1_char < 23)) {
                    if (MorseAsciiLength == 32) {
                        for (int morse_cnt=0; morse_cnt<32; morse_cnt++) {
                            MorseAscii[morse_cnt] = MorseAscii[morse_cnt + 1];
                        }
                        MorseAsciiLength = 31;
                    }
                    if (uart1_char == 20) {       
                        MorseCode[MorseCodeLength] = 1;
                        MorseCodeLength++;
                    }
                    if (uart1_char == 21) {
                        MorseCode[MorseCodeLength] = 2;
                        MorseCodeLength++;
                    }
                    if (uart1_char == 22) {
                        //printf("MorseCodeLength=%d\r\n", MorseCodeLength);
                        //printf("MorseCodeSize=%d\r\n", sizeof(MorseCode));
                        //printf("MorseCode[0]=%d\r\n", MorseCode[0]);
                        //printf("MorseCode[1]=%d\r\n", MorseCode[1]);
                        //printf("MorseCode[2]=%d\r\n", MorseCode[2]);
                        //printf("MorseCode[3]=%d\r\n", MorseCode[3]);
                        //printf("MorseCode[4]=%d\r\n", MorseCode[4]);
                        
                        /*
                        printf("MorseCodeAscii_A[0]=%d\r\n", MorseCodeAscii_A[0]);
                        printf("MorseCodeAscii_A[1]=%d\r\n", MorseCodeAscii_A[1]);
                        printf("MorseCodeAscii_A[2]=%d\r\n", MorseCodeAscii_A[2]);
                        printf("MorseCodeAscii_A[3]=%d\r\n", MorseCodeAscii_A[3]);
                        printf("MorseCodeAscii_A[4]=%d\r\n", MorseCodeAscii_A[4]);
             
                        printf("MorseCodeAscii_B[0]=%d\r\n", MorseCodeAscii_B[0]);
                        printf("MorseCodeAscii_B[1]=%d\r\n", MorseCodeAscii_B[1]);
                        printf("MorseCodeAscii_B[2]=%d\r\n", MorseCodeAscii_B[2]);
                        printf("MorseCodeAscii_B[3]=%d\r\n", MorseCodeAscii_B[3]);
                        printf("MorseCodeAscii_B[4]=%d\r\n", MorseCodeAscii_B[4]);
                        
                        printf("MorseCodeAscii_C[0]=%d\r\n", MorseCodeAscii_C[0]);
                        printf("MorseCodeAscii_C[1]=%d\r\n", MorseCodeAscii_C[1]);
                        printf("MorseCodeAscii_C[2]=%d\r\n", MorseCodeAscii_C[2]);
                        printf("MorseCodeAscii_C[3]=%d\r\n", MorseCodeAscii_C[3]);
                        printf("MorseCodeAscii_C[4]=%d\r\n", MorseCodeAscii_C[4]);
             
                        printf("MorseCodeAscii_D[0]=%d\r\n", MorseCodeAscii_D[0]);
                        printf("MorseCodeAscii_D[1]=%d\r\n", MorseCodeAscii_D[1]);
                        printf("MorseCodeAscii_D[2]=%d\r\n", MorseCodeAscii_D[2]);
                        printf("MorseCodeAscii_D[3]=%d\r\n", MorseCodeAscii_D[3]);
                        printf("MorseCodeAscii_D[4]=%d\r\n", MorseCodeAscii_D[4]);
                        
                        printf("MorseCodeAscii_F[0]=%d\r\n", MorseCodeAscii_F[0]);
                        printf("MorseCodeAscii_F[1]=%d\r\n", MorseCodeAscii_F[1]);
                        printf("MorseCodeAscii_F[2]=%d\r\n", MorseCodeAscii_F[2]);
                        printf("MorseCodeAscii_F[3]=%d\r\n", MorseCodeAscii_F[3]);
                        printf("MorseCodeAscii_F[4]=%d\r\n", MorseCodeAscii_F[4]);
                        
                        printf("MorseCodeAscii_G[0]=%d\r\n", MorseCodeAscii_G[0]);
                        printf("MorseCodeAscii_G[1]=%d\r\n", MorseCodeAscii_G[1]);
                        printf("MorseCodeAscii_G[2]=%d\r\n", MorseCodeAscii_G[2]);
                        printf("MorseCodeAscii_G[3]=%d\r\n", MorseCodeAscii_G[3]);
                        printf("MorseCodeAscii_G[4]=%d\r\n", MorseCodeAscii_G[4]);
                        
                        printf("MorseCodeAscii_H[0]=%d\r\n", MorseCodeAscii_H[0]);
                        printf("MorseCodeAscii_H[1]=%d\r\n", MorseCodeAscii_H[1]);
                        printf("MorseCodeAscii_H[2]=%d\r\n", MorseCodeAscii_H[2]);
                        printf("MorseCodeAscii_H[3]=%d\r\n", MorseCodeAscii_H[3]);
                        printf("MorseCodeAscii_H[4]=%d\r\n", MorseCodeAscii_H[4]);
                        
                        printf("MorseCodeAscii_I[0]=%d\r\n", MorseCodeAscii_I[0]);
                        printf("MorseCodeAscii_I[1]=%d\r\n", MorseCodeAscii_I[1]);
                        printf("MorseCodeAscii_I[2]=%d\r\n", MorseCodeAscii_I[2]);
                        printf("MorseCodeAscii_I[3]=%d\r\n", MorseCodeAscii_I[3]);
                        printf("MorseCodeAscii_I[4]=%d\r\n", MorseCodeAscii_I[4]);
                        
                        printf("MorseCodeAscii_J[0]=%d\r\n", MorseCodeAscii_J[0]);
                        printf("MorseCodeAscii_J[1]=%d\r\n", MorseCodeAscii_J[1]);
                        printf("MorseCodeAscii_J[2]=%d\r\n", MorseCodeAscii_J[2]);
                        printf("MorseCodeAscii_J[3]=%d\r\n", MorseCodeAscii_J[3]);
                        printf("MorseCodeAscii_J[4]=%d\r\n", MorseCodeAscii_J[4]);
                        
                        printf("MorseCodeAscii_K[0]=%d\r\n", MorseCodeAscii_K[0]);
                        printf("MorseCodeAscii_K[1]=%d\r\n", MorseCodeAscii_K[1]);
                        printf("MorseCodeAscii_K[2]=%d\r\n", MorseCodeAscii_K[2]);
                        printf("MorseCodeAscii_K[3]=%d\r\n", MorseCodeAscii_K[3]);
                        printf("MorseCodeAscii_K[4]=%d\r\n", MorseCodeAscii_K[4]);
                        
                        printf("MorseCodeAscii_L[0]=%d\r\n", MorseCodeAscii_L[0]);
                        printf("MorseCodeAscii_L[1]=%d\r\n", MorseCodeAscii_L[1]);
                        printf("MorseCodeAscii_L[2]=%d\r\n", MorseCodeAscii_L[2]);
                        printf("MorseCodeAscii_L[3]=%d\r\n", MorseCodeAscii_L[3]);
                        printf("MorseCodeAscii_L[4]=%d\r\n", MorseCodeAscii_L[4]);
                        
                        printf("MorseCodeAscii_M[0]=%d\r\n", MorseCodeAscii_M[0]);
                        printf("MorseCodeAscii_M[1]=%d\r\n", MorseCodeAscii_M[1]);
                        printf("MorseCodeAscii_M[2]=%d\r\n", MorseCodeAscii_M[2]);
                        printf("MorseCodeAscii_M[3]=%d\r\n", MorseCodeAscii_M[3]);
                        printf("MorseCodeAscii_M[4]=%d\r\n", MorseCodeAscii_M[4]);
                        
                        printf("MorseCodeAscii_N[0]=%d\r\n", MorseCodeAscii_N[0]);
                        printf("MorseCodeAscii_N[1]=%d\r\n", MorseCodeAscii_N[1]);
                        printf("MorseCodeAscii_N[2]=%d\r\n", MorseCodeAscii_N[2]);
                        printf("MorseCodeAscii_N[3]=%d\r\n", MorseCodeAscii_N[3]);
                        printf("MorseCodeAscii_N[4]=%d\r\n", MorseCodeAscii_N[4]);
                        
                        printf("MorseCodeAscii_O[0]=%d\r\n", MorseCodeAscii_O[0]);
                        printf("MorseCodeAscii_O[1]=%d\r\n", MorseCodeAscii_O[1]);
                        printf("MorseCodeAscii_O[2]=%d\r\n", MorseCodeAscii_O[2]);
                        printf("MorseCodeAscii_O[3]=%d\r\n", MorseCodeAscii_O[3]);
                        printf("MorseCodeAscii_O[4]=%d\r\n", MorseCodeAscii_O[4]);
                        
                        printf("MorseCodeAscii_P[0]=%d\r\n", MorseCodeAscii_P[0]);
                        printf("MorseCodeAscii_P[1]=%d\r\n", MorseCodeAscii_P[1]);
                        printf("MorseCodeAscii_P[2]=%d\r\n", MorseCodeAscii_P[2]);
                        printf("MorseCodeAscii_P[3]=%d\r\n", MorseCodeAscii_P[3]);
                        printf("MorseCodeAscii_P[4]=%d\r\n", MorseCodeAscii_P[4]);
                        
                        printf("MorseCodeAscii_Q[0]=%d\r\n", MorseCodeAscii_Q[0]);
                        printf("MorseCodeAscii_Q[1]=%d\r\n", MorseCodeAscii_Q[1]);
                        printf("MorseCodeAscii_Q[2]=%d\r\n", MorseCodeAscii_Q[2]);
                        printf("MorseCodeAscii_Q[3]=%d\r\n", MorseCodeAscii_Q[3]);
                        printf("MorseCodeAscii_Q[4]=%d\r\n", MorseCodeAscii_Q[4]);
                        
                        printf("MorseCodeAscii_R[0]=%d\r\n", MorseCodeAscii_R[0]);
                        printf("MorseCodeAscii_R[1]=%d\r\n", MorseCodeAscii_R[1]);
                        printf("MorseCodeAscii_R[2]=%d\r\n", MorseCodeAscii_R[2]);
                        printf("MorseCodeAscii_R[3]=%d\r\n", MorseCodeAscii_R[3]);
                        printf("MorseCodeAscii_R[4]=%d\r\n", MorseCodeAscii_R[4]);
                        
                        printf("MorseCodeAscii_S[0]=%d\r\n", MorseCodeAscii_S[0]);
                        printf("MorseCodeAscii_S[1]=%d\r\n", MorseCodeAscii_S[1]);
                        printf("MorseCodeAscii_S[2]=%d\r\n", MorseCodeAscii_S[2]);
                        printf("MorseCodeAscii_S[3]=%d\r\n", MorseCodeAscii_S[3]);
                        printf("MorseCodeAscii_S[4]=%d\r\n", MorseCodeAscii_S[4]);
                        
                        printf("MorseCodeAscii_T[0]=%d\r\n", MorseCodeAscii_T[0]);
                        printf("MorseCodeAscii_T[1]=%d\r\n", MorseCodeAscii_T[1]);
                        printf("MorseCodeAscii_T[2]=%d\r\n", MorseCodeAscii_T[2]);
                        printf("MorseCodeAscii_T[3]=%d\r\n", MorseCodeAscii_T[3]);
                        printf("MorseCodeAscii_T[4]=%d\r\n", MorseCodeAscii_T[4]);
                        
                        printf("MorseCodeAscii_U[0]=%d\r\n", MorseCodeAscii_U[0]);
                        printf("MorseCodeAscii_U[1]=%d\r\n", MorseCodeAscii_U[1]);
                        printf("MorseCodeAscii_U[2]=%d\r\n", MorseCodeAscii_U[2]);
                        printf("MorseCodeAscii_U[3]=%d\r\n", MorseCodeAscii_U[3]);
                        printf("MorseCodeAscii_U[4]=%d\r\n", MorseCodeAscii_U[4]);
                        
                        printf("MorseCodeAscii_V[0]=%d\r\n", MorseCodeAscii_V[0]);
                        printf("MorseCodeAscii_V[1]=%d\r\n", MorseCodeAscii_V[1]);
                        printf("MorseCodeAscii_V[2]=%d\r\n", MorseCodeAscii_V[2]);
                        printf("MorseCodeAscii_V[3]=%d\r\n", MorseCodeAscii_V[3]);
                        printf("MorseCodeAscii_V[4]=%d\r\n", MorseCodeAscii_V[4]);
                        
                        printf("MorseCodeAscii_W[0]=%d\r\n", MorseCodeAscii_W[0]);
                        printf("MorseCodeAscii_W[1]=%d\r\n", MorseCodeAscii_W[1]);
                        printf("MorseCodeAscii_W[2]=%d\r\n", MorseCodeAscii_W[2]);
                        printf("MorseCodeAscii_W[3]=%d\r\n", MorseCodeAscii_W[3]);
                        printf("MorseCodeAscii_W[4]=%d\r\n", MorseCodeAscii_W[4]);
                        
                        printf("MorseCodeAscii_X[0]=%d\r\n", MorseCodeAscii_X[0]);
                        printf("MorseCodeAscii_X[1]=%d\r\n", MorseCodeAscii_X[1]);
                        printf("MorseCodeAscii_X[2]=%d\r\n", MorseCodeAscii_X[2]);
                        printf("MorseCodeAscii_X[3]=%d\r\n", MorseCodeAscii_X[3]);
                        printf("MorseCodeAscii_X[4]=%d\r\n", MorseCodeAscii_X[4]);
                        
                        printf("MorseCodeAscii_Y[0]=%d\r\n", MorseCodeAscii_Y[0]);
                        printf("MorseCodeAscii_Y[1]=%d\r\n", MorseCodeAscii_Y[1]);
                        printf("MorseCodeAscii_Y[2]=%d\r\n", MorseCodeAscii_Y[2]);
                        printf("MorseCodeAscii_Y[3]=%d\r\n", MorseCodeAscii_Y[3]);
                        printf("MorseCodeAscii_Y[4]=%d\r\n", MorseCodeAscii_Y[4]);
                        
                        printf("MorseCodeAscii_Z[0]=%d\r\n", MorseCodeAscii_Z[0]);
                        printf("MorseCodeAscii_Z[1]=%d\r\n", MorseCodeAscii_Z[1]);
                        printf("MorseCodeAscii_Z[2]=%d\r\n", MorseCodeAscii_Z[2]);
                        printf("MorseCodeAscii_Z[3]=%d\r\n", MorseCodeAscii_Z[3]);
                        printf("MorseCodeAscii_Z[4]=%d\r\n", MorseCodeAscii_Z[4]);
                        
                        printf("MorseCodeAscii_1[0]=%d\r\n", MorseCodeAscii_1[0]);
                        printf("MorseCodeAscii_1[1]=%d\r\n", MorseCodeAscii_1[1]);
                        printf("MorseCodeAscii_1[2]=%d\r\n", MorseCodeAscii_1[2]);
                        printf("MorseCodeAscii_1[3]=%d\r\n", MorseCodeAscii_1[3]);
                        printf("MorseCodeAscii_1[4]=%d\r\n", MorseCodeAscii_1[4]);
                        
                        printf("MorseCodeAscii_2[0]=%d\r\n", MorseCodeAscii_2[0]);
                        printf("MorseCodeAscii_2[1]=%d\r\n", MorseCodeAscii_2[1]);
                        printf("MorseCodeAscii_2[2]=%d\r\n", MorseCodeAscii_2[2]);
                        printf("MorseCodeAscii_2[3]=%d\r\n", MorseCodeAscii_2[3]);
                        printf("MorseCodeAscii_2[4]=%d\r\n", MorseCodeAscii_2[4]);
                        
                        printf("MorseCodeAscii_3[0]=%d\r\n", MorseCodeAscii_3[0]);
                        printf("MorseCodeAscii_3[1]=%d\r\n", MorseCodeAscii_3[1]);
                        printf("MorseCodeAscii_3[2]=%d\r\n", MorseCodeAscii_3[2]);
                        printf("MorseCodeAscii_3[3]=%d\r\n", MorseCodeAscii_3[3]);
                        printf("MorseCodeAscii_3[4]=%d\r\n", MorseCodeAscii_3[4]);
                        
                        printf("MorseCodeAscii_4[0]=%d\r\n", MorseCodeAscii_4[0]);
                        printf("MorseCodeAscii_4[1]=%d\r\n", MorseCodeAscii_4[1]);
                        printf("MorseCodeAscii_4[2]=%d\r\n", MorseCodeAscii_4[2]);
                        printf("MorseCodeAscii_4[3]=%d\r\n", MorseCodeAscii_4[3]);
                        printf("MorseCodeAscii_4[4]=%d\r\n", MorseCodeAscii_4[4]);
                        
                        printf("MorseCodeAscii_5[0]=%d\r\n", MorseCodeAscii_5[0]);
                        printf("MorseCodeAscii_5[1]=%d\r\n", MorseCodeAscii_5[1]);
                        printf("MorseCodeAscii_5[2]=%d\r\n", MorseCodeAscii_5[2]);
                        printf("MorseCodeAscii_5[3]=%d\r\n", MorseCodeAscii_5[3]);
                        printf("MorseCodeAscii_5[4]=%d\r\n", MorseCodeAscii_5[4]);
                        
                        printf("MorseCodeAscii_5[0]=%d\r\n", MorseCodeAscii_6[0]);
                        printf("MorseCodeAscii_5[1]=%d\r\n", MorseCodeAscii_6[1]);
                        printf("MorseCodeAscii_5[2]=%d\r\n", MorseCodeAscii_6[2]);
                        printf("MorseCodeAscii_5[3]=%d\r\n", MorseCodeAscii_6[3]);
                        printf("MorseCodeAscii_5[4]=%d\r\n", MorseCodeAscii_6[4]);
                        
                        printf("MorseCodeAscii_7[0]=%d\r\n", MorseCodeAscii_7[0]);
                        printf("MorseCodeAscii_7[1]=%d\r\n", MorseCodeAscii_7[1]);
                        printf("MorseCodeAscii_7[2]=%d\r\n", MorseCodeAscii_7[2]);
                        printf("MorseCodeAscii_7[3]=%d\r\n", MorseCodeAscii_7[3]);
                        printf("MorseCodeAscii_7[4]=%d\r\n", MorseCodeAscii_7[4]);
                        
                        printf("MorseCodeAscii_8[0]=%d\r\n", MorseCodeAscii_8[0]);
                        printf("MorseCodeAscii_8[1]=%d\r\n", MorseCodeAscii_8[1]);
                        printf("MorseCodeAscii_8[2]=%d\r\n", MorseCodeAscii_8[2]);
                        printf("MorseCodeAscii_8[3]=%d\r\n", MorseCodeAscii_8[3]);
                        printf("MorseCodeAscii_8[4]=%d\r\n", MorseCodeAscii_8[4]);
                        
                        printf("MorseCodeAscii_9[0]=%d\r\n", MorseCodeAscii_9[0]);
                        printf("MorseCodeAscii_9[1]=%d\r\n", MorseCodeAscii_9[1]);
                        printf("MorseCodeAscii_9[2]=%d\r\n", MorseCodeAscii_9[2]);
                        printf("MorseCodeAscii_9[3]=%d\r\n", MorseCodeAscii_9[3]);
                        printf("MorseCodeAscii_9[4]=%d\r\n", MorseCodeAscii_9[4]);
                        
                        printf("MorseCodeAscii_0[0]=%d\r\n", MorseCodeAscii_0[0]);
                        printf("MorseCodeAscii_0[1]=%d\r\n", MorseCodeAscii_0[1]);
                        printf("MorseCodeAscii_0[2]=%d\r\n", MorseCodeAscii_0[2]);
                        printf("MorseCodeAscii_0[3]=%d\r\n", MorseCodeAscii_0[3]);
                        printf("MorseCodeAscii_0[4]=%d\r\n", MorseCodeAscii_0[4]);
                        */
                        
                        
                        if (MorseodeMatch(MorseCode, MorseCodeAscii_A, MorseCodeLength, MorseCodeAscii_A_len)) {
                            MorseAscii[MorseAsciiLength] = 'A';
                            //printf("MorseCodeLength=%d MorseCode A found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_B, MorseCodeLength, MorseCodeAscii_B_len)) {
                            MorseAscii[MorseAsciiLength] = 'B';
                            //printf("MorseCodeLength=%d MorseCode B found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_C, MorseCodeLength, MorseCodeAscii_C_len)) {
                            MorseAscii[MorseAsciiLength] = 'C';
                            //printf("MorseCodeLength=%d MorseCode C found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_D, MorseCodeLength, MorseCodeAscii_D_len)) {
                            MorseAscii[MorseAsciiLength] = 'D';
                            //printf("MorseCodeLength=%d MorseCode D found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_E, MorseCodeLength, MorseCodeAscii_E_len)) {
                            MorseAscii[MorseAsciiLength] = 'E';
                            //printf("MorseCodeLength=%d MorseCode E found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_F, MorseCodeLength, MorseCodeAscii_F_len)) {
                            MorseAscii[MorseAsciiLength] = 'F';
                            //printf("MorseCodeLength=%d MorseCode F found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_G, MorseCodeLength, MorseCodeAscii_G_len)) {
                            MorseAscii[MorseAsciiLength] = 'G';
                            //printf("MorseCodeLength=%d MorseCode G found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_H, MorseCodeLength, MorseCodeAscii_H_len)) {
                            MorseAscii[MorseAsciiLength] = 'H';
                            //printf("MorseCodeLength=%d MorseCode H found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_I, MorseCodeLength, MorseCodeAscii_I_len)) {
                            MorseAscii[MorseAsciiLength] = 'I';
                            //printf("MorseCodeLength=%d MorseCode I found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_J, MorseCodeLength, MorseCodeAscii_J_len)) {
                            MorseAscii[MorseAsciiLength] = 'J';
                            //printf("MorseCodeLength=%d MorseCode J found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_K, MorseCodeLength, MorseCodeAscii_K_len)) {
                            MorseAscii[MorseAsciiLength] = 'K';
                            //printf("MorseCodeLength=%d MorseCode K found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_L, MorseCodeLength, MorseCodeAscii_L_len)) {
                            MorseAscii[MorseAsciiLength] = 'L';
                            //printf("MorseCodeLength=%d MorseCode L found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_M, MorseCodeLength, MorseCodeAscii_M_len)) {
                            MorseAscii[MorseAsciiLength] = 'M';
                            //printf("MorseCodeLength=%d MorseCode M found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_N, MorseCodeLength, MorseCodeAscii_N_len)) {
                            MorseAscii[MorseAsciiLength] = 'N';
                            //printf("MorseCodeLength=%d MorseCode N found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_O, MorseCodeLength, MorseCodeAscii_O_len)) {
                            MorseAscii[MorseAsciiLength] = 'O';
                            //printf("MorseCodeLength=%d MorseCode O found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_P, MorseCodeLength, MorseCodeAscii_P_len)) {
                            MorseAscii[MorseAsciiLength] = 'P';
                            //printf("MorseCodeLength=%d MorseCode P found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_Q, MorseCodeLength, MorseCodeAscii_Q_len)) {
                            MorseAscii[MorseAsciiLength] = 'Q';
                            //printf("MorseCodeLength=%d MorseCode Q found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_R, MorseCodeLength, MorseCodeAscii_R_len)) {
                            MorseAscii[MorseAsciiLength] = 'R';
                            //printf("MorseCodeLength=%d MorseCode R found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_S, MorseCodeLength, MorseCodeAscii_S_len)) {
                            MorseAscii[MorseAsciiLength] = 'S';
                            //printf("MorseCodeLength=%d MorseCode S found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_T, MorseCodeLength, MorseCodeAscii_T_len)) {
                            MorseAscii[MorseAsciiLength] = 'T';
                            //printf("MorseCodeLength=%d MorseCode T found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_U, MorseCodeLength, MorseCodeAscii_U_len)) {
                            MorseAscii[MorseAsciiLength] = 'U';
                            //printf("MorseCodeLength=%d MorseCode U found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_V, MorseCodeLength, MorseCodeAscii_V_len)) {
                            MorseAscii[MorseAsciiLength] = 'V';
                            //printf("MorseCodeLength=%d MorseCode V found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_W, MorseCodeLength, MorseCodeAscii_W_len)) {
                            MorseAscii[MorseAsciiLength] = 'W';
                            //printf("MorseCodeLength=%d MorseCode W found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_X, MorseCodeLength, MorseCodeAscii_X_len)) {
                            MorseAscii[MorseAsciiLength] = 'X';
                            //printf("MorseCodeLength=%d MorseCode X found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_Y, MorseCodeLength, MorseCodeAscii_Y_len)) {
                            MorseAscii[MorseAsciiLength] = 'Y';
                            //printf("MorseCodeLength=%d MorseCode Y found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_Z, MorseCodeLength, MorseCodeAscii_Z_len)) {
                            MorseAscii[MorseAsciiLength] = 'Z';
                            //printf("MorseCodeLength=%d MorseCode Z found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_1, MorseCodeLength, MorseCodeAscii_1_len)) {
                            MorseAscii[MorseAsciiLength] = '1';
                            //printf("MorseCodeLength=%d MorseCode 1 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_2, MorseCodeLength, MorseCodeAscii_2_len)) {
                            MorseAscii[MorseAsciiLength] = '2';
                            //printf("MorseCodeLength=%d MorseCode 2 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_3, MorseCodeLength, MorseCodeAscii_3_len)) {
                            MorseAscii[MorseAsciiLength] = '3';
                            //printf("MorseCodeLength=%d MorseCode 3 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_4, MorseCodeLength, MorseCodeAscii_4_len)) {
                            MorseAscii[MorseAsciiLength] = '4';
                            //printf("MorseCodeLength=%d MorseCode 4 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_5, MorseCodeLength, MorseCodeAscii_5_len)) {
                            MorseAscii[MorseAsciiLength] = '5';
                            //printf("MorseCodeLength=%d MorseCode 5 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_6, MorseCodeLength, MorseCodeAscii_6_len)) {
                            MorseAscii[MorseAsciiLength] = '6';
                            //printf("MorseCodeLength=%d MorseCode 6 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_7, MorseCodeLength, MorseCodeAscii_7_len)) {
                            MorseAscii[MorseAsciiLength] = '7';
                            //printf("MorseCodeLength=%d MorseCode 7 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_8, MorseCodeLength, MorseCodeAscii_8_len)) {
                            MorseAscii[MorseAsciiLength] = '8';
                            //printf("MorseCodeLength=%d MorseCode 8 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_9, MorseCodeLength, MorseCodeAscii_9_len)) {
                            MorseAscii[MorseAsciiLength] = '9';
                            //printf("MorseCodeLength=%d MorseCode 9 found \r\n", MorseCodeLength);
                        }
                        else if (MorseodeMatch(MorseCode, MorseCodeAscii_0, MorseCodeLength, MorseCodeAscii_0_len)) {
                            MorseAscii[MorseAsciiLength] = '0';
                            //printf("MorseCodeLength=%d MorseCode 0 found \r\n", MorseCodeLength);
                        }
                        else if (MorseCodeLength == 0) {
                            MorseAscii[MorseAsciiLength] = ' ';
                            //printf("MorseCodeLength=%d MorseCode space found \r\n", MorseCodeLength);
                        }
                        else {
                            MorseAscii[MorseAsciiLength] = '-';
                            //printf("MorseCodeLength=%d MorseCode invalid \r\n", MorseCodeLength);
                        }
                        MorseAsciiLength++;
                        //printf("MorseAsciiLength=%d \r\n", MorseAsciiLength);
          
                        for (int morse_cnt=0; morse_cnt<5; morse_cnt++) {
                            MorseCode[morse_cnt] = 0;
                        }
                        MorseCodeLength = 0;
                    }
                    
                }
                if (uart1_char == 22) {
                    LCD_Clear();
                    
                    sprintf(data,"");
                    for (int morse_cnt=0; morse_cnt<16; morse_cnt++) {
                        sprintf(data,"%s%c",data,MorseAscii[morse_cnt]);
                    }
                    printf("data_0_15=%s \r\n", data);
                    LCD_String_xy(1,0,data);
                    
                    sprintf(data,"");
                    for (int morse_cnt=16; morse_cnt<32; morse_cnt++) {
                        sprintf(data,"%s%c",data,MorseAscii[morse_cnt]);
                    }
                    printf("data_16_32=%s \r\n", data);
                    LCD_String_xy(2,0,data);/*Send string data for printing*/    
                }
                if (uart1_char == 23) {
                    LCD_Clear();
                    
                    sprintf(data,"");
                    for (int morse_cnt=0; morse_cnt<16; morse_cnt++) {
                        sprintf(data,"%s%c",data,MorseAscii[morse_cnt]);
                    }
                    printf("data_0_15=%s \r\n", data);
                    LCD_String_xy(1,0,data);
                    
                    sprintf(data,"");
                    for (int morse_cnt=16; morse_cnt<32; morse_cnt++) {
                        sprintf(data,"%s%c",data,MorseAscii[morse_cnt]);
                    }
                    printf("data_16_32=%s \r\n", data);
                    LCD_String_xy(2,0,data);/*Send string data for printing*/
                    
                    for (int morse_cnt=0; morse_cnt<5; morse_cnt++) {
                        MorseCode[morse_cnt] = 0;
                    }
                    for (int morse_cnt=0; morse_cnt<32; morse_cnt++) {
                        MorseAscii[morse_cnt] = '\0';
                    }
                    MorseCodeLength = 0;
                    MorseAsciiLength = 0;
                }
            }
            
            if(uart1_char == 30) {
                LCD_Clear();
                LCD_String_xy(1,0,"Hi!"); 
                LCD_String_xy(2,0,"How you doing?");/*Send string data for printing*/
            }

            //if(uart1_char == 15) {
                //sprintf(data,"%d",uart1_char);
                //LCD_Clear();
                //LCD_String_xy(1,0,"UART1_CHAR:"); 
                //LCD_String_xy(2,0,data);/*Send string data for printing*/
                //melody(5,1000);
                //buzzer(20,100);
           // }

#endif // PIC2
        }

    }
    
}
