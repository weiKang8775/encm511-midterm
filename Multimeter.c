#include "math.h"

#include "xc.h"
#include "ADC.h"
#include "IOs.h"
#include "UART2.h"
#include "ChangeClk.h"

uint16_t multimeter_mode = -1;
float vRef = 3.2;

void DisplayVoltage(void)
{
    uint16_t adc_output = getVoltage(1);            //do ADC
    float inv_Vres = 1023/vRef; 
    float voltage_reading = adc_output/inv_Vres;    //converts adc output to voltage
    
    //Displays voltage reading
    NewClk(8);
    XmitUART2('\r', 1);
    Disp2String("VOLTMETER Voltage = ");
    Disp2Dec(voltage_reading);
    XmitUART2('V', 1);
    XmitUART2(' ', 5);
    NewClk(defaultClk);

    return;
}

void DisplayResistance(void)
{
    uint16_t adc_output = getVOltage(2);            // do ADC
    float inv_Vres = 1023/vRef;
    float voltage_reading = adc_output/inv_Vres;    //converts adc output to voltage
    float resistance_reading = voltage_reading / (vRef - voltage_reading) *  1000; //voltage divider formula
    
    //Displays resistance reading
    NewClk(8);
    XmitUART2('\r', 1);
    Disp2String("OHMMETER Resistance = ");
    Disp2Dec(resistance_reading);
    XmitUART2(234, 1);
    XmitUART2(' ', 5);
    NewClk(defaultClk);

    return;
}

void DisplayPulse(void) {

    /*---------- Get Amplitude ----------*/

    uint16_t amplitude = 0;
    uint8_t counter = 0;

    IEC1bits.CNIE = 0; //Disable CN interrupt

    while (counter++ < 100) // Samples voltage 100 times
    {
        uint16_t adc_output = getVoltage(3);    
        amplitude = adc_output > amplitude ? adc_output : amplitude; // Find max value
    }
    
    float Vres = vRef / 1023;
    float voltage_reading = amplitude * Vres;

    /*---------- Get Frequency ----------*/

    uint16_t ms_elapsed;
    float freq_kHz;

    IEC1bits.CNIE = 1; //Enable CN interrupt

    cnFlag = 1;
    while (cnFlag == 1) {}

    cnFlag = 1;
    IEC0bits.T2IE = 0; //Disable timer interrupt
    TMR2 = 0;
    T2CONbits.TON = 1; //Start timer
    while (cnFlag == 1) {}
    T2CONbits.TON = 0; //Stop timer

    if (defaultClk == 500) // 500 kHz
    {
        ms_elapsed = 2 / 8000 * TMR2 * 4
    }
    else if (defaultClk == 8) // 8 MHz
    {
        ms_elapsed = 2 / 500 * TMR2 / 4;
    }
    else if (defaultClk == 32) // 32 kHz
    {
        ms_elapsed = TMR2 >> 4;
    }

    freq_kHz = 1 / (ms_elapsed);
    
    NewClk(8);
    XmitUART2('\r', 1);
    Disp2String("PULSEMETER Freq = ");
    Disp2Dec(freq_kHz);
    Disp2String("kHz, Amplitude = ");
    Disp2Dec(voltage_reading);
    XmitUART2('V', 1);
    XmitUART2(' ', 5);
    NewClk(defaultClk);

    return;
}

void Multimeter(){
    if(multimeter_mode == 1)        //voltage mode
    { 
        DisplayVoltage();
    }
    else if(multimeter_mode == 2)   //resistance mode
    {
        DisplayResistance();       
    }
    else if (multimeter_mode == 3)  //pulse mode
    {
        DisplayPulse();
    }
    else                            //idle mode
    {
        //Clears terminal
        NewClk(8);
        XmitUART2('\r', 1);
        XmitUART2(' ', 70);
        NewClk(defaultClk);
        
        Idle();
    }
}
