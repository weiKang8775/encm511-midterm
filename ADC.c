 /*
 * File:   ADC.c
 * Author: Francis
 *
 * Created on November 23, 2020, 6:41 PM
 */


#include "xc.h"
#include "ADC.h"
#include "ChangeClk.h"
#include "UART2.h"
#include "IOs.h"
#include "math.h"

void adcInit(uint8_t mode) {
    // AD1CON1 Config
    AD1CON1bits.ADON = 0;       //Turn off ADC, turn on before sampling
	AD1CON1bits.ADSIDL = 0;     //Continue ADC operation in idle mode
	AD1CON1bits.FORM = 0b00;    //0b00: Int, 01: Signed Int, 10:Fractional, 11:Signed Fract
	AD1CON1bits.SSRC = 0b111;   //Start ADC conversion Time set by SAMC has passed
	AD1CON1bits.ASAM = 0; 	    //Next Sampling begins when SAMP bit is set
	AD1CON1bits.SAMP = 0;		//Set this bit in ADC routine to start sampling

    // AD1CON2 Config
	AD1CON2bits.VCFG=0b000;     //Vref+=AVDD   Vref-=AVSS
	AD1CON2bits.CSCNA=0;	    //Do not scan inputs; use channel selected by CH0SA bits
	AD1CON2bits.SMPI=0b0000;    //Any ADC interrrups occur at completion of each sample/convert seq.
	AD1CON2bits.BUFM=0;         //Buffer confugured as 1 16 word buffer 
	AD1CON2bits.ALTS=0;         //Always use MUXA input MUX settings

    // AD1CON3 Config
    AD1CON3bits.ADRC=0;         //Use System clk
	AD1CON3bits.SAMC=0b11111;   //Sample time = 31*TAD = 31*1/125e3 = 0.248ms
    AD1CON3bits.ADCS=0b111111;  //ADCS conversion clock selet bits. Ignored if using internal AD RC clock.

    // Mux Config
    AD1CHSbits.CH0NA=0;	        //Ch0 -ve Input is VR-

    // Modes Config
    if (mode == 1)                  // Voltage Measurement
    {
        AD1CHSbits.CH0SA = 0b0101;  //Positive input is AN5/RA3/pin8
        TRISAbits.TRISA3 = 1;       //Set pin8/RA3/AN5 as input
        AD1PCFG = 0xFFFF;           //Set all bits as digital
        AD1PCFGbits.PCFG5=0; 	    //Set only pin8/AN5/RA3 as Analog input for ADC
    }
    else if (mode == 2)             // Resistance Measurement
    {
        AD1CHSbits.CH0SA = 0b1011;  //Positive input is pin16/RB13/AN11
        TRISBbits.TRISB13 = 1;      //Set pin16/RB13/AN11 as input
        AD1PCFG = 0xFFFF;           //Set all bits as digital
        AD1PCFGbits.PCFG11 = 0;     //Set only pin16/RB13/AN11 as Analog input for ADC
    }
    else if (mode == 3)             // Amplitude Measurement
    {
        AD1CHSbits.CH0SA = 0b1100;  //Positive input is pin15/AN12/RB12
        TRISBbits.TRISB12 = 1;      //Set pin15/AN12/RB12 as input
        AD1PCFG = 0xFFFF;           //Set all bits as digital
        AD1PCFGbits.PCFG12 = 0;     //Set only pin15/AN12/RB12 as Analog input for ADC
    }

    AD1CSSL = 0;                //Input Scan disabled, 0x0000 is default state.
}

uint16_t getVoltage(uint8_t mode) {

    adcInit(mode);                  //Initialize ADC

    uint16_t adcValue;

    AD1CON1bits.ADON=1;             //Turn on ADC, ADC value stored in ADC1BUF0;
    AD1CON1bits.SAMP=1;             //Start Sampling, Conversion starts automatically after SSRC and SAMC settings
    while(AD1CON1bits.DONE==0) {}
    adcValue= ADC1BUF0;             // ADC output is stored in ADC1BUF0 as this point
    AD1CON1bits.SAMP=0;             //Stop sampling
    AD1CON1bits.ADON=0;             //Turn off ADC, ADC value stored in ADC1BUF0;
    return (adcValue);              //returns 10 bit ADC output stored in ADC1BIF0 to calling function
}


uint16_t Do_ADC(uint16_t CH0SA_bits){   
    uint16_t ADCvalue; // 16 bit register used to hold ADC converted digital output ADC1BUF0
    /* -------------ADC INITIALIZATION ------------------*/
    AD1CON1bits.ADON = 1; //turn on ADC module
    AD1CON1bits.FORM = 0b00; //saves output of ADC as an int
        
    AD1CON1bits.SSRC = 0b111; //automatic conversion
    AD1CON1bits.ASAM = 0; //starts sampling when samp = 1
    
    AD1CON2bits.VCFG = 0b000; //selects AVDD, AVSS (supply voltage to PIC) as Vref
    AD1CON2bits.CSCNA = 0; //one input for ADC

    AD1CON2bits.BUFM = 0;
    AD1CON2bits.ALTS = 0; //uses MUX A

    AD1CON3bits.ADRC = 0; //use microcontroller's system clock
    AD1CON3bits.SAMC = 0b11111;
    
    AD1CHSbits.CH0NA = 0;
    
    //Positive input select
    AD1CHSbits.CH0SA = CH0SA_bits;        
    
    AD1PCFGbits.PCFG5 = 0;
    AD1CSSLbits.CSSL5 = 0;    
   
    /* -------------ADC SAMPLING AND CONVERSION ------------------*/
    AD1CON1bits.SAMP=1; //Start Sampling, Conversion starts automatically after SSRC and SAMC settings
    while(AD1CON1bits.DONE==0) {}
    ADCvalue= ADC1BUF0; // ADC output is stored in ADC1BUF0 as this point
    AD1CON1bits.SAMP=0; //Stop sampling
    AD1CON1bits.ADON=0; //Turn off ADC, ADC value stored in ADC1BUF0;
    return (ADCvalue); //returns 10 bit ADC output stored in ADC1BIF0 to calling function
}