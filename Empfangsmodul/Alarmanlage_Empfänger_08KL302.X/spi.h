/* 
 * File:   spi.h
 * Author: Arthur Werle
 *
 * Created on 20. Juni 2015, 13:21
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define	_SPI_H_

extern void spi_init(void);
extern void spi_transfer_sync(unsigned char* dataout, unsigned char* datain, unsigned int length);
extern void spi_transmit_sync(unsigned char* data, unsigned int length);
extern unsigned char spi_fast_shift(unsigned char data);



#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

