# STM32-LED-Matrix-Controller

##Pinout

Pin Nb	PINs	FUNCTIONs	LABELs
2	PC13	GPIO_EXTI13	B1 [Blue PushButton]
3	PC14-OSC32_IN	RCC_OSC32_IN	
4	PC15-OSC32_OUT	RCC_OSC32_OUT	
5	PH0-OSC_IN	RCC_OSC_IN	
6	PH1-OSC_OUT	RCC_OSC_OUT	
16	PA2		USART2_TX	USART_TX
17	PA3		USART2_RX	USART_RX
21	PA5		GPIO_Output	LD2 [Green Led]
26	PB0		GPIO_Output	STB
27	PB1		GPIO_Output	OE
28	PB2		GPIO_Output	CLK
40	PC9		GPIO_Output	R1
42	PA9		USART1_TX	
43	PA10	USART1_RX	
46	PA13	SYS_JTMS-SWDIO	TMS
49	PA14	SYS_JTCK-SWCLK	TCK
51	PC10	GPIO_Output	G1
55	PB3		SYS_JTDO-SWO	SWO
59	PB7		GPIO_Output	A
61	PB8		GPIO_Output	B
62	PB9		GPIO_Output	C
29	PB10	GPIO_Output	D

## Software

In the uVision project the ST-delivered freeRTOS is deactivated. Instead the freeRTOS from KEIL is used, so uVision is able to do RTOS-aware debugging.

## Control

The board expects an NMEA-like string. Every part of the following string must be send, although when using the Clear command the remaining parts get discarded by the program.

"$PFZSDSP,<COLOR>,<LENGTH>,<XPOS>,<YPOS>,<DATA>*<CRC>\CR"

<COLOR> a single CAPITAL ASCII letter, can be either:
*O Off
*R Red
*G Green
*Y Yellow
*C Clear the whole display

<LENGTH> one or two digit base-10 number

<XPOS> one or two digit base-10 number, represents the horizontal start position (counted in characters) to write on the framebuffer

<YPOS> one digit base-10 number, represents the vertical start position (counted in lines) to write on the framebuffer. In this specific configuration it can be either 0 or 1. 

<DATA> ASCII-String of the <LENGTH> stated before

<CRC> two digit base-16 number representing the CRC. See calculation of CRC in the NMEA protocol.

