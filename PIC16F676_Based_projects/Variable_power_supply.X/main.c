/*
 * File:   main.c
 * Author: yuva
 *
 * Created on 20 February, 2021, 11:23 AM
 */

// PIC16F676 Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA3/MCLR pin function select (RA3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000
#define DATA_A PORTAbits.RA2
#define DATA_B PORTCbits.RC0
#define DATA_C PORTCbits.RC1
#define DATA_D PORTCbits.RC2
#define DATA_E PORTCbits.RC3
#define DATA_F PORTCbits.RC4
#define DATA_G PORTCbits.RC5
#define DATA_DP PORTAbits.RA3
#define digit_2 PORTAbits.RA4
#define digit_1 PORTAbits.RA5
void look_up (unsigned char);
void convertion(unsigned int);
void PORT_config();
void Interrupt_config();
unsigned int ADC(unsigned char);
void ADC_init(void);

unsigned char arr[2]={0,0};
unsigned char count=0;

void look_up(unsigned char data)
{
    switch(data)
    {
        case 0:
            DATA_A=0;//Display=0
            DATA_B=0;
            DATA_C=0;
            DATA_D=0;
            DATA_E=0;
            DATA_F=0;
            DATA_G=1;
            DATA_DP=0;
            break;
        case 1:
            DATA_A=1;//Display=1
            DATA_B=0;
            DATA_C=0;
            DATA_D=1;
            DATA_E=1;
            DATA_F=1;
            DATA_G=1;
            DATA_DP=0;
            break;
        case 2:
            DATA_A=0;//Display=2
            DATA_B=0;
            DATA_C=1;
            DATA_D=0;
            DATA_E=0;
            DATA_F=1;
            DATA_G=0;
            DATA_DP=0;
            break;
        case 3:
            DATA_A=0;//Display=3
            DATA_B=0;
            DATA_C=0;
            DATA_D=0;
            DATA_E=1;
            DATA_F=1;
            DATA_G=0;
            DATA_DP=0;
            break;
        case 4:
            DATA_A=1;//Display=4
            DATA_B=0;
            DATA_C=0;
            DATA_D=1;
            DATA_E=1;
            DATA_F=0;
            DATA_G=0;
            DATA_DP=0;
            break;
        case 5:
            DATA_A=0;//Display=5
            DATA_B=1;
            DATA_C=0;
            DATA_D=0;
            DATA_E=1;
            DATA_F=0;
            DATA_G=0;
            DATA_DP=0;
            break;
        case 6:
            DATA_A=0;//Display=6
            DATA_B=1;
            DATA_C=0;
            DATA_D=0;
            DATA_E=0;
            DATA_F=0;
            DATA_G=0;
            DATA_DP=0;
            break;
        case 7:
            DATA_A=0;//Display=7
            DATA_B=0;
            DATA_C=0;
            DATA_D=1;
            DATA_E=1;
            DATA_F=1;
            DATA_G=1;
            DATA_DP=0;
            break;
        case 8:
            DATA_A=0;//Display=8
            DATA_B=0;
            DATA_C=0;
            DATA_D=0;
            DATA_E=0;
            DATA_F=0;
            DATA_G=0;
            DATA_DP=0;
            break;
        case 9:
            DATA_A=0;//Display=9
            DATA_B=0;
            DATA_C=0;
            DATA_D=0;
            DATA_E=1;
            DATA_F=0;
            DATA_G=0;
            DATA_DP=0;
            break;
        default:
            DATA_A=0;//Display=E
            DATA_B=1;
            DATA_C=1;
            DATA_D=0;
            DATA_E=0;
            DATA_F=0;
            DATA_G=0;
            DATA_DP=0;
            break;
         
    }
}
void convertion(unsigned int adc_value)
{
 arr[0]=adc_value/ 1000;
 arr[1]=(adc_value/100)%10;
}
void PORT_config(){
   TRISA=0x01;
   TRISC=0x00;
   PORTA=0xFF;
   PORTC=0xFF;
   ANSEL=0x01;
   CMCON=0x07;
}
void Interrupt_config(){
   OPTION_REG=0x86;// 128 prescalar with 10ms delay
   TMR0=175;
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;
    INTCONbits.T0IE=1;
}
void ISR_call(){
    if(count==0)
    {
    digit_2=1;
    look_up(arr[0]);
    digit_1=0;
    }
    if(count==1)
    {
    digit_1=1;
    look_up(arr[1]); 
    digit_2=0;
    }
    if(++count>=2)
    {
        count=0;
    }
}
unsigned int ADC(unsigned char CH)
{
   ADCON0 &=0b11000101;     //Clear only the channel bit then AND the bits with 1 to  get the desird value 
   ADCON0 |=CH<<3;
   __delay_ms(2);
   ADCON0bits.GO_nDONE=1;   
   while(ADCON0bits.GO_nDONE);
   return ((ADRESH<<8)+ADRESL);
}  
void ADC_init()
{
    ADCON0=0x81;                 //10000001
    ADCON1=0x00;                  //00000000
    
}
void __interrupt() timer(void)
{
 INTCONbits.GIE=0;
 if(TMR0IF==1){
     INTCONbits.T0IE=0;
        TMR0IE=0;
        ISR_call();
        TMR0IF=0;
        TMR0IE=1;
        T0IF=0;
    }
   INTCONbits.GIE=1;
    INTCONbits.T0IE=1;
    TMR0=176;
}
void main(void){
    unsigned int value;
    unsigned int adc;
    PORT_config();
    Interrupt_config();
    while(1)
        
    {
    ADC_init();
    value=ADC(0);
    adc=((value*5)/9)*16;
    convertion(adc);
    }
    return;
}
