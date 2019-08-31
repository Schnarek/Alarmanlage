/*
 *Arthur Werle
 *BA_Projekt
 * Created on 9. Augsust 2015, 

 */


//#define _XTAL_FREQ 16000000
#include <stdio.h>
#include "stdio.h"
#include <stdlib.h>
#include <libpic30.h>
#include <p24F08KL302.h>
#include <xc.h>
//#include <spi.h>
#include "nRF24L01.h"
#include "wl_module.h"

/*
 * 
 */
void PNconfig(void)
 {

    ANSA = 0; //0 = Digital input buffer is active
    ANSB = 0; //0 = Digital input buffer is active
    //Für Interrupt inpiut!
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;               
    TRISBbits.TRISB7 = 1;               //IRQ
    TRISBbits.TRISB8 = 0;
    TRISBbits.TRISB9 = 0;               
    TRISBbits.TRISB10 = 1;              //SDI Input
    TRISBbits.TRISB11 = 0;              //SCK Out
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;              //SDO1  Output
    TRISBbits.TRISB14 = 0;              
    TRISBbits.TRISB15 = 0;              //SS1 als Output CSN

    TRISAbits.TRISA7 = 1;               //Input für sensor
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0; 
    TRISAbits.TRISA2 = 0;               
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA6 = 0;

}

void _delay_10ms(int length){
    while(length){
        length--;       
    }
}

void SPIInit1(void)
{
    SPI1BUF = 0;                        // clear SPI buffer
    SSP1CON1bits.SSPEN=1;
    IEC1bits.SSP1IE= 0;                 // do not disable int !!
                                        // Activate SDA SCL Ports
    SSP1CON1bits.CKP=0;                 // Aktiviert bei Idle von Clk auf Low
    SSP1CON1bits.SSPM=2;                // 0010 = SPI Master mode, Clock = FOSC/32
    
    PADCFG1bits.SCK1DIS=0;              // SPI Mode (SCK1) output to the Pin
    PADCFG1bits.SDO1DIS=0;              // SPI Mode (SDO1) output

    SSP1STATbits.CKE = 1;               //0 = Transmit occurs on transition from Idle to active clock state !!!!
    SSP1STATbits.SMP=1;
    SSP1CON1bits.SSPOV = 0;              //Transmit occurs on transition from Idle to active clock state
}



void spi_transmit_sync(unsigned char * data, unsigned int length)
    {              // Important changed to Int
        unsigned char tmp;
        while(length){
	SSP1BUF = *data;
	while( !IFS1bits.SSP1IF ); // wait until buffer full
        IFS1bits.SSP1IF = 0; // clear SSP1IF
        tmp = SSP1BUF; // read out data
         length--;
         data ++;
        }}

void spi_transfer_sync(unsigned char * dataout, unsigned char * datain, unsigned int length)
{
    while(length)
 {
    SSP1BUF = *dataout;
    while( !IFS1bits.SSP1IF ); // wait for buffer full
    IFS1bits.SSP1IF = 0; // clear SSP1IF
    *datain = SSP1BUF; // read out data
    length--;
    dataout++;
    datain++;
 }
}

unsigned char spi_fast_shift(unsigned char data)
{
    SSP1BUF = data;
    while( !IFS1bits.SSP1IF ); // wait for buffer full
    IFS1bits.SSP1IF = 0; // clear SSP1IF
    return SSP1BUF;
}

void _ISR_PSV_INT2Interrupt()           
{
    __delay32(806451);                  //software debounce -sort of
     PORTBbits.RB15 = 0;                //Manual /SS
     //wl_module_send(50);              //send message
     PORTBbits.RB15 = 1;                //Manual /SS
     IFS1bits.INT2IF=0;                  //Interrupt has occurred clear
}


int main(int argc, char** argv)

{
    
    PORTBbits.RB9=0;
    unsigned char payload[wl_module_PAYLOAD];   //holds the payload
    unsigned char nRF_status;                   //STATUS information of nRF24L01+
    unsigned char zaehler = 0;
    unsigned char Status;
    POSCMD_XT;
    LPRCSEL_HP;
    OSCIOFNC_ON;
    POSCMD_NONE;
    SOSCSEL_SOSCHP;
    
    
    PNconfig();                                 //IO-Configuration
    SPIInit1();                                 //SPI Initialization
    wl_module_init();                           //wireless module Init
    //Call wireless module and init
    _delay_10ms(260);                           //                           
    wl_module_tx_config(wl_module_TX_NR_0);             //Config for transfer
    PORTBbits.RB3=0;

    while(1)
    {   
    strcpy( payload,"Hallo");
    //payload = 9;
   
    _delay_10ms(260);
    
    while (1)
    {
                                      
        if (PORTAbits.RA7 == 1)                         //Polling
        {
           
            wl_module_send(payload, wl_module_PAYLOAD); //Load Payload + Config
             
          _delay_10ms(260);              
        }

    }
   
    return (EXIT_SUCCESS);
    }}

   


void __attribute__((__interrupt__,auto_psv)) _INT0Interrupt(void) //int0 inerrupt 
{
    _delay_10ms(260);
    PORTBbits.RB3=1;
    
    // external interrupt IRQ pin of NRF24L01
    if( IFS0bits.INT0IF){
        unsigned char status;

        // Read wl_module status
        wl_module_CSN_lo; // Pull down chip select
        status = spi_fast_shift(NOP); // Read status register
        wl_module_CSN_hi; // Pull up chip select


        if (status & (1<<TX_DS)){ // IRQ: Package has been sent
                    wl_module_config_register(STATUS, (1<<TX_DS)); //Clear Interrupt Bit
                    PTX=0;
        }

	if (status & (1<<MAX_RT)){ // IRQ: Package has not been sent, send again
		wl_module_config_register(STATUS, (1<<MAX_RT));	// Clear Interrupt Bit
		wl_module_CE_hi; // Start transmission
		_delay_10ms(5);
		wl_module_CE_lo;
	}

	if (status & (1<<TX_FULL)){ //TX_FIFO Full <-- this is not an IRQ
		wl_module_CSN_lo; // Pull down chip select
		spi_fast_shift(FLUSH_TX); // Flush TX-FIFO
		wl_module_CSN_hi; // Pull up chip select
	}
        // reset INT1 flag
        IFS0bits.INT0IF = 0;         //Clear INT0 interrupt flag 
        _delay_10ms(260);
    }
}
