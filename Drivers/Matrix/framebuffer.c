//framebuffer.c

//TODO: sort functions
#include "stm32f4xx.h"
#include "framebuffer.h"
#include "main.h"
#include "fonts/6x8_vertikal_LSB_1.h"
#include <string.h>



static const int PIXEL_H_OFFSET = (0);
static const unsigned int CHAR_NUM_BREAK = 10;
static const unsigned int CHAR_BREAK_PIXELS = 4;


void Framebuffer_Init(void)
{
    framebufferR[0].w = 0x0003;
    framebufferG[0].w = 0x0005;
    //framebufferB[0] =0x64;
    for(int i = 1; i < sizeof(framebufferR) / sizeof(framebufferR[0].w); i++)
    {
        framebufferR[i].w = 0;
        framebufferG[i].w = 0;
        //framebufferB[i]=8-i;
        //framebufferR[i].w = 0xFFFF;
        //framebufferG[i].w = 0xFFFF;
    }
    return;
}

void Framebuffer_Clear(void)
{
    //framebufferB[0] =0x64;
    for(int i = 0; i < sizeof(framebufferR) / sizeof(framebufferR[0].w); i++)
    {
        framebufferR[i].w = 0;
        framebufferG[i].w = 0;
        //framebufferB[i]=8-i;
        //framebufferR[i].w = 0xFFFF;
        //framebufferG[i].w = 0xFFFF;
    }
    return;
}


void Framebuffer_Swap()
{
    return;
}


void Framebuffer_Sync()
{
    return;
}


void Framebuffer_Write()
{
    uint32_t tempreg = 0;
    CLEAR_BIT(interruptFlags, INTFLAG_FramebufferWrite);
    for(unsigned int line = 0; line < MATRIX_PANEL_SCANROWS; line++)
    {
        //linemapping because image is shifted down by 1
        unsigned int linemap;
        if(line == 0)
            linemap = 0xF;
        else
            linemap = line - 1;
        //HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET); //set
        OE_GPIO_Port->BSRR = (uint32_t)OE_Pin; // set
        //HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_SET);
        STB_GPIO_Port->BSRR = (uint32_t)STB_Pin; // set
        __NOP();
        __NOP();
        STB_GPIO_Port->BSRR = (uint32_t)STB_Pin << 16U; // reset
        //HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_RESET);
        // Set address
        //HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, (GPIO_PinState)((linemap) % 2));
        //HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, (GPIO_PinState)((linemap >> 1) % 2));
        //HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, (GPIO_PinState)((linemap >> 2) % 2));
        //HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, (GPIO_PinState)((linemap >> 3) % 2));
        tempreg = ((linemap) % 2) != 0 ? (uint32_t) A_Pin : (uint32_t) A_Pin << 16U;
        tempreg += ((linemap >> 1) % 2) != 0 ? (uint32_t) B_Pin : (uint32_t) B_Pin << 16U;
        tempreg += ((linemap >> 2) % 2) != 0 ? (uint32_t) C_Pin : (uint32_t) C_Pin << 16U;
        tempreg += ((linemap >> 3) % 2) != 0 ? (uint32_t) D_Pin : (uint32_t) D_Pin << 16U;
        A_GPIO_Port->BSRR = tempreg;
        HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET); //reset
        //clock data out
        for(unsigned int col = 0; col < MATRIX_WIDTH; col++)
        {
            tempreg  = ((framebufferR[col].w >> line) % 2) != 0 ? (uint32_t) R1_Pin : (uint32_t) R1_Pin << 16U;
            tempreg += ((framebufferG[col].w >> line) % 2) != 0 ? (uint32_t) G1_Pin : (uint32_t) G1_Pin << 16U;
            R1_GPIO_Port->BSRR = tempreg; // set R and G Data Bits
            CLK_GPIO_Port->BSRR = (uint32_t)CLK_Pin << 16U; // set
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            __NOP();
            CLK_GPIO_Port->BSRR = (uint32_t)CLK_Pin; //reset
        }
    }
    return;
}


void fb_error(int errorNo)
{
    char tempstring[6];
    sprintf(tempstring, "ERR %1d", (char)errorNo);
    Framebuffer_Putch((char*)tempstring, 0, RED, 0);
    return;
}




void Framebuffer_Putch(char inputstring[], unsigned int cursorposition, FB_COLOR_t color, char upperlower)
{
    if(color == CLEAR)
    {
        Framebuffer_Clear();
        return;
    }
    for(unsigned int j = cursorposition; j < MAX_FB_LENGTH; j++)
    {
        if(inputstring[j] == NULL)  //End of line reckognition
            return;
        // make a pretty linebreak
        int pixeloffset = (j * FONT_WIDTH) + PIXEL_H_OFFSET - 1;
        //if (j == CHAR_NUM_BREAK)
        //    pixeloffset += CHAR_BREAK_PIXELS;
        if(upperlower == 0)
        {
            for(int i = 0; i < FONT_WIDTH; i++)
            {
                switch(color)
                {
                case OFF:
                    framebufferR[i + pixeloffset].h = 0;
                    framebufferG[i + pixeloffset].h = 0;
                    break;
                case RED:
                    framebufferR[i + pixeloffset].h = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + pixeloffset].h = 0;
                    break;
                case GREEN:
                    framebufferR[i + pixeloffset].h = 0;
                    framebufferG[i + pixeloffset].h = font[inputstring[j - cursorposition]][i];
                    break;
                case YELLOW:
                    framebufferR[i + pixeloffset].h = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + pixeloffset].h = font[inputstring[j - cursorposition]][i];
                    break;
                default:
                    fb_error(1);
                    return;
                }
            }
        }
        else if(upperlower == 1)
        {
            for(int i = 0; i < FONT_WIDTH; i++)
            {
                switch(color)
                {
                case OFF:
                    framebufferR[i + pixeloffset].l = 0;
                    framebufferG[i + pixeloffset].l = 0;
                    break;
                case RED:
                    framebufferR[i + pixeloffset].l = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + pixeloffset].l = 0;
                    break;
                case GREEN:
                    framebufferR[i + pixeloffset].l = 0;
                    framebufferG[i + pixeloffset].l = font[inputstring[j - cursorposition]][i];
                    break;
                case YELLOW:
                    framebufferR[i + pixeloffset].l = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + pixeloffset].l = font[inputstring[j - cursorposition]][i];
                    break;
                default:
                    fb_error(2);
                    return;
                }
            }
        }
        else
        {
            char tempstring[] = "ERR UL";
            Framebuffer_Putch(tempstring, 0, RED, 0);
            fb_error(0);
        }
    }
}
