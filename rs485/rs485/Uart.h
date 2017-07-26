#ifndef Uart_H
#define Uart_H

class Uart 
{	
	private:
			static volatile unsigned int rxCounter;
			static volatile unsigned char rxBuff[15];
			static volatile unsigned char txBuff[15];
			static volatile uint16_t loadingMode;
	
	public:
			void initUart(void);
			void sendUartEn(void);
			void getchUartEn(void);
			void sendUart(unsigned char);
	 static void readUart(void);
			unsigned char getchUart(void);
			void clearUart(void);
	 static uint8_t getLoadingMode(void);
			void setLoadingMode(uint16_t mode);
};

#endif