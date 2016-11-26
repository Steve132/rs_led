#include <avr/pgmspace.h>
// ******** DEBUG ==== should auto config to adapt different mother board *********
//#define DATA_1 (PORTF |=  0X01)    // DATA 1    // for ATMEGA
//#define DATA_0 (PORTF &=  0XFE)    // DATA 0    // for ATMEGA
//#define STRIP_PINOUT DDRF=0xFF  // for ATMEGA
 
#define DATA_1 (PORTC |=  0X01)    // DEFINE a value for DATA as 1  // for UNO
#define DATA_0 (PORTC &=  0XFE)    // DEFINE a value for DATA 0   // for UNO
#define STRIP_PINOUT (DDRC=0x3F)  // DEFINE PORTC as OUTPUT // for UNO (change pins 0-5; leave PORTC 6 & 7 alone)

#define NOP1 "nop\n\t"
#define NOP2 NOP1 NOP1
#define NOP4 NOP2 NOP2
#define NOP8 NOP4 NOP4
#define NOP16 NOP8 NOP8


class LedStripRS
{
private:
	int pin;
	void send_single(uint32_t data)
	{
		int i;
		unsigned long j=0x800000;


		for (i=0;i<24;i++)
		{
			if (data & j)
			{
				//digitalWrite(pin,HIGH);
				DATA_1;				
				__asm__(NOP16 NOP2 NOP4 NOP2);
				//digitalWrite(pin,LOW);
				DATA_0;
			}
			else
			{
				//digitalWrite(pin,HIGH);
				DATA_1;
				__asm__(NOP8 NOP1); 
				//digitalWrite(pin,LOW);
				DATA_0;				
				/*----------------------------*/    
				__asm__(NOP2 NOP1);    
				/*----------------------------*/      
			}

			j>>=1;
		}
	}
public:
	LedStripRS()
	{}
	void setup(int ipin)
	{
		pin=ipin;
		pinMode(pin, OUTPUT);      // sets the digital pin as output	
	}

	void send_color(uint32_t c)
	{
		int j=0;
		noInterrupts(); // Turn OFF Interupts for more precise Pulse Timing
		for (j=0;j<10;j++)
		{
			send_single(c);
		}
		interrupts(); // Turn ON Interrupts after data is sent
	}
	void reset_strip()
	{
	  DATA_0;
	  delayMicroseconds(20);
	}
};
  
LedStripRS trs;

void setup() {              
 
  trs.setup(A0);
  trs.reset_strip();        // resets each of the LED nodes to OFF 
 
}

void loop()
{
  //send_1M_pattern(pattern_test_red, 10, 500);
  trs.send_color(0xFFFF00);
  delay(1000);
}
