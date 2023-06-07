/* 
 * File:   uart.h
 * Author: marno
 *
 * Created on June 6, 2023, 1:11 PM
 */

#ifndef UART2__H__
#define UART2__H__

#include <xc.h> // processor SFR definitions
#include <sys/attribs.h> // __ISR macro

#include "nu32dip.h"

void UART2_Startup();
void WriteUART2(const char * string);
int get_uart2_flag();
void set_uart2_flag();
int get_uart2_value();

#endif // UART2__H__