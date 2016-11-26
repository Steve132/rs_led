#include "LedStripRS.h"
  
LedStripRS<A0> trs;

void setup() 
{              
	trs.setup();
	trs.reset_strip();        // resets each of the LED nodes to OFF 
}

void loop()
{
	//send_1M_pattern(pattern_test_red, 10, 500);
	trs.send_color(0x0000FF);
	delay(1000);
	trs.send_color(0x00FF00);
	delay(1000);
}
