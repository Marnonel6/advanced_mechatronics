#ifndef SPI__H__
#define SPI__H__

void initSPI();
unsigned char spi_io(unsigned char o);

#define CS LATBbits.LATB12

#endif // SPI__H__