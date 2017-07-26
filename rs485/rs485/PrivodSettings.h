#ifndef PrivodSettings_H
#define PrivodSettings_H

class PrivodSettings
{
	private:
	volatile uint16_t currentPositionAdr;
	volatile uint16_t maxPositionAdr;
	volatile uint16_t maxSpeedAdr;
	volatile uint16_t minSpeedAdr;
	volatile uint16_t dovodkaPositionAdr;
	volatile uint16_t hummerPositionAdr;
	volatile uint16_t numberOfHummersAdr;
	
	public:
	PrivodSettings();
	void setCurrentPosition(uint16_t newCurrentPosition);
	void setMaxPosition(uint16_t newMaxPosition);
	void setMaxSpeed(uint16_t newMaxSpeed);
	void setMinSpeed(uint16_t newMinSpeed);
	void setDovodkaPosition(uint16_t newDovodkaPosition);
	void setHummerPosition(uint16_t newHummerPosition);
	void setNumberOfHummers(uint16_t newNumberOfHummers);
};

#endif
