#include <avr/pgmspace.h>

uint32_t brighten(uint32_t color,uint8_t brightness)
{
	uint32_t out=0;
	uint32_t r;

	r=color & 0xFF;
	r*=brightness;
	r>>=8;
	r++;
	out |= r;

	r=(color >> 8) & 0xFF;
	r*=brightness;
	r>>=8;
	r++;
	out |= r << 8;

	r=(color >> 16) & 0xFF;
	r*=brightness;
	r>>=8;
	r++;
	out |= r << 16;
	return out;
}
uint32_t slowdown(uint32_t delay,uint32_t sd)  //fixed-point, 10 bit fraction. 
{
	uint32_t output=delay;
	output*=sd;
	output >>=10;
	return output;
}


