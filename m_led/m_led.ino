#include "LedStripRS.h"
  
LedStripRS<A0> trs;

static BaseLedStripRS* strips[]=
{
	&trs
};

static const int num_strips=sizeof(strips)/sizeof(strips[0]);

void setup() 
{              
	for(int i=0;i<num_strips;i++)
	{
		strips[i]->setup();
	}
}

void send_color_all(uint32_t color)
{
	for(int i=0;i<num_strips;i++)
	{
		strips[i]->send_color(color);
	}
}

void loop()
{
	send_color_all(0x0000FF);
	delay(1000);
	send_color_all(0x00FF00);
	delay(1000);
}
