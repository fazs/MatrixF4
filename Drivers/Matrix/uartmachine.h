/*
 * Sponsored License - for use in support of a program or activity
 * sponsored by MathWorks.  Not for government, commercial or other
 * non-sponsored organizational use.
 *
 * File: matrix.h
 *
 * Code generated for Simulink model :matrix.
 *
 * Model version      : 1.38
 * Simulink Coder version    : 8.14 (R2018a) 06-Feb-2018
 * TLC version       : 8.14 (Feb 22 2018)
 * C/C++ source code generated on  : Mon May 21 12:20:37 2018
 *
 * Target selection: stm32.tlc
 * Embedded hardware selection: STMicroelectronics->STM32 32-bit Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. ROM efficiency
 *    4. Debugging
 *    5. MISRA C:2012 guidelines
 * Validation result: Not run
 *
 *
 *
 * ******************************************************************************
 * * attention
 * *
 * * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * *
 * ******************************************************************************
 */

#ifndef RTW_HEADER_matrix_h_
#define RTW_HEADER_matrix_h_
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

//#include "matrix_External_Functions.h"
//#include "matrix_USART.h"
#ifndef matrix_COMMON_INCLUDES_
# define matrix_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* matrix_COMMON_INCLUDES_ */

#include "matrix_types.h"
//#include "slMsgSvc.h"


/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (default storage) */
typedef struct
{
    uint8_T colormode;                   /* '<S11>/uartparsemachine' */
    uint8_T xpos;                        /* '<S11>/uartparsemachine' */
    uint8_T data[30];                    /* '<S11>/uartparsemachine' */
    uint8_T ypos;                        /* '<S11>/uartparsemachine' */
    uint8_T data_length;                 /* '<S11>/uartparsemachine' */    
} B_matrix;

/* Block states (default storage) for system '<Root>' */
typedef struct
{
//    slMsgWrapper slMsgMgr_Msg[14];       /* synthesized block */
//    slMsgUint8 slMsgMgr_MsgData[14];     /* synthesized block */
//    slMsgManager slMsgMgr_Impl;          /* synthesized block */
//    slMsgQueue slMsgMgr_MsgQueues;       /* synthesized block */
//    slMsgMemPool slMsgMgr_MemPools[2];   /* synthesized block */
    void* M_msgInterface;                /* '<S11>/uartparsemachine' */
    void* M_msgHandle;                   /* '<S11>/uartparsemachine' */
    void* M_msgDataPtr;                  /* '<S11>/uartparsemachine' */
    void* message_msgInterface;          /* '<S11>/dummyConvertingMachine' */
//    int_T Queue_msgQid;                  /* synthesized block */
//    uint16_T ram_DSTATE[128];            /* '<S8>/ram' */
//    uint16_T ram_DSTATE_m[128];          /* '<S9>/ram' */
//    uint16_T ram_data_DSTATE;            /* '<S9>/ram_data' */
//    uint16_T ram_data_DSTATE_o;          /* '<S8>/ram_data' */
    uint8_T is_c1_matrix;                /* '<S11>/uartparsemachine' */
    uint8_T crc;                         /* '<S11>/uartparsemachine' */
    uint8_T receive_position;            /* '<S11>/uartparsemachine' */
    uint8_T crc_reference;               /* '<S11>/uartparsemachine' */
    uint8_T M_msgData;                   /* '<S11>/uartparsemachine' */
} DW_matrix;

/* Real-time Model Data Structure */
struct tag_RTM_matrix
{
    const char_T* errorStatus;

    /*
     * Timing:
     * The following substructure contains information regarding
     * the timing information for the model.
     */
    struct
    {
        struct
        {
            uint32_T TID[2];
        } TaskCounters;
    } Timing;
};

/* Block signals (default storage) */
extern B_matrix matrix_B;

/* Block states (default storage) */
extern DW_matrix matrix_DW;

/* Model entry point functions */
extern void matrix_initialize(void);
extern void matrix_step(void);

/* Real-time Model object */
extern RT_MODEL_matrix* const matrix_M;


/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'matrix'
 * '<S1>'   : 'matrix/Extract Bits'
 * '<S2>'   : 'matrix/Extract Bits1'
 * '<S3>'   : 'matrix/Extract Bits2'
 * '<S4>'   : 'matrix/Extract Bits3'
 * '<S5>'   : 'matrix/Model Info'
 * '<S6>'   : 'matrix/MuxtoBit'
 * '<S7>'   : 'matrix/MuxtoBit1'
 * '<S8>'   : 'matrix/RamGreen'
 * '<S9>'   : 'matrix/RamRed'
 * '<S10>'  : 'matrix/matrixmachine'
 * '<S11>'  : 'matrix/uartParse'
 * '<S12>'  : 'matrix/MuxtoBit/Extract Bits10'
 * '<S13>'  : 'matrix/MuxtoBit/Extract Bits11'
 * '<S14>'  : 'matrix/MuxtoBit/Extract Bits12'
 * '<S15>'  : 'matrix/MuxtoBit/Extract Bits13'
 * '<S16>'  : 'matrix/MuxtoBit/Extract Bits14'
 * '<S17>'  : 'matrix/MuxtoBit/Extract Bits15'
 * '<S18>'  : 'matrix/MuxtoBit/Extract Bits16'
 * '<S19>'  : 'matrix/MuxtoBit/Extract Bits17'
 * '<S20>'  : 'matrix/MuxtoBit/Extract Bits18'
 * '<S21>'  : 'matrix/MuxtoBit/Extract Bits19'
 * '<S22>'  : 'matrix/MuxtoBit/Extract Bits4'
 * '<S23>'  : 'matrix/MuxtoBit/Extract Bits5'
 * '<S24>'  : 'matrix/MuxtoBit/Extract Bits6'
 * '<S25>'  : 'matrix/MuxtoBit/Extract Bits7'
 * '<S26>'  : 'matrix/MuxtoBit/Extract Bits8'
 * '<S27>'  : 'matrix/MuxtoBit/Extract Bits9'
 * '<S28>'  : 'matrix/MuxtoBit1/Extract Bits10'
 * '<S29>'  : 'matrix/MuxtoBit1/Extract Bits11'
 * '<S30>'  : 'matrix/MuxtoBit1/Extract Bits12'
 * '<S31>'  : 'matrix/MuxtoBit1/Extract Bits13'
 * '<S32>'  : 'matrix/MuxtoBit1/Extract Bits14'
 * '<S33>'  : 'matrix/MuxtoBit1/Extract Bits15'
 * '<S34>'  : 'matrix/MuxtoBit1/Extract Bits16'
 * '<S35>'  : 'matrix/MuxtoBit1/Extract Bits17'
 * '<S36>'  : 'matrix/MuxtoBit1/Extract Bits18'
 * '<S37>'  : 'matrix/MuxtoBit1/Extract Bits19'
 * '<S38>'  : 'matrix/MuxtoBit1/Extract Bits4'
 * '<S39>'  : 'matrix/MuxtoBit1/Extract Bits5'
 * '<S40>'  : 'matrix/MuxtoBit1/Extract Bits6'
 * '<S41>'  : 'matrix/MuxtoBit1/Extract Bits7'
 * '<S42>'  : 'matrix/MuxtoBit1/Extract Bits8'
 * '<S43>'  : 'matrix/MuxtoBit1/Extract Bits9'
 * '<S44>'  : 'matrix/uartParse/dummyConvertingMachine'
 * '<S45>'  : 'matrix/uartParse/uartparsemachine'
 */

/*-
 * Requirements for '<Root>': matrix
 */
#endif                                 /* RTW_HEADER_matrix_h_ */

/* File trailer for Real-Time Workshop generated code.
 *
 * [EOF] matrix.h
 */
uint8_t nextchar(char tmpchar);
    
