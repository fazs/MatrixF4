//framebuffer.c

//TODO: sort functions
#include "framebuffer.h"

void framebuffer_init(void)
{
    framebufferR[0].w = 0x3C5A;
    framebufferG[0].w = 0x0FA5;
    //framebufferB[0] =0x64;
    for(int i = 1; i < sizeof(framebufferR) / sizeof(framebufferR[0].w); i++)
    {
        //  framebufferR[i]=i%0x10000;
        //  framebufferG[i]=(UINT16_MAX-i)%0x10000;
        //framebufferB[i]=8-i;
        framebufferR[i].w = 0xFFFF;
        framebufferG[i].w = 0xFFFF;
    }
    return;
}

void framebuffer_swap()
{
    return;
}

void framebuffer_sync()
{
    return;
}


void framebufferWrite()
{
    return;
}
