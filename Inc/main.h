//main.h

//#include "config.h"
//#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "stm32f4xx_hal_spi.h"
#include "mxconstants.h"
#include "config.h"
#include "matrix.h"
#include "framebuffer.h"
#include "stdint.h"


#include <stdio.h>
#include <stdlib.h>

#ifndef MAIN_H
#define MAIN_H

#pragma anon_unions
typedef union uint2uchar_neew{
	uint16_t w;
	struct{
		unsigned char l;
		unsigned char h;
	};
} framebuffer_t;

static framebuffer_t framebufferR [MATRIX_WIDTH+1];
static framebuffer_t framebufferG [MATRIX_WIDTH+1];

static volatile unsigned int templine = 0;

#define INTFLAG_FramebufferWrite	0x0001
#define INTFLAG_ExtInt				0x0002
static volatile char interruptFlags = INTFLAG_FramebufferWrite;

#define MAX_FB_LENGTH (MATRIX_WIDTH+1)

void fb_error(int errorNo);
void framebuffer_putch(char inputstring[], unsigned int cursorposition, unsigned int color, char upperlower);

static RTC_TimeTypeDef currenttime;
static RTC_DateTypeDef currentdate;
static RTC_TimeTypeDef lasttime;
static RTC_TimeTypeDef difftime;

static unsigned int prediv = 15625;

#endif //MAIN_H
