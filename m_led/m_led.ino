#include "LedStripRS.h"
  
LedStripRS<A0> trs;

static BaseLedStripRS* strips[]=
{
	&trs
};

static const int num_strips=sizeof(strips)/sizeof(strips[0]);

void send_color_all(uint32_t color)
{
	for(int i=0;i<num_strips;i++)
	{
		strips[i]->send_color(color);
	}
}

#define RAINBOW_SLOWNESS 25
PROGMEM static const uint32_t patterns[]=
{
	//4th of july
	4,
	500,0x0000FF,250,0xFFFFFF,500,0x00FF00,250,0xFFFFFF,
	//rainbow (72)
	72,
	RAINBOW_SLOWNESS,0x0000ff,RAINBOW_SLOWNESS,0x0015ff,RAINBOW_SLOWNESS,0x002bff,RAINBOW_SLOWNESS,0x0040ff,RAINBOW_SLOWNESS,0x0055ff,
	RAINBOW_SLOWNESS,0x006aff,RAINBOW_SLOWNESS,0x0080ff,RAINBOW_SLOWNESS,0x0095ff,RAINBOW_SLOWNESS,0x00aaff,RAINBOW_SLOWNESS,0x00bfff,
	RAINBOW_SLOWNESS,0x00d5ff,RAINBOW_SLOWNESS,0x00eaff,RAINBOW_SLOWNESS,0x00ffff,RAINBOW_SLOWNESS,0x00ffea,RAINBOW_SLOWNESS,0x00ffd4,
	RAINBOW_SLOWNESS,0x00ffbf,RAINBOW_SLOWNESS,0x00ffaa,RAINBOW_SLOWNESS,0x00ff95,RAINBOW_SLOWNESS,0x00ff80,RAINBOW_SLOWNESS,0x00ff6a,
	RAINBOW_SLOWNESS,0x00ff55,RAINBOW_SLOWNESS,0x00ff40,RAINBOW_SLOWNESS,0x00ff2a,RAINBOW_SLOWNESS,0x00ff15,RAINBOW_SLOWNESS,0x00ff00,
	RAINBOW_SLOWNESS,0x15ff00,RAINBOW_SLOWNESS,0x2bff00,RAINBOW_SLOWNESS,0x40ff00,RAINBOW_SLOWNESS,0x55ff00,RAINBOW_SLOWNESS,0x6aff00,
	RAINBOW_SLOWNESS,0x80ff00,RAINBOW_SLOWNESS,0x95ff00,RAINBOW_SLOWNESS,0xaaff00,RAINBOW_SLOWNESS,0xbfff00,RAINBOW_SLOWNESS,0xd5ff00,
	RAINBOW_SLOWNESS,0xeaff00,RAINBOW_SLOWNESS,0xffff00,RAINBOW_SLOWNESS,0xffea00,RAINBOW_SLOWNESS,0xffd400,RAINBOW_SLOWNESS,0xffbf00,
	RAINBOW_SLOWNESS,0xffaa00,RAINBOW_SLOWNESS,0xff9500,RAINBOW_SLOWNESS,0xff8000,RAINBOW_SLOWNESS,0xff6a00,RAINBOW_SLOWNESS,0xff5500,
	RAINBOW_SLOWNESS,0xff4000,RAINBOW_SLOWNESS,0xff2b00,RAINBOW_SLOWNESS,0xff1500,RAINBOW_SLOWNESS,0xff0000,RAINBOW_SLOWNESS,0xff0015,
	RAINBOW_SLOWNESS,0xff002b,RAINBOW_SLOWNESS,0xff0040,RAINBOW_SLOWNESS,0xff0055,RAINBOW_SLOWNESS,0xff006a,RAINBOW_SLOWNESS,0xff0080,
	RAINBOW_SLOWNESS,0xff0095,RAINBOW_SLOWNESS,0xff00aa,RAINBOW_SLOWNESS,0xff00bf,RAINBOW_SLOWNESS,0xff00d5,RAINBOW_SLOWNESS,0xff00ea,
	RAINBOW_SLOWNESS,0xff00ff,RAINBOW_SLOWNESS,0xea00ff,RAINBOW_SLOWNESS,0xd400ff,RAINBOW_SLOWNESS,0xbf00ff,RAINBOW_SLOWNESS,0xaa00ff,
	RAINBOW_SLOWNESS,0x9500ff,RAINBOW_SLOWNESS,0x8000ff,RAINBOW_SLOWNESS,0x6a00ff,RAINBOW_SLOWNESS,0x5500ff,RAINBOW_SLOWNESS,0x4000ff,
	RAINBOW_SLOWNESS,0x2b00ff,RAINBOW_SLOWNESS,0x1500ff
};

static const int size_patterns=sizeof(patterns)/sizeof(patterns[0]);

int iterate_pattern(int pptr,int pattern_frame_id,uint8_t brightness,uint32_t slowdown_factor)
{
	uint32_t len=pgm_read_dword_near(patterns+pptr);
	
	int pfidlu=pattern_frame_id << 1;
	uint32_t pslowness=pgm_read_dword_near(patterns+pptr+1+pfidlu);
	uint32_t color=pgm_read_dword_near(patterns+pptr+2+pfidlu);
	color=brighten(color,brightness);
	send_color_all(color);
	pslowness=slowdown(pslowness,slowdown_factor);
	delay(pslowness);
	pattern_frame_id++;
	if(pattern_frame_id >= len)
	{
		pattern_frame_id=0;
	}
	return pattern_frame_id;
}

int increment_pattern(int pptr)
{
	pptr+=(pgm_read_dword_near(patterns+pptr) << 1);
	pptr++;
	if(pptr >= size_patterns)
	{
		pptr=0;
	}
	return pptr;
}


int pframeid=0;
int curptr=0;
void goto_pattern(int pattern_id)
{
	curptr=0;
	pframeid=0;
	for(int i=0;i<pattern_id;i++)
	{
		curptr=increment_pattern(curptr);
	}
}

uint8_t curbrightness=255;	//brightness is X=curbrightness/255.  Max brightness is set here
uint32_t curslowdown=1024; //Slowness factor is X=slowdown/1024.  For example, set slowdown to 2048 to get a 2x slowdown.  set slowdown to 512 to get a 50% slowdown (2x speedup).  

void setup() 
{              
	for(int i=0;i<num_strips;i++)
	{
		strips[i]->setup();
	}
	goto_pattern(1);
}
void loop()
{	
	pframeid=iterate_pattern(curptr,pframeid,curbrightness,curslowdown);
}
