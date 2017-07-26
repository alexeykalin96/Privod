/*
 * rs485.cpp
 *
 * Created: 30.05.2017 10:41:33
 *  Author: user
 */ 

#define F_CPU 4608000UL
#define ADDRESS1 '1'
#define ADDRESS2 '7'
#define ADDRESS3 '0'
#define ADDRESS4 '6'
#define ADDRESS5 '0'
#define ADDRESS6 '0'
#define ADDRESS7 '0'
#define ADDRESS8 '2'
#define sm_per_round 0.154F
#define ANSWER_TIME 2000
#include "Commands.h"

#include <avr/io.h>
#include "Uart.h"
#include "Uart.cpp"
#include "Privod.h"
#include "Privod.cpp"
#include "PrivodSettings.h"
#include "PrivodSettings.cpp"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

void InitPorts(void)
{
	//Порты управления приводом на вход - подтяжка через внешние резисторы
	DDRB &= ~(1 << 0);
	DDRB &= ~(1 << 1);	
	
	//Полярность на вход
	DDRB &= ~(1 << 2);	
	
	//Тревожная кнопка на вход
	DDRA &= ~(1 << 6);
	
	//Концевик на вход
	DDRB &= ~(1 << 6);
}

int main(void)
{	
	_delay_ms(100);
	
	InitPorts();
	
	Uart uart;
	uart.initUart();
	uart.getchUartEn();
	
	Privod privod;
	privod.Init_Timer0_Crash();
	privod.Init_Timer1_PWM();
	privod.Init_Int1_Holla();
	
	uint16_t polarity = privod.get_polarity();
	uint16_t mode = Modes::NO;
	
	sei();
	
	while(uart.getLoadingMode() == Loading::NORMAL)
	{
		if(polarity == privod.get_polarity())
		{
			if(mode != privod.getAnalysis(polarity))
			{
				mode = privod.getAnalysis(polarity);
				switch(mode)
				{
					case Modes::STOP:
					privod.stop();
					_delay_ms(1000);
					privod.clearCrashedFlag();
					break;
					
					case Modes::OPEN:
					privod.stop();
					_delay_ms(1000);
					privod.open(privod.getMaxSpeed());
					privod.clearCrashedFlag();
					break;
					
					case Modes::CLOSE:
					privod.stop();
					_delay_ms(1000);
					privod.close(privod.getMaxSpeed());
					privod.clearCrashedFlag();
					break;
					
					case Modes::DOVODKA:
					privod.stop();
					_delay_ms(1000);
					privod.close(privod.getMinSpeed());
					privod.clearCrashedFlag();
					break;
					
					case Modes::KONCEVIK:
					privod.stop();
					_delay_ms(2000);
					privod.close(720);
					_delay_ms(3000);
					privod.stop();
					uart.setLoadingMode(Loading::WAITING);
					break;
					
					case Modes::HUMMER:
					privod.stop();
					_delay_ms(2000);
					privod.hummer();
					privod.stop();
					_delay_ms(1000);
					privod.clearCrashedFlag();
					break;
					
					case Modes::BACK:
					privod.stop();
					_delay_ms(2000);
					privod.open(720);
					_delay_ms(3000);
					privod.stop();
					privod.clearCrashedFlag();
					break;
					
					case Modes::WAITING:
					privod.stop();
					_delay_ms(1000);
					uart.setLoadingMode(Loading::WAITING);
					break;
					
					default:
					privod.stop();
					_delay_ms(1000);
					uart.setLoadingMode(Loading::WAITING);
				}
			}
		}
		else
		{
			privod.stop();	
			_delay_ms(1000);
			uart.setLoadingMode(Loading::WAITING);
		}				
	}
	
	privod.stop();
	while(uart.getLoadingMode() == Loading::WAITING)
	{
	
	}	
	
	PrivodSettings privodSettings;
	bool was = false;
	while(uart.getLoadingMode() == Loading::PROGRAMMING)
	{
		//запрос адреса
		if((uart.getchUart() == Commands::ADDRESS) && (was == false))
		{
			privod.stop();
			uart.sendUartEn();
			uart.sendUart(Commands::ADDRESS);
			_delay_ms(300);		
			privod.stop();
			uart.getchUartEn();	
			was = true;	
		}
		
		//сбросить нуль	
		else if((uart.getchUart() == Commands::RESET) && (was == false))
		{
			privodSettings.setCurrentPosition(0);
			uart.sendUartEn();
			uart.sendUart(Commands::ECHO);
			_delay_ms(300);	
			uart.getchUartEn();	
			was = true;
		}	
		
		else if(was == true)
		{
			uart.clearUart();
			was = false;
		}	
	}	
}


ISR(INT1_vect, ISR_BLOCK)
{
	Privod::Int1_Holla();
}

ISR(TIMER0_OVF_vect, ISR_NOBLOCK)
{
	Privod::Timer0_Crash();
}

ISR(LIN_TC_vect, ISR_BLOCK)
{
	Uart::readUart();
}