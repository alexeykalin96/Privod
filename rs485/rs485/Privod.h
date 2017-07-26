#ifndef Privod_H
#define Privod_H

class Privod
{
	private:
		static volatile uint16_t mode;
		static volatile uint16_t currentPosition;
		static volatile uint16_t maxPosition;
		static volatile uint16_t dovodkaPosition;
		static volatile uint16_t hummerPosition;
		static volatile uint16_t maxSpeed;
		static volatile uint16_t minSpeed;
		static volatile uint16_t numberOfCrashes;
		static volatile int16_t counterDir;
		static volatile uint16_t crashedCounter;
		static volatile uint16_t timer0;
		static volatile uint16_t crashedTime;
		static volatile uint16_t filterTime;
		static volatile bool closedFlag;
		static volatile bool crashedFlag;
		static volatile bool koncevikFlag;
		
		static volatile uint16_t currentPositionAdr;
		static volatile uint16_t maxPositionAdr;
		static volatile uint16_t dovodkaPositionAdr;	
		static volatile uint16_t hummerPositionAdr;
		static volatile uint16_t maxSpeedAdr;
		static volatile uint16_t minSpeedAdr;
		static volatile uint16_t numberOfHummersAdr;
		
	public:
		Privod();
		void Init_Timer0_Crash(void);
		void Init_Timer1_PWM(void);
		void Init_Int1_Holla(void);
		uint16_t get_polarity(void);
		uint16_t getAnalysis(uint16_t polarity);
		uint16_t getMaxSpeed(void);
		uint16_t getMinSpeed(void);
		void clearCrashedFlag(void);
		void clearClosedFlag(void);
		void close(uint16_t speed_1);
		void open(uint16_t speed_2);
		void stop(void);
		void hummer(void);
		static void Int1_Holla(void);
		static void Timer0_Crash(void);
		static void writeCurrentPosition(uint16_t newCurrentPosition);
		unsigned int readCurrentPosition(void);
		unsigned int readMaxPosition(void);
		unsigned int readMaxSpeed(void);
		unsigned int readMinSpeed(void);
		unsigned int readHummerPosition(void);
		unsigned int readDovodkaPosition(void);
	
};

#endif
