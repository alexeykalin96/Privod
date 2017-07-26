#ifndef F_CPU
#define F_CPU 4608000UL
#endif

#ifndef SAMPLES_PER_BIT
#define SAMPLES_PER_BIT 8
#endif

#ifndef BAUD_RATE
#define BAUD_RATE 2400
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Uart.h"

//==========================private_static_fields_of_class_Uart====================================================
volatile unsigned int Uart::rxCounter;
volatile unsigned char Uart::rxBuff[15];
volatile unsigned char Uart::txBuff[15];
volatile uint16_t Uart::loadingMode = 0;
//=================================================================================================================

//==========================public_methods_of_class_Uart=========================================================
void Uart::initUart(void)
{
	LINBTR = (1 << LDISR)|SAMPLES_PER_BIT; // set 600 bod
	LINBRR = ((F_CPU / SAMPLES_PER_BIT) / BAUD_RATE) - 1; // set 600 bod
}

void Uart::sendUartEn(void) {
	LINCR = (1 << LCMD2)|(0 << LCMD1)|(1 << LCMD0); // enable transmit mode
	LINENIR = (0 << LENRXOK); // receive interrupt is disable
	LINCR |= (1 << LENA); // UART is enable, Tx and Rx are enabled too
	DDRB |= (1 << 3); // set PD3 as output
	PORTB |= (1 << 3); // set PD3
}

void Uart::getchUartEn(void) {
	LINCR = (1 << LCMD2)|(1 << LCMD1)|(0 << LCMD0); // enable receive mode
	LINENIR = (1 << LENRXOK); // receive interrupt is enable
	LINCR |= (1 << LENA); // UART is enable, Tx and Rx are enabled too
	DDRB |= (1 << 3); // set PD3 as output
	PORTB &= ~(1 << 3); // clear PD3
}

void Uart::sendUart(unsigned char send)
{
	switch(send)
	{
		case Commands::ECHO:
			for(int i = 0; i < 14; i++)
			{
				LINDAT = rxBuff[i];
				while(!(LINSIR & (1 << LTXOK)));
			}		
		break;	
		
		case Commands::ADDRESS:
			txBuff[0] = '&';
			txBuff[1] = ADDRESS1;
			txBuff[2] = ADDRESS2;	
			txBuff[3] = ADDRESS3;
			txBuff[4] = ADDRESS4;
			txBuff[5] = ADDRESS5;
			txBuff[6] = ADDRESS6;	
			txBuff[7] = ADDRESS7;
			txBuff[8] = ADDRESS8;
			txBuff[9] = Commands::ADDRESS;
			txBuff[10] = Commands::NULL;
			txBuff[11] = Commands::NULL;
			txBuff[12] = Commands::NULL;
			txBuff[13] = '$';
							
			for(int i = 0; i < 14; i++)
			{
				LINDAT = txBuff[i];
				while(!(LINSIR & (1 << LTXOK)));
			}	
			break;
	}
}

void Uart::readUart(void)
{
	if(LINDAT == '&')
	{
		rxCounter = 0;	
	}
		rxBuff[rxCounter] = LINDAT;
		rxCounter++;
		if(rxCounter == 1)
		{
			for(int i = 1; i < 14; i++)
			{
				rxBuff[i] = 0;
			}
		}
		else if(rxCounter == 14)
		{
			rxCounter = 0;
		}
		if((rxBuff[0] == '&') && (rxBuff[1] == '0') && (rxBuff[2] == '0') && (rxBuff[3] == '0') && (rxBuff[4] == '0') &&
		  (rxBuff[5] == '0') && (rxBuff[7] == '0') && (rxBuff[8] == '0') && (rxBuff[9] == 'A') && (rxBuff[10] == '0') &&
		  (rxBuff[11] == '0') && (rxBuff[12] == '0') && (rxBuff[13] == '$'))
		{
			loadingMode = Loading::PROGRAMMING;
		}
}

unsigned char Uart::getchUart(void)
{
	if((rxBuff[0] == '&') && (rxBuff[13] == '$'))
	{
//==========обработка адресных команд===========================		
		if((rxBuff[1] == ADDRESS1) && (rxBuff[2] == ADDRESS2) &&
		   (rxBuff[3] == ADDRESS3) && (rxBuff[4] == ADDRESS4) &&
		   (rxBuff[5] == ADDRESS5) && (rxBuff[6] == ADDRESS6) &&
		   (rxBuff[7] == ADDRESS7) && (rxBuff[8] == ADDRESS8))
		   {
				switch(rxBuff[9])
				{
					case Commands::RESET:
					return Commands::RESET;
					break;
				}	
			}   
//=======================================================================		

//==========обработка широковещательных команд===========================	
	else if((rxBuff[1] == '0') && (rxBuff[2] == '0') &&
			(rxBuff[3] == '0') && (rxBuff[4] == '0') &&
			(rxBuff[5] == '0') && (rxBuff[6] == '0') &&
			(rxBuff[7] == '0') && (rxBuff[8] == '0'))
			{
				switch(rxBuff[9])
				{
					//запрос адреса
					case Commands::ADDRESS:
					return Commands::ADDRESS;
					break;
				}
			}
//=======================================================================			
	}
}

void Uart::clearUart(void)
{
	for(int i = 0; i < 14; i++)
	{
		rxBuff[i] = 0;
	}
}

uint8_t Uart::getLoadingMode(void)
{
	return loadingMode;
}

void Uart::setLoadingMode(uint16_t mode)
{
	loadingMode = mode;
}
//=================================================================================================================
