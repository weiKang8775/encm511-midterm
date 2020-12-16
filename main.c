/*
 * File:   main.c
 * Author: Francis
 *
 * Created on November 22, 2020, 9:33 PM
 */

// MPLAB header libraries
#include <xc.h>

#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <errno.h>

// User header files
#include "ChangeClk.h"
#include "IOs.h"
#include "Multimeter.h"

uint8_t defaultClk = 32;
uint16_t multimeter_mode = 0;
uint8_t cnFlag = 0;

// CLOCK CONTROL
#pragma config IESO = OFF // 2 Speed Startup disabled
#pragma config FNOSC = FRC // Start up CLK = 8 MHz
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor off
#pragma config SOSCSEL = SOSCLP // Secondary oscillator for Low Power Operation
#pragma config POSCFREQ = MS //Primary Oscillator/External clk freq betwn
#pragma config OSCIOFNC = ON //CLKO output disabled on pin 8, use as IO.
#pragma config POSCMOD = NONE // Primary oscillator mode is disabled

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

//MAIN
void main(void) {
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    AD1PCFGbits.PCFG5 = 0;
    IOinit();

    defaultClk = 8;

    // Change Clock
    NewClk(defaultClk); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz

    //Clock output on REFO/RB15 – PULSE GEN Testing purposes only
    TRISBbits.TRISB15 = 0; // Set RB15 as output for REFO
    REFOCONbits.ROSSLP = 1; // Ref oscillator is disabled in sleep
    REFOCONbits.ROSEL = 0; // Output base clk showing clock switching
    REFOCONbits.RODIV = 0b0011;
    REFOCONbits.ROEN = 1; // Ref oscillator is enabled 

    while(1)
    {
        Multimeter();
    }
    
    return;
}