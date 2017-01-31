Wiring
===
HUB75 pins
---
see pin-layout on pcb


The LED panel on the right is where the signal should be piped in from the STM32F4Discovery board.

STM32F4Discovery matrix pins
---
| STM32 | Name |
| --- | --- |
| GND | GND (Don't forget to connect this!) |
| PD0 | R1 |
| PD1 | G1 |
| PD2 | B1 |
| PD3 | R2 |
| PD4 | G2 |
| PD5 | B2 |
| PE2 | A |
| PE3 | B |
| PE4 | C |
| PE5 | D (must be available)|
| PD6 | CLK |
| PE6 | STB |
| PC6 | OE |

see also [https://github.com/Frans-Willem/LEDMatrixHUB75/]