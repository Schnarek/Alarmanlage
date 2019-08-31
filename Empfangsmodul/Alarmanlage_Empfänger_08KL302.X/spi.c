
#include <xc.h>
#include "spi.h"

void spi_init(void){
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

void spi_transmit_sync(unsigned char * data, unsigned int length){
    unsigned char tmp;
    while(length != 0){
	SSP1BUF = *data;
	while( !IFS1bits.SSP1IF ); // wait for buffer full
        IFS1bits.SSP1IF = 0; // clear SSP1IF
        tmp = SSP1BUF; // read out data
        length--;
	data++;
    }
}


void spi_transfer_sync(unsigned char * dataout, unsigned char * datain, unsigned int length){
    while(length != 0){
	SSP1BUF = *dataout;
	while( !IFS1bits.SSP1IF ); // wait for buffer full
        IFS1bits.SSP1IF = 0; // clear SSP1IF
        *datain = SSP1BUF; // read out data
        length--;
	dataout++;
        datain++;
    }
}

unsigned char spi_fast_shift(unsigned char data){
    SSP1BUF = data;
    while( !IFS1bits.SSP1IF );  // wait for buffer full
    IFS1bits.SSP1IF = 0;        // clear SSP1IF
    return SSP1BUF;
}