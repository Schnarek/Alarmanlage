/*
 *Arthur Werle
 *BA_Projekt
 * Created on 9. Augsust 2015, 

 */
#define _XTAL_FREQ 16000000UL
#include <xc.h>
#include <stdlib.h>
#include "spi.h"
#include "nRF24L01.h"
#include "wl_module.h"
#include <p24F08KL302.h>
#include "stdio.h"
#include <libpic30.h>


// PIC24F08KL302 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// FBS
#pragma config BWRP = OFF               // Boot Segment Write Protect (Disabled)
#pragma config BSS = OFF                // Boot segment Protect (No boot flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Flash Write Protect (General segment may be written)
#pragma config GSS0 = OFF               // General Segment Code Protect (No Protection)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with Postscaler and PLL (FRCDIV+PLL))
#pragma config SOSCSRC = ANA            // SOSC Source Type (Analog Mode for use with crystal)
#pragma config LPRCSEL = HP             // LPRC Power and Accuracy (High Power/High Accuracy)
#pragma config IESO = OFF               // Internal External Switch Over bit (Internal External Switchover mode disabled (Two-speed Start-up disabled))

// FOSC
#pragma config POSCMD = NONE              // Primary Oscillator Mode (HS oscillator mode selected)
#pragma config OSCIOFNC = OFF            // CLKO Pin I/O Function (CLKO output signal enabled)
#pragma config POSCFREQ = MS            // Primary Oscillator Frequency Range (Primary Oscillator/External Clock frequency between 100kHz to 8MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary Oscillator configured for high-power operation)
#pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock Switching and Fail-safe Clock Monitor Enabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32768)
#pragma config FWPSA = PR128            // WDT Prescaler bit (WDT prescaler ratio of 1:128)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bits (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected (windowed WDT disabled))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON              // Power-up Timer Enable (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default SCL1/SDA1 Pins for I2C1)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset at 1.8V)
#pragma config MCLRE = ON               // MCLR Pin Enable bit (RA5 input disabled; MCLR enabled)

// FICD
#pragma config ICS = PGx1               // ICD Pin Placement Select (EMUC/EMUD share PGC1/PGD1)



#define MAX_STRLEN 14
char received_string[MAX_STRLEN+1];

void _delay_10ms(int length){
    while(length){
        length--;
       //_delay_10ms(5);
        
    }
}
void Pinconfig(){
}



int main(int argc, char** argv){
    TRISBbits.TRISB0 = 0;               // All 0 as output for secure
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;               
    TRISBbits.TRISB7 = 0;               //IRQ
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0; 
    TRISBbits.TRISB10 = 1;              //SDI Input
    TRISBbits.TRISB11 = 0;              //SCK Out
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;              //SDO1  Output
    TRISBbits.TRISB14 = 1;              //Interrupt INT1
    TRISBbits.TRISB15 = 0;              //SS1 as Output CSN
    TRISAbits.TRISA7 = 1;               //Input für sensor
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0; 
    TRISAbits.TRISA2 = 0;               
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA6 = 0;
    //Reset Port
    PORTBbits.RB8=1;        
    PORTAbits.RA2=1;
    unsigned char payload[wl_module_PAYLOAD]; //holds the payload
    unsigned char nRF_status; //STATUS information of nRF24L01+
    unsigned char zaehler = 0;
    // configure output port
    IEC1bits.INT1IE = 1; // peripheral interrupts enabled
    IPC5bits.INT1IP = 7; //INTERRUPT PRIORITY CONTROL REGISTER 5

     wl_module_init(); //Init nRF Module
    _delay_10ms(260); //wait for Module

    wl_module_config();	//config nRF as RX Module, simple Version

    while(1){
        while (!wl_module_data_ready()); //waits for RX_DR Flag in STATUS
        nRF_status = wl_module_get_data(payload); //reads the incomming Data to Array payload
	    zaehler = payload[0];
        if(payload == "Hallo"){         
            PORTBbits.RB8=1;                        // Output for raspberry
            _delay_10ms(260);                       
            PORTBbits.RB8=0;                        // toggle back
        }      
        _delay_10ms(260);                          //Delay for secure work
        }
    return 0;
}

//void interrupt_ISR(void){
//    // external interrupt IRQ pin of NRF24L01
//    if( IFS1bits.INT1IF){
//        unsigned char status;
//
//        // Read wl_module status
//        wl_module_CSN_lo; // Pull down chip select
//        status = spi_fast_shift(NOOP); // Read status register
//        wl_module_CSN_hi; // Pull up chip select
//
//
//        if (status & (1<<TX_DS)){ // IRQ: Package has been sent
//                    wl_module_config_register(STATUS, (1<<TX_DS)); //Clear Interrupt Bit
//                    PTX=0;
//        }
//
//	if (status & (1<<MAX_RT)){ // IRQ: Package has not been sent, send again
//		wl_module_config_register(STATUS, (1<<MAX_RT));	// Clear Interrupt Bit
//		wl_module_CE_hi; // Start transmission
//		_delay_10ms(5);
//		wl_module_CE_lo;
//	}
//
//	if (status & (1<<TX_FULL)){ //TX_FIFO Full <-- this is not an IRQ
//		wl_module_CSN_lo; // Pull down chip select
//		spi_fast_shift(FLUSH_TX); // Flush TX-FIFO
//		wl_module_CSN_hi; // Pull up chip select
//	}
//        // reset INT1 flag
//       IFS1bits.INT1IF = 0;
//    }
//}

