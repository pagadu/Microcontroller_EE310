//---------------------
// Title: CalculatorProject
//---------------------
// Program Details:
// The purpose of this project is make a calculator with inputs coming from a
// keypad. The output/results of the calculator will show up on LEDs in binary.
// We have operands like Addition, Subtraction, Multiplication, and Division
// and we can use digits from 1 to 9 as our input values.

// Inputs: RB4, RB5, RB6, RB7
// Outputs: RB0, RB1, RB2, RB3, PORTD
// Date: April 9, 2024
// File Dependencies / Libraries: CalculatorConfig.inc 
// Compiler: xc8, 2.46
// Author: Alexander Pagaduan
// Versions:
//       V1.0: Initial Draft
//       V1.1: Program with comments
// Useful links: 
//       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
//       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
//       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 


//---------------------
// INITIALIZATION
//---------------------
#include<stdio.h>
#include<stdlib.h>
#include "CalculatorConfig.h"  
#include <xc.h>

// Define macros for GPIO ports
#define KEYPAD_PORT PORTB
#define KEYPAD_TRIS TRISB
#define LED_PORT PORTD
#define LED_TRIS TRISD

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

// Function prototypes
int readKeypad();
void displayResult(int result);
void configure_ports();
void delay(int num1, int num2);

    
// Global variables
volatile uint8_t X_Input_REG = 0; //first input
volatile uint8_t Y_Input_REG = 0; //second input
volatile char Operation_REG = ' '; // operand var
volatile int Display_Result_REG = 0; // Result value to be placed on LEDs
volatile int button_pressed = 0; // Used to wait for button to be depressed

int reset = 0; // reset flag if "*" is pressed
int key1 = 0; // stores the first input
int op = 0; // stores operand
int key2 = 0; // stores the second input
int result = 0; // result flag if "#" is pressed
        
//Placeholder if we want to add delays
//Didn't need to use it
void delay(int num1, int num2) {
     int cnt1, cnt2;
//   for (cnt1=0; cnt1<num1; cnt1++)
//       for (cnt2=0; cnt2<num2; cnt2++)
//       {}
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

void displayResult(int result) {
    // Convert result to binary and display on LEDs
    LED_PORT = (unsigned char) result;
    delay(1,10);
    
}

/////////////////////////////////////////////
//CONFIGURE PORTS
void configure_ports() {
    // Initialize ports for keypad and LEDs
    PORTD = 0;
    LATD = 0;
    ANSELD = 0;
    KEYPAD_TRIS = 0b00001111; // Set first 4 pins as input
    
    PORTB = 0;
    LATB = 0;
    ANSELB = 0;
    LED_TRIS = 0x00; // Set all pins as output
}


void main() {
    //printf ("Start main\n");
    configure_ports();
    
    while (1) {
        //reset all flags
        reset = 0;
        key1 = 0;
        op = 0;
        key2 = 0;
        result = 0;
        X_Input_REG = 0;
        Y_Input_REG = 0;
        
        /////////////////////////////////////////////
        //KEY1
        //while loop waiting for valid key1 or reset
        while (((key1<1) || (key1>9) || (key1==16)) && (reset != 1)){
            key1 = readKeypad();
            if (key1 == 14) {
                reset = 1;
            }
        }
        //if reset clear LEDs and set and reset flag
        if (reset == 1) {
            displayResult(0);
        //if key1 is valid set LED bit 0
        } else if ((key1>0) && (key1<10)) {
            displayResult(1);
        }
        
        /////////////////////////////////////////////
        //OPERAND
        //while loop waiting for valid operand or reset
        while (((op<10) || (op>13) || (op==16)) && (reset != 1)){
            op = readKeypad();
            if (op == 14) {
                reset = 1;
            }
        }
        //if reset clear LEDs and set and reset flag
        if (reset == 1) {
            displayResult(0);
         //if operand is valid set LED bit 0
        } else if ((op>9) && (op<14)) {
            displayResult(1);
        }
        
        /////////////////////////////////////////////
        //KEY2
        //while loop waiting for valid key2 or reset
        while (((key2<1) || (key2>9) || (key2==16)) && (reset != 1)){
            key2 = readKeypad();
            if (key2 == 14) {
                reset = 1;
            }
        }
        //if reset clear LEDs and set and reset flag
        if (reset == 1) {
            displayResult(0);
        //if key2 is valid set LED bit 1
        } else if ((key2>0) && (key2<10)) {
            displayResult(2);
        }
        
        
        /////////////////////////////////////////////
        //RESULT
        //while loop waiting for valid result or reset
        while (((result!=15)) && (reset != 1)){
            result = readKeypad();
            //If result = 14 then reset, otherwise print result
            if (result == 14) {
                reset = 1;
            }
        }
        
        //If key1, operand, key2, result are all valid
        //and no reset, then process the calculation
        if (((key1>0) && (key1<10)) && // valid key1
           ((op>9) && (op<14)) &&    //valid operand
           ((key2>0) && (key2<10)) && //valid key2
           ((reset == 0)) && //no reset
           ((result == 15))) { //valid result
           X_Input_REG = (uint8_t) key1;
           Y_Input_REG = (uint8_t) key2;
           
           // convert operand to Operation_REG
           if (op == 10) {
               Operation_REG = 'A';
           }
           if (op == 11) {
               Operation_REG = 'B';
           }
           if (op == 12) {
               Operation_REG = 'C';
           }
           if (op == 13) {
               Operation_REG = 'D';
           }
           
            // Perform calculation and display
            switch(Operation_REG) {
                case 'A':
                    Display_Result_REG = X_Input_REG + Y_Input_REG;
                    break;
                case 'B':
                    Display_Result_REG = X_Input_REG - Y_Input_REG;
                    break;
                case 'C':
                    Display_Result_REG = X_Input_REG * Y_Input_REG;
                    break;
                case 'D':
                    if (Y_Input_REG != 0) {
                        Display_Result_REG = X_Input_REG / Y_Input_REG;
                    } else {
                        // Handle division by zero error
                        Display_Result_REG = 0;
                    }
                    break;
                default:
                    // Invalid operation
                    Display_Result_REG = 0;
            }
            // Display result
            displayResult(Display_Result_REG);

        } else {
            //Clear LEDs if reset
            displayResult(0);
        }
    }
}

