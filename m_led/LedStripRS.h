#ifndef LedStripRS_H
#define LedStripRS_H

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

#define LSPC 2
#define LSPB 1
#define LSPD 0


template<int cs, int spin>
struct _static_pin_part2
{
	inline static void set() __attribute__((always_inline));
	inline static void clear() __attribute__((always_inline));
};

template<int spin>
struct _static_pin_part2<LSPD,spin>
{
	inline static void set() __attribute__((always_inline))
	{
		PORTD |= (0x1 << spin);
	}
	inline static void clear() __attribute__((always_inline))
	{
		PORTD &= ~(0x1 << spin);
	}
};


template<int spin>
struct _static_pin_part2<LSPB,spin>
{
	inline static void set() __attribute__((always_inline))
	{
		PORTB |= (0x1 << (spin-8));
	}
	inline static void clear() __attribute__((always_inline))
	{
		PORTB &= ~(0x1 << (spin-8));
	}
};

template<int spin>
struct _static_pin_part2<LSPC,spin>
{
	inline static void set() __attribute__((always_inline))
	{
		PORTC |= (0x1 << (spin-14));
	}
	inline static void clear() __attribute__((always_inline))
	{
		PORTC  &= ~(0x1 << (spin-14));
	}
};


template<int spin>
struct _static_pin
{
	typedef _static_pin_part2<(spin < 8) ? ( LSPD ) : ( (spin < 14) ? LSPB : LSPC ),spin> _part2;
	inline static void set() __attribute__((always_inline))
	{
		_part2::set();
	}
	inline static void clear() __attribute__((always_inline))
	{
		_part2::clear();
	} 
};

template<int spin,int length=10>
class LedStripRS
{
private:
	void send_single(uint32_t data)
	{
		int i;
		unsigned long j=0x800000;


		for (i=0;i<24;i++)
		{
			if (data & j)
			{
				//digitalWrite(pin,HIGH);
				_static_pin<spin>::set();				
				__asm__(NOP16 NOP8);
				//digitalWrite(pin,LOW);
				_static_pin<spin>::clear();
			}
			else
			{
				//digitalWrite(pin,HIGH);
				_static_pin<spin>::set();
				__asm__(NOP8 NOP1); 
				//digitalWrite(pin,LOW);
				_static_pin<spin>::clear();
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
	void setup()
	{
		pinMode(spin, OUTPUT);      // sets the digital pin as output	
	}
	void send_pattern(const uint32_t data[])
	{
		int j=0;
		noInterrupts(); // Turn OFF Interupts for more precise Pulse Timing
		for (j=0;j<length;j++)
		{
			uint32_t c=pgm_read_dword_near(&data[j]);
			send_single(c);
		}
		interrupts(); // Turn ON Interrupts after data is sent
	}
	void send_color(uint32_t c)
	{
		int j=0;
		noInterrupts(); // Turn OFF Interupts for more precise Pulse Timing
		for (j=0;j<length;j++)
		{
			send_single(c);
		}
		interrupts(); // Turn ON Interrupts after data is sent
	}
	void reset_strip()
	{
	  _static_pin<spin>::clear();
	  delayMicroseconds(20);
	}
};


#endif //LedStripRS
