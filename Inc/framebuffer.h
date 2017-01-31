/*
 * framebuffer.h
 *
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_
#include <stdint.h>
#include "config.h"
#include "main.h"

void framebuffer_init(void);
void framebuffer_swap(void);
void framebuffer_sync(void);
void framebufferWrite(void);

#endif /* FRAMEBUFFER_H_ */
