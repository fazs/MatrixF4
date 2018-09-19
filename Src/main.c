/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal_rtc.h"
#include "main.h"
#include "mxconstants.h"
#include "config.h"
#include "matrix.h"
#include "framebuffer.h"
#include "stdint.h"
#include "stm32f4xx_hal_spi.h"

#include "fonts/6x8_vertikal_LSB_1.h"
#include <stdio.h>
#include <stdlib.h>

#undef RANDOMREDSTUFF
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
RNG_HandleTypeDef hrng;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */


uint16_t subsec = 999;


/* Private variables ---------------------------------------------------------*/
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM8_Init(void);
static void MX_RNG_Init(void);
static void MX_RTC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
typedef enum
{
    OFF = 0,
    GREEN = 1,
    RED = 2,
    YELLOW = 3
} FB_COLOR_t;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void fb_error(int errorNo)
{
    char tempstring[6];
    sprintf(tempstring, "ERR %1d", (char)errorNo);
    framebuffer_putch((char*)tempstring, 0, RED, 0);
    return;
}

void framebuffer_putch(char inputstring[], unsigned int cursorposition, unsigned int color, char upperlower)
{
    for(unsigned int j = cursorposition; j < MAX_FB_LENGTH; j++)
    {
        if(inputstring[j] == NULL)  //End of line reckognition
            return;
        if(upperlower == 0)
        {
            for(int i = 0; i < FONT_WIDTH; i++)
            {
                switch(color)
                {
                case OFF:
                    framebufferR[i + (j * FONT_WIDTH) - 1].h = 0;
                    framebufferG[i + (j * FONT_WIDTH) - 1].h = 0;
                    break;
                case RED:
                    framebufferR[i + (j * FONT_WIDTH) - 1].h = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + (j * FONT_WIDTH) - 1].h = 0;
                    break;
                case GREEN:
                    framebufferR[i + (j * FONT_WIDTH) - 1].h = 0;
                    framebufferG[i + (j * FONT_WIDTH) - 1].h = font[inputstring[j - cursorposition]][i];
                    break;
                case YELLOW:
                    framebufferR[i + (j * FONT_WIDTH) - 1].h = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + (j * FONT_WIDTH) - 1].h = font[inputstring[j - cursorposition]][i];
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
                    framebufferR[i + (j * FONT_WIDTH)].l = 0;
                    framebufferG[i + (j * FONT_WIDTH)].l = 0;
                    break;
                case RED:
                    framebufferR[i + (j * FONT_WIDTH)].l = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + (j * FONT_WIDTH)].l = 0;
                    break;
                case GREEN:
                    framebufferR[i + (j * FONT_WIDTH)].l = 0;
                    framebufferG[i + (j * FONT_WIDTH)].l = font[inputstring[j - cursorposition]][i];
                    break;
                case YELLOW:
                    framebufferR[i + (j * FONT_WIDTH)].l = font[inputstring[j - cursorposition]][i];
                    framebufferG[i + (j * FONT_WIDTH)].l = font[inputstring[j - cursorposition]][i];
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
            framebuffer_putch(tempstring, 0, RED, 0);
            // fb_error(0);
        }
    }
}

int stdin_getchar()
{
    uint8_t getch;
    HAL_UART_Receive(&huart1, &getch, 1, 100);
    return getch;
}

int stdout_putchar(int ch)
{
    uint8_t putch = ch;
    HAL_UART_Transmit(&huart1, &putch, 1, 100);
    return 0;
}
void write_time(char tempstring[])
{
    HAL_RTC_GetTime(&hrtc, &currenttime, RTC_HOURFORMAT_24);
    __disable_irq();
    subsec = (1000 - (RTC->SSR * 1000) / (prediv)) % 1000;
    (void)(RTC->DR);
    __enable_irq();
    //sprintf(tempstring, "%02d:%02d:%02d.%03d", currenttime.Hours, currenttime.Minutes, currenttime.Seconds, currenttime.SubSeconds / (currenttime.SecondFraction + 1));
    //sprintf(tempstring, "%02d:%02d:%02d.%03d", currenttime.Hours, currenttime.Minutes, currenttime.Seconds, subsec);
    char blinkingpoints = ':';
    if(subsec > 500)
        //if(currenttime.Seconds %2)
        blinkingpoints = ' ';
    sprintf(tempstring, "%02d%1c%02d.%03d", currenttime.Minutes, blinkingpoints, currenttime.Seconds, subsec);
    framebuffer_putch(tempstring, 0, GREEN, 0);
}
void write_difftime(char tempstring[])
{
    CLEAR_BIT(interruptFlags, INTFLAG_ExtInt);
    sprintf(tempstring, "%02d:%02d.%03d", difftime.Minutes, difftime.Seconds, difftime.SubSeconds);
    framebuffer_putch(tempstring, 0, YELLOW, 1);
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_0)
    {
        // HAL_RTC_GetTime(&hrtc, &currenttime, RTC_FORMAT_BCD);
        difftime.SubSeconds = 1000 - (currenttime.SubSeconds - lasttime.SubSeconds);
        difftime.Seconds = 0;
        while(difftime.SubSeconds >= 1000)
        {
            difftime.SubSeconds -= 1000;
            difftime.Seconds++;
        }
        difftime.Seconds += currenttime.Seconds - lasttime.Seconds;
        difftime.Minutes = 0;
        while(difftime.Seconds >= 60)
        {
            difftime.Seconds -= 60;
            difftime.Minutes++;
        }
        difftime.Minutes += currenttime.Minutes - lasttime.Minutes;
        difftime.Hours = 0;
        while(difftime.Minutes >= 60)
        {
            difftime.Minutes -= 60;
            difftime.Hours++;
        }
        difftime.Hours += currenttime.Hours - lasttime.Hours;
        lasttime.Hours = currenttime.Hours;
        lasttime.Minutes = currenttime.Minutes;
        lasttime.Seconds = currenttime.Seconds;
        lasttime.SubSeconds = currenttime.SubSeconds;
        SET_BIT(interruptFlags, INTFLAG_ExtInt);
    }
}
/* USER CODE END 0 */

int main(void)
{
    /* USER CODE BEGIN 1 */
#ifdef RANDOMREDSTUFF
    int cycles = 0;
#endif //RANDOMREDSTUFF
    //const char* fontptr = &font[0][0];
    /* USER CODE END 1 */
    /* MCU Configuration----------------------------------------------------------*/
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* Configure the system clock */
    SystemClock_Config();
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_TIM3_Init();
    MX_USART1_UART_Init();
    MX_TIM8_Init();
    MX_RNG_Init();
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    
    //HAL_TIM_OC_MspInit(&htim8);
    //HAL_TIM_OC_Init(&htim8);
    //HAL_TIM_OC_Start_IT(&htim8,TIM_CHANNEL_1);
    //HAL_TIM_Base_MspInit(&htim3);
    //HAL_TIM_OC_Init(&htim3);
    // HAL_TIM_Base_Start_IT(&htim3);
    // prediv = ((RTC->PRER & RTC_PRER_PREDIV_S) + 1);
    // prediv =  15625; //hrtc.Init.SynchPrediv;
    HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_SET);
    framebuffer_init();
    HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
    char tempstring2[22] = ".-.-.";
    framebuffer_putch((char*)tempstring2, 0, RED, 0);
    //char tempstring[] = "Test1234567890";
    char tempstring[22] = "T                     ";
    //framebuffer_putch((char*)tempstring, 4, GREEN, 0);
    framebuffer_putch((char*)tempstring, 0, YELLOW, 1);
    framebuffer_putch((char*)tempstring2, 5, GREEN, 0);
    framebuffer_putch((char*)tempstring2, 10, GREEN, 1);
    /* USER CODE END 2 */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while(1)
    {
#ifdef RANDOMREDSTUFF
        cycles++;
#endif //RANDOMREDSTUFF
        //if (READ_BIT(interruptFlags, INTFLAG_FramebufferWrite))
        if(1)
        {
            CLEAR_BIT(interruptFlags, INTFLAG_FramebufferWrite);
            for(int line = 0; line <= MATRIX_PANEL_SCANROWS; line++)
            {
                //linemapping because image is shifted down by 1
                unsigned int linemap;
                if(line == 0)
                    linemap = 0xF;
                else
                    linemap = line - 1;
                HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_SET);
                HAL_GPIO_WritePin(STB_GPIO_Port, STB_Pin, GPIO_PIN_RESET);
                // Set address
                HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, (GPIO_PinState)((linemap) % 2));
                HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, (GPIO_PinState)((linemap >> 1) % 2));
                HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, (GPIO_PinState)((linemap >> 2) % 2));
                HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, (GPIO_PinState)((linemap >> 3) % 2));
                HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
                //clock data out
                for(int col = 0; col <= MATRIX_WIDTH - 1; col++)
                {
                    HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, (GPIO_PinState)((framebufferR[col].w >> line) % 2));
                    HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, (GPIO_PinState)((framebufferG[col].w >> line) % 2));
                    CLK_GPIO_Port->BSRR = (uint32_t)CLK_Pin << 16U;
                    CLK_GPIO_Port->BSRR = (uint32_t)CLK_Pin;
                    //HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, 1);
                    //HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET);
                }
            }
        }
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
#ifdef RANDOMREDSTUFF
        if((cycles % 100) >= 50)
        {
            for(unsigned int i = 0; i < sizeof(framebufferR) / sizeof(framebufferR[0].w); i++)
            {
                //  framebufferR[i]=i%0x10000;
                //  framebufferG[i]=(UINT16_MAX-i)%0x10000;
                //framebufferB[i]=8-i;
                uint32_t temprand;
                HAL_RNG_GenerateRandomNumber(&hrng, &temprand);
                framebufferR[i].w = temprand;
                HAL_RNG_GenerateRandomNumber(&hrng, &temprand);
                //framebufferG[i].w = temprand;
            }
            cycles = 0;
        }
#endif //RANDOMREDSTUFF
        if(READ_BIT(interruptFlags, INTFLAG_ExtInt))
            write_difftime(tempstring2);
        write_time(tempstring);
    }
    /* USER CODE END 3 */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV8;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* RNG init function */
static void MX_RNG_Init(void)
{
    hrng.Instance = RNG;
    if(HAL_RNG_Init(&hrng) != HAL_OK)
    {
        Error_Handler();
    }
}

/* RTC init function */
static void MX_RTC_Init(void)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    RTC_AlarmTypeDef sAlarm;
    /**Initialize RTC and set the Time and Date
    */
    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 64;
    hrtc.Init.SynchPrediv = 15625;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    if(HAL_RTC_Init(&hrtc) != HAL_OK)
    {
        Error_Handler();
    }
    sTime.Hours = 0x12;
    sTime.Minutes = 0x0;
    sTime.Seconds = 0x0;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }
    sDate.WeekDay = RTC_WEEKDAY_MONDAY;
    sDate.Month = RTC_MONTH_JANUARY;
    sDate.Date = 0x1;
    sDate.Year = 0x17;
    if(HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }
    /**Enable the Alarm A
    */
    sAlarm.AlarmTime.Hours = 0x0;
    sAlarm.AlarmTime.Minutes = 0x0;
    sAlarm.AlarmTime.Seconds = 0x0;
    sAlarm.AlarmTime.SubSeconds = 0x0;
    sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
    sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
    sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
    sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    sAlarm.AlarmDateWeekDay = 0x1;
    sAlarm.Alarm = RTC_ALARM_A;
    if(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
    {
        Error_Handler();
    }
    /**Enable Calibrartion
    */
    if(HAL_RTCEx_SetCalibrationOutPut(&hrtc, RTC_CALIBOUTPUT_512HZ) != HAL_OK)
    {
        Error_Handler();
    }
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if(HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 42;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if(HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if(HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if(HAL_TIM_OC_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if(HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_TIMING;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if(HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* TIM8 init function */
static void MX_TIM8_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
    TIM_OC_InitTypeDef sConfigOC;
    htim8.Instance = TIM8;
    htim8.Init.Prescaler = 1;
    htim8.Init.CounterMode = TIM_COUNTERMODE_DOWN;
    htim8.Init.Period = 0xf000;
    htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim8.Init.RepetitionCounter = 0;
    if(HAL_TIM_Base_Init(&htim8) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if(HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if(HAL_TIM_OC_Init(&htim8) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if(HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if(HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_TIMING;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if(HAL_TIM_OC_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if(HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
     PC3   ------> I2S2_SD
     PA4   ------> I2S3_WS
     PB10   ------> I2S2_CK
     PC7   ------> I2S3_MCK
     PA9   ------> USB_OTG_FS_VBUS
     PA10   ------> USB_OTG_FS_ID
     PA11   ------> USB_OTG_FS_DM
     PA12   ------> USB_OTG_FS_DP
     PC10   ------> I2S3_CK
     PC12   ------> I2S3_SD
     PB9   ------> I2C1_SDA
*/
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOE, A_Pin | B_Pin | C_Pin | D_Pin
                      | STB_Pin, GPIO_PIN_RESET);
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin
                      | R1_Pin | G1_Pin | B1D2_Pin | R2_Pin
                      | G2_Pin | B2_Pin | CLK_Pin, GPIO_PIN_RESET);
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
    /*Configure GPIO pins : A_Pin B_Pin C_Pin D_Pin
                             STB_Pin */
    GPIO_InitStruct.Pin = A_Pin | B_Pin | C_Pin | D_Pin
                          | STB_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
    GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pin : PDM_OUT_Pin */
    GPIO_InitStruct.Pin = PDM_OUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pin : B1_Pin */
    GPIO_InitStruct.Pin = B1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pin : PA4 */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /*Configure GPIO pin : BOOT1_Pin */
    GPIO_InitStruct.Pin = BOOT1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pin : CLK_IN_Pin */
    GPIO_InitStruct.Pin = CLK_IN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin
                             R1_Pin G1_Pin B1D2_Pin R2_Pin
                             G2_Pin B2_Pin CLK_Pin */
    GPIO_InitStruct.Pin = LD4_Pin | LD3_Pin | LD5_Pin | LD6_Pin
                          | R1_Pin | G1_Pin | B1D2_Pin | R2_Pin
                          | G2_Pin | B2_Pin | CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    /*Configure GPIO pin : OE_Pin */
    GPIO_InitStruct.Pin = OE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(OE_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pins : PC7 I2S3_SCK_Pin PC12 */
    GPIO_InitStruct.Pin = GPIO_PIN_7 | I2S3_SCK_Pin | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    /*Configure GPIO pin : VBUS_FS_Pin */
    GPIO_InitStruct.Pin = VBUS_FS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
    GPIO_InitStruct.Pin = OTG_FS_ID_Pin | OTG_FS_DM_Pin | OTG_FS_DP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /*Configure GPIO pin : Audio_SDA_Pin */
    GPIO_InitStruct.Pin = Audio_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(Audio_SDA_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pin : MEMS_INT2_Pin */
    GPIO_InitStruct.Pin = MEMS_INT2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while(1)
    {
        HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, GPIO_PIN_SET);
        __SYSCFG_FORCE_RESET();
    }
    /* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
      ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
