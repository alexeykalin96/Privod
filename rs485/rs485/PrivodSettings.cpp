#include "PrivodSettings.h"
#include <avr/io.h>
#include <avr/eeprom.h>

//==========================Constructor_of_class_PrivodSettings==========================================
PrivodSettings::PrivodSettings()
{
	currentPositionAdr = 20;
	maxPositionAdr = 26;
	maxSpeedAdr = 32;
	minSpeedAdr = 38;
	dovodkaPositionAdr = 44;
	hummerPositionAdr = 50;
	numberOfHummersAdr = 56;	
}
//=================================================================================================================

//==========================public_methods_of_class_PrivodSettings==========================================
void PrivodSettings::setCurrentPosition(uint16_t newCurrentPosition)
{
	eeprom_write_word((uint16_t*)currentPositionAdr, newCurrentPosition);
	eeprom_write_word((uint16_t*)currentPositionAdr+2, newCurrentPosition);
	eeprom_write_word((uint16_t*)currentPositionAdr+4, newCurrentPosition);
}

void PrivodSettings::setMaxPosition(uint16_t newMaxPosition)
{
	eeprom_write_word((uint16_t*)maxPositionAdr, newMaxPosition);
	eeprom_write_word((uint16_t*)maxPositionAdr+2, newMaxPosition);
	eeprom_write_word((uint16_t*)maxPositionAdr+4, newMaxPosition);
}

void PrivodSettings::setMaxSpeed(uint16_t newMaxSpeed)
{
	eeprom_write_word((uint16_t*)maxSpeedAdr, newMaxSpeed);
	eeprom_write_word((uint16_t*)maxSpeedAdr+2, newMaxSpeed);
	eeprom_write_word((uint16_t*)maxSpeedAdr+4, newMaxSpeed);
}

void PrivodSettings::setMinSpeed(uint16_t newMinSpeed)
{
	eeprom_write_word((uint16_t*)minSpeedAdr, newMinSpeed);
	eeprom_write_word((uint16_t*)minSpeedAdr+1, newMinSpeed);
	eeprom_write_word((uint16_t*)minSpeedAdr+2, newMinSpeed);
}

void PrivodSettings::setDovodkaPosition(uint16_t newDovodkaPosition)
{
	eeprom_write_word((uint16_t*)dovodkaPositionAdr, newDovodkaPosition);
	eeprom_write_word((uint16_t*)dovodkaPositionAdr+2, newDovodkaPosition);
	eeprom_write_word((uint16_t*)dovodkaPositionAdr+4, newDovodkaPosition);
}

void PrivodSettings::setHummerPosition(uint16_t newHummerPosition)
{
	eeprom_write_word((uint16_t*)hummerPositionAdr, newHummerPosition);
	eeprom_write_word((uint16_t*)hummerPositionAdr+2, newHummerPosition);
	eeprom_write_word((uint16_t*)hummerPositionAdr+4, newHummerPosition);
}

void PrivodSettings::setNumberOfHummers(uint16_t newNumberOfHummers)
{
	eeprom_write_word((uint16_t*)numberOfHummersAdr, newNumberOfHummers);
	eeprom_write_word((uint16_t*)numberOfHummersAdr+2, newNumberOfHummers);
	eeprom_write_word((uint16_t*)numberOfHummersAdr+4, newNumberOfHummers);
}
//=================================================================================================================

