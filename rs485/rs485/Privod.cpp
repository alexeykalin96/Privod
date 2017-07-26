/*
 * privod.cpp
 *
 * Created: 30.05.2017 11:54:57
 *  Author: user
 */ 

#include "Privod.h"
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>


volatile uint16_t Privod::mode;
volatile uint16_t Privod::currentPosition;
volatile uint16_t Privod::maxPosition;
volatile uint16_t Privod::dovodkaPosition;
volatile uint16_t Privod::hummerPosition;
volatile uint16_t Privod::maxSpeed;
volatile uint16_t Privod::minSpeed;
volatile uint16_t Privod::numberOfCrashes;
volatile int16_t Privod::counterDir;
volatile uint16_t Privod::crashedCounter;
volatile uint16_t Privod::timer0;
volatile uint16_t Privod::crashedTime;
volatile uint16_t Privod::filterTime;
volatile bool Privod::closedFlag;
volatile bool Privod::crashedFlag;
volatile bool Privod::koncevikFlag;
		
volatile uint16_t Privod::currentPositionAdr;
volatile uint16_t Privod::maxPositionAdr;
volatile uint16_t Privod::dovodkaPositionAdr;
volatile uint16_t Privod::hummerPositionAdr;
volatile uint16_t Privod::maxSpeedAdr;
volatile uint16_t Privod::minSpeedAdr;
volatile uint16_t Privod::numberOfHummersAdr;

//==========================Constructor_of_class_Privod============================================================
Privod::Privod()
{
	mode = 0;
	counterDir = 0;
	crashedCounter = 0;
	timer0 = 0;
	filterTime = 0;
	crashedTime = 110;
	koncevikFlag = false;
	crashedFlag = false;
	closedFlag = false;
	numberOfCrashes = 0;
		
	currentPositionAdr = 20;
	maxPositionAdr = 26;
	maxSpeedAdr = 32;
	minSpeedAdr = 38;
	dovodkaPositionAdr = 44;
	hummerPositionAdr = 50;
	numberOfHummersAdr = 56;
	
	if(eeprom_read_byte((uint8_t*)500) == 0x00)
	{
		currentPosition = readCurrentPosition();
		maxPosition = readMaxPosition();
		maxSpeed = readMaxSpeed();
		minSpeed = readMinSpeed();
		hummerPosition = readHummerPosition();
		dovodkaPosition = readDovodkaPosition();		
	}
	
	else if(eeprom_read_byte((uint8_t*)500) == 0xFF)
	{
		if(readCurrentPosition() > 1500)
		{
			writeCurrentPosition(0);
		}
		currentPosition = readCurrentPosition();
		maxPosition = 155;
		maxSpeed = 600;
		minSpeed = 170;
		hummerPosition = 50;
		dovodkaPosition = 50;		
	}
}
//=================================================================================================================

//==========================public_methods_of_class_Privod=========================================================
//инициализация Timer0 в режиме прерывания по переполнению 17.6Гц
void Privod::Init_Timer0_Crash(void) {
	TCCR0B = (1 << CS02)|(0 << CS01)|(1 << CS00);
	TIMSK0 = (1 << TOIE0);
}

//инициализация Timer1 в режиме Fast PWM 50Гц
void Privod::Init_Timer1_PWM(void) {
		TCCR1D = (1 << OC1BU)|(1 << OC1AU);
		ICR1 = 720;
		TCCR1A = (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(1 << WGM11)|(0 << WGM10);
		TCCR1B = (1 << WGM13)|(1 << WGM12)|(0 << CS12)|(1 << CS11)|(1 << CS10);
}

//инициализация INT1
void Privod::Init_Int1_Holla(void) {
	EICRA = (1 << ISC11)|(0 << ISC10);
	EIMSK = (1 << INT1);
}

unsigned int Privod::get_polarity(void) {
		if( (PINB & (1 << 2)) == 0b00000000 ) {return 0;}
		if( (PINB & (1 << 2)) == 0b00000100 ) {return 1;}
}

uint16_t Privod::getAnalysis(uint16_t polarity)
{
	if((koncevikFlag == false) && (crashedFlag == false))
	{
		if(polarity == 0)
		{
			if(currentPosition < maxPosition) {mode = Modes::OPEN;}
			else {mode = Modes::STOP;}
		}
		
		else if(polarity == 1)
		{
			if(currentPosition > dovodkaPosition) {mode = Modes::CLOSE;}
			else if((currentPosition <= dovodkaPosition) && (closedFlag == false)) {mode = Modes::DOVODKA;}
			else if((currentPosition <= dovodkaPosition) && (closedFlag == true)) {mode = Modes::STOP;}
		}
	}
	
	else if((koncevikFlag == true) && (crashedFlag == false))
	{
		mode = Modes::KONCEVIK;
	}
	
	else if((koncevikFlag == false) && (crashedFlag == true))
	{
		if(polarity == 0)
		{
			if((currentPosition <= hummerPosition) && (numberOfCrashes <= 3)) {mode = Modes::HUMMER;}
			else if((currentPosition > hummerPosition) && (numberOfCrashes < 5)) {mode = Modes::STOP;}
			else {mode = Modes::WAITING;}	
		}
		
		else if(polarity == 1)
		{
			if((currentPosition <= dovodkaPosition) && (numberOfCrashes < 5)) {mode = Modes::BACK;}
			else if((currentPosition > dovodkaPosition) && (numberOfCrashes < 5)) {mode = Modes::STOP;}
			else {mode = Modes::WAITING;}	
		}
	}
	
	else
	{
		mode = Modes::WAITING;
	}
	
	return mode;
}

uint16_t Privod::getMaxSpeed(void)
{
	return maxSpeed;
}

uint16_t Privod::getMinSpeed(void)
{
	return minSpeed;
}

void Privod::clearCrashedFlag(void)
{
	crashedFlag = false;
}

void Privod::clearClosedFlag(void)
{
	closedFlag = false;
}

//включение канала_1 в инверсном Fast PWM
void Privod::close(uint16_t speed_1) {
		DDRB &= ~(1 << 0);
		DDRB |= (1 << 1);
		crashedCounter = 1;
		if(currentPosition > 0) {counterDir = -1;}
		if(speed_1 == maxSpeed) {filterTime = 0;}
		else if(speed_1 == minSpeed) {filterTime = 0;}
		OCR1B = speed_1; //установка коэффициента заполнения
		TCCR1A = (0 << COM1A1)|(0 << COM1A0)|(1 << COM1B1)|(1 << COM1B0)|(1 << WGM11)|(0 << WGM10);
}

//включение канала_2 в инверсном Fast PWM
void Privod::open(uint16_t speed_2) {
		DDRB &= ~(1 << 1);
		DDRB |= (1 << 0);
		crashedCounter = 1;	
		if(currentPosition < maxPosition) {counterDir = 1;}	
		if(speed_2 == maxSpeed) {filterTime = 0;}
		else if(speed_2 == minSpeed) {filterTime = 0;}
		OCR1A = speed_2; //установка коэффициента заполнения
		TCCR1A = (1 << COM1A1)|(1 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(1 << WGM11)|(0 << WGM10);
}

//отключение канала_A и канала_B
void Privod::stop(void) {
		DDRB &= ~(1 << 0);
		DDRB &= ~(1 << 1);
		crashedCounter = 0;
		counterDir = 0;
		timer0 = 0;
		OCR1A = 0; //установка коэффициента заполнения
		OCR1B = 0; //установка коэффициента заполнения
		TCCR1A = (0 << COM1A1)|(0 << COM1A0)|(0 << COM1B1)|(0 << COM1B0)|(1 << WGM11)|(0 << WGM10);
}

//включение режима молотка
void Privod::hummer(void) {
		for(int i = 0; i < 5; i++)
		{
			open(720);
			_delay_ms(200);
			stop();
			_delay_ms(300);
		}
}

//подчсет числа импульсов от датчика Холла
void Privod::Int1_Holla(void)
{
	if((currentPosition == 0) && ((PINB & (1 << 2)) == 0b00000100))
	{
		counterDir = 0;
	}
	
	if(timer0 > filterTime)
	{
	currentPosition = currentPosition + counterDir;
	writeCurrentPosition(currentPosition);
	timer0 = 0;
	}
}

//проверка заклинивания
void Privod::Timer0_Crash(void) {
	/*//ускорение
	DDRA &= ~(1 << 6);
	if(( (PINA & (1 << 6)) == 0b01000000) && ( (PINB & (1 << 2)) == 0b00000000))
	{
		OCR1A = 720;
	}
	else if(( (PINA & (1 << 6)) == 0b00000000) && ( (PINB & (1 << 2)) == 0b00000000))
	{
		OCR1A = 600;
	}**/
	
	// сработал концевик
	DDRB &= ~(1 << 6);
	if( (PINB & (1 << 6)) == 0b00000000 ) 
	{
		koncevikFlag = true;
	}
	
	timer0 += crashedCounter;
	if(timer0 > crashedTime)
	{
		if((currentPosition <= 20) && ((PINB & (1 << 2)) == 0b00000100))
		{
			closedFlag = true;
		}
		
		else
		{
			crashedFlag = true;
			numberOfCrashes++;
		}
		timer0 = 0;
	}
}

void Privod::writeCurrentPosition(uint16_t newCurrentPosition)
{
	eeprom_write_word((uint16_t*)currentPositionAdr, newCurrentPosition);
	eeprom_write_word((uint16_t*)currentPositionAdr+2, newCurrentPosition);
	eeprom_write_word((uint16_t*)currentPositionAdr+4, newCurrentPosition);
}

unsigned int Privod::readCurrentPosition(void)
{
	if(eeprom_read_word((uint16_t*)currentPositionAdr) == eeprom_read_word((uint16_t*)currentPositionAdr+2))
	{
		currentPosition = eeprom_read_word((uint16_t*)currentPositionAdr);
	}
	else if(eeprom_read_word((uint16_t*)currentPositionAdr) == eeprom_read_word((uint16_t*)currentPositionAdr+4))
	{
		currentPosition = eeprom_read_word((uint16_t*)currentPositionAdr);
	}	
	else if(eeprom_read_word((uint16_t*)currentPositionAdr+2) == eeprom_read_word((uint16_t*)currentPositionAdr+4))
	{
		currentPosition = eeprom_read_word((uint16_t*)currentPositionAdr+2);
	}	
	return currentPosition;
}

unsigned int Privod::readMaxPosition(void)
{
	if(eeprom_read_word((uint16_t*)maxPositionAdr) == eeprom_read_word((uint16_t*)maxPositionAdr+2))
	{
		maxPosition = eeprom_read_word((uint16_t*)maxPositionAdr);
	}
	else if(eeprom_read_word((uint16_t*)maxPositionAdr) == eeprom_read_word((uint16_t*)maxPositionAdr+4))
	{
		maxPosition = eeprom_read_word((uint16_t*)maxPositionAdr);
	}
	else if(eeprom_read_word((uint16_t*)maxPositionAdr+2) == eeprom_read_word((uint16_t*)maxPositionAdr+4))
	{
		maxPosition = eeprom_read_word((uint16_t*)maxPositionAdr+2);
	}	
	return maxPosition;
}

unsigned int Privod::readMaxSpeed(void)
{
	if(eeprom_read_word((uint16_t*)maxSpeedAdr) == eeprom_read_word((uint16_t*)maxSpeedAdr+2))
	{
		maxSpeed = eeprom_read_word((uint16_t*)maxSpeedAdr);
	}
	else if(eeprom_read_word((uint16_t*)maxSpeedAdr) == eeprom_read_word((uint16_t*)maxSpeedAdr+4))
	{
		maxSpeed = eeprom_read_word((uint16_t*)maxSpeedAdr);
	}	
	else if(eeprom_read_word((uint16_t*)maxSpeedAdr+2) == eeprom_read_word((uint16_t*)maxSpeedAdr+4))
	{
		maxSpeed = eeprom_read_word((uint16_t*)maxSpeedAdr+2);
	}	
	return maxSpeed;
}

unsigned int Privod::readMinSpeed(void)
{
	if(eeprom_read_word((uint16_t*)minSpeedAdr) == eeprom_read_word((uint16_t*)minSpeedAdr+2))
	{
		minSpeed = eeprom_read_word((uint16_t*)minSpeedAdr);
	}
	else if(eeprom_read_word((uint16_t*)minSpeedAdr) == eeprom_read_word((uint16_t*)minSpeedAdr+4))
	{
		minSpeed = eeprom_read_word((uint16_t*)minSpeedAdr);
	}
	else if(eeprom_read_word((uint16_t*)minSpeedAdr+2) == eeprom_read_word((uint16_t*)minSpeedAdr+4))
	{
		minSpeed = eeprom_read_word((uint16_t*)minSpeedAdr+2);
	}		
	return minSpeed;
}

unsigned int Privod::readHummerPosition(void)
{
	if(eeprom_read_word((uint16_t*)hummerPositionAdr) == eeprom_read_word((uint16_t*)hummerPositionAdr+2))
	{
		hummerPosition = eeprom_read_word((uint16_t*)hummerPositionAdr);
	}	
	else if(eeprom_read_word((uint16_t*)hummerPositionAdr) == eeprom_read_word((uint16_t*)hummerPositionAdr+4))
	{
		hummerPosition = eeprom_read_word((uint16_t*)hummerPositionAdr);
	}	
	else if(eeprom_read_word((uint16_t*)hummerPositionAdr+2) == eeprom_read_word((uint16_t*)hummerPositionAdr+4))
	{
		hummerPosition = eeprom_read_word((uint16_t*)hummerPositionAdr+2);
	}	
	return hummerPosition;
}

unsigned int Privod::readDovodkaPosition(void)
{
	if(eeprom_read_word((uint16_t*)dovodkaPositionAdr) == eeprom_read_word((uint16_t*)dovodkaPositionAdr+2))
	{
		dovodkaPosition = eeprom_read_word((uint16_t*)dovodkaPositionAdr);
	}
	else if(eeprom_read_word((uint16_t*)dovodkaPositionAdr) == eeprom_read_word((uint16_t*)dovodkaPositionAdr+4))
	{
		dovodkaPosition = eeprom_read_word((uint16_t*)dovodkaPositionAdr);
	}	
	else if(eeprom_read_word((uint16_t*)dovodkaPositionAdr+2) == eeprom_read_word((uint16_t*)dovodkaPositionAdr+4))
	{
		dovodkaPosition = eeprom_read_word((uint16_t*)dovodkaPositionAdr+2);
	}	
	return dovodkaPosition;
}
//=================================================================================================================