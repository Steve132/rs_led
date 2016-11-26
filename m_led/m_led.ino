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

typedef struct pattern_ts
{
	const uint32_t colors[];
	int length;
	int slowness;
} pattern_t;

pattern_t patterns[]={
	{
		{0x00FF00,0xFF0000,0x0000FF},
		3,
		500
	}
};
static const int num_patterns=sizeof(patterns)/sizeof(patterns[0]);

int iterate_pattern(int pid,int pattern_frame_id)
{
	pattern_t *selpattern=&patterns[pid % num_patterns];
	const uint32_t *frames=selpattern->colors;
	send_color_all(frames[pattern_frame_id % selpattern->length]);
	delay(selpattern->slowness);
	pattern_frame_id++;
	return pattern_frame_id;
} 

int pframeid=0;
int curpattern=0;

void loop()
{
	pframeid=iterate_pattern(curpattern,pframeid);
}
