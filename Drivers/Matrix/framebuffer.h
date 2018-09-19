/*
 * framebuffer.h
 *
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_
#include <stdint.h>
#include "config.h"

void Framebuffer_Init(void);
void Framebuffer_Swap(void);
void Framebuffer_Sync(void);
void Framebuffer_Write(void);
void fb_error(int errorNo);

typedef enum FB_COLOR_e
{
    OFF = 0,
    GREEN = 1,
    RED = 2,
    YELLOW = 3,
    CLEAR = 4
} FB_COLOR_t;

#pragma anon_unions
typedef union framebuffer_ut_
{
    uint16_t w;
    struct
    {
        char l;
        char h;
    };
} framebuffer_ut;


#define MAX_FB_LENGTH (MATRIX_WIDTH + 1)

void Framebuffer_Putch(char inputstring[], unsigned int cursorposition, FB_COLOR_t color, char upperlower);

extern framebuffer_ut framebufferR [MATRIX_WIDTH + 1];
extern framebuffer_ut framebufferG [MATRIX_WIDTH + 1];
#endif /* FRAMEBUFFER_H_ */
