#include "uartmachine.h"
#include "matrix_private.h"
#include "framebuffer.h"
#include <string.h>


/* Named constants for Chart: '<S11>/uartparsemachine' */
#define matrix_IN_Start                ((uint8_T)1U)
#define matrix_IN_d_P                  ((uint8_T)2U)
#define matrix_IN_d_PF                 ((uint8_T)3U)
#define matrix_IN_d_PFZ                ((uint8_T)4U)
#define matrix_IN_d_PFZS               ((uint8_T)5U)
#define matrix_IN_d_PFZSD              ((uint8_T)6U)
#define matrix_IN_d_PFZSDS             ((uint8_T)7U)
#define matrix_IN_d_PFZSDSP            ((uint8_T)8U)
#define matrix_IN_d_PFZSDSP_CRC        ((uint8_T)9U)
#define matrix_IN_d_PFZSDSP_CRC1       ((uint8_T)10U)
#define matrix_IN_d_PFZSDSP_Length     ((uint8_T)11U)
#define matrix_IN_d_PFZSDSP_LengthAdd  ((uint8_T)12U)
#define matrix_IN_d_PFZSDSP_Lengthfin  ((uint8_T)13U)
#define matrix_IN_d_PFZSDSP_Xpos       ((uint8_T)14U)
#define matrix_IN_d_PFZSDSP_XposAdd    ((uint8_T)15U)
#define matrix_IN_d_PFZSDSP_Xposfin    ((uint8_T)16U)
#define matrix_IN_d_PFZSDSP_Ypos       ((uint8_T)17U)
#define matrix_IN_d_PFZSDSP_YposAdd    ((uint8_T)18U)
#define matrix_IN_d_PFZSDSP_Yposfin    ((uint8_T)19U)
#define matrix_IN_d_PFZSDSP_color      ((uint8_T)20U)
#define matrix_IN_d_PFZSDSP_colorfin   ((uint8_T)21U)
#define matrix_IN_d_PFZSDSP_data_receive ((uint8_T)22U)
#define matrix_IN_d_PFZSDSP_data_receive_Start ((uint8_T)23U)
#define matrix_IN_d_PFZSDSP_idfin      ((uint8_T)24U)
#define matrix_IN_d_start              ((uint8_T)25U)


///* Block signals (default storage) */
//static B_matrix matrix_B;

///* Block states (default storage) */
//static DW_matrix matrix_DW;

/* Block signals (default storage) */
//extern B_matrix matrix_B;

/* Block states (default storage) */
//extern DW_matrix matrix_DW;


uint8_t nextchar(char tmpchar)
{
    FB_COLOR_t tmpcolor = YELLOW;

    /* During: uartParse/uartparsemachine */
    switch(matrix_DW.is_c1_matrix)
    {
    case matrix_IN_Start:
        matrix_DW.crc = 0U;
        if(tmpchar == '$')
        {
            /* Transition: '<S45>:4' */
            matrix_DW.is_c1_matrix = matrix_IN_d_start;
        }
        break;
    case matrix_IN_d_P:
        //matrix_d_P(&M_isValid);
        /* During 'd_P': '<S45>:8' */
        if(tmpchar == 'F')
        {
            /* Transition: '<S45>:15' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PF;
            /* Entry 'd_PF': '<S45>:14' */
            matrix_DW.crc  = matrix_DW.crc ^ tmpchar;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PF:
        /* During 'd_PF': '<S45>:14' */
        if(tmpchar == 'Z')
        {
            /* Transition: '<S45>:18' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZ;
            /* Entry 'd_PFZ': '<S45>:17' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
        }
        else
        {
            /* Transition: '<S45>:42' */
            /* Transition: '<S45>:106' */
            /* Transition: '<S45>:107' */
            /* Transition: '<S45>:108' */
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZ:
        /* During 'd_PFZ': '<S45>:17' */
        if(tmpchar == 'S')
        {
            /* Transition: '<S45>:21' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZS;
            /* Entry 'd_PFZS': '<S45>:20' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZS:
        /* During 'd_PFZS': '<S45>:20' */
        if(tmpchar == 'D')
        {
            /* Transition: '<S45>:24' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSD;
            /* Entry 'd_PFZSD': '<S45>:23' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZSD:
        /* During 'd_PFZSD': '<S45>:23' */
        if(tmpchar == 'S')
        {
            /* Transition: '<S45>:27' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDS;
            /* Entry 'd_PFZSDS': '<S45>:26' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        break;
    case matrix_IN_d_PFZSDS:
        /* During 'd_PFZSDS': '<S45>:26' */
        if(tmpchar == 'P')
        {
            /* Transition: '<S45>:30' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP;
            /* Entry 'd_PFZSDSP': '<S45>:29' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZSDSP:
        /* During 'd_PFZSDSP': '<S45>:29' */
        if(tmpchar == ',')
        {
            /* Transition: '<S45>:66' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_idfin;
            /* Entry 'd_PFZSDSP_idfin': '<S45>:65' */
            /* crc ^= M.data; */
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZSDSP_CRC:
        /* During 'd_PFZSDSP_CRC': '<S45>:160' */
        if((tmpchar >= '0') && (tmpchar <= '9'))
        {
            /* Transition: '<S45>:175' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_CRC1;
            /* Entry 'd_PFZSDSP_CRC1': '<S45>:174' */
            matrix_DW.crc = matrix_DW.crc_reference + tmpchar;
            matrix_DW.crc_reference = (uint8_T)matrix_DW.crc;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZSDSP_CRC1:
        //if(matrix_DW.crc == matrix_DW.crc_reference)
        if(1)
        {
            /* Transition: '<S45>:181' */
            printf("parsed OK\n");
			
            matrix_B.data[matrix_B.data_length]= NULL;
            
            switch (matrix_B.colormode){
                case 'O':
                    tmpcolor = OFF;
                    break;
                case 'R':
                    tmpcolor = RED;
                    break;
                case 'G':
                    tmpcolor = GREEN;
                    break;
                case 'Y':
                    tmpcolor = YELLOW;
                    break;
                case 'C':
                    tmpcolor = CLEAR;
                    break;
            }
            
            if(matrix_B.ypos > 1) matrix_B.ypos = 1;
            
			Framebuffer_Putch((char *)matrix_B.data, matrix_B.xpos, tmpcolor, matrix_B.ypos);
        }
        matrix_DW.is_c1_matrix = matrix_IN_Start;
        /* Entry 'Start': '<S45>:1' */
        matrix_DW.crc = 0U;
        break;
    case matrix_IN_d_PFZSDSP_Length:
        /* During 'd_PFZSDSP_Length': '<S45>:135' */
        if((tmpchar >= '0') && (tmpchar <= '9'))
        {
            /* Transition: '<S45>:136' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_LengthAdd;
            /* Entry 'd_PFZSDSP_LengthAdd': '<S45>:146' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            matrix_B.data_length = (matrix_B.data_length * 10) + (tmpchar - '0');
        }
        else if(tmpchar == ',')
        {
            /* Transition: '<S45>:143' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Lengthfin;
            /* Entry 'd_PFZSDSP_Lengthfin': '<S45>:141' */
            /* crc ^= M.data; */
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZSDSP_LengthAdd:
        /* During 'd_PFZSDSP_LengthAdd': '<S45>:146' */
        if((tmpchar != ','))
        {
            /* Transition: '<S45>:147' */
            /* Transition: '<S45>:145' */
            /* Transition: '<S45>:134' */
            /* Transition: '<S45>:87' */
            /* Transition: '<S45>:83' */
            /* Transition: '<S45>:93' */
            /* Transition: '<S45>:98' */
            /* Transition: '<S45>:100' */
            /* Transition: '<S45>:102' */
            /* Transition: '<S45>:104' */
            /* Transition: '<S45>:105' */
            /* Transition: '<S45>:106' */
            /* Transition: '<S45>:107' */
            /* Transition: '<S45>:108' */
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        else if(tmpchar == ',')
        {
            /* Transition: '<S45>:142' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Lengthfin;
            /* Entry 'd_PFZSDSP_Lengthfin': '<S45>:141' */
            /* crc ^= M.data; */
        }
        else
        {
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            matrix_B.data_length = (matrix_B.data_length * 10) + tmpchar;
        }
        break;
    case matrix_IN_d_PFZSDSP_Lengthfin:
        /* During 'd_PFZSDSP_Lengthfin': '<S45>:141' */
        if(((tmpchar >= '0') && (tmpchar <= '9')))
        {
            /* Transition: '<S45>:137' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Xpos;
            /* Entry 'd_PFZSDSP_Xpos': '<S45>:75' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            matrix_DW.crc = (uint8_T)matrix_DW.crc;
            matrix_B.xpos = tmpchar - '0';
        }
        break;
    case matrix_IN_d_PFZSDSP_Xpos:
        /* During 'd_PFZSDSP_Xpos': '<S45>:75' */
        if(((tmpchar >= '0') && (tmpchar <= '9')))
        {
            /* Transition: '<S45>:109' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_XposAdd;
            /* Entry 'd_PFZSDSP_XposAdd': '<S45>:110' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            
            matrix_B.xpos = (matrix_B.xpos * 10) + tmpchar;
        }
        else if(tmpchar == ',')
        {
            /* Transition: '<S45>:77' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Xposfin;
            /* Entry 'd_PFZSDSP_Xposfin': '<S45>:76' */
            /* crc ^= M.data; */
        }
        else
        {
            /* Transition: '<S45>:90' */
            /* Transition: '<S45>:149' */
            /* Transition: '<S45>:145' */
            /* Transition: '<S45>:134' */
            /* Transition: '<S45>:87' */
            /* Transition: '<S45>:83' */
            /* Transition: '<S45>:93' */
            /* Transition: '<S45>:98' */
            /* Transition: '<S45>:100' */
            /* Transition: '<S45>:102' */
            /* Transition: '<S45>:104' */
            /* Transition: '<S45>:105' */
            /* Transition: '<S45>:106' */
            /* Transition: '<S45>:107' */
            /* Transition: '<S45>:108' */
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        break;
    case matrix_IN_d_PFZSDSP_XposAdd:
        /* During 'd_PFZSDSP_XposAdd': '<S45>:110' */
        if(tmpchar != ',')
        {
            /* Transition: '<S45>:117' */
            /* Transition: '<S45>:118' */
            /* Transition: '<S45>:149' */
            /* Transition: '<S45>:145' */
            /* Transition: '<S45>:134' */
            /* Transition: '<S45>:87' */
            /* Transition: '<S45>:83' */
            /* Transition: '<S45>:93' */
            /* Transition: '<S45>:98' */
            /* Transition: '<S45>:100' */
            /* Transition: '<S45>:102' */
            /* Transition: '<S45>:104' */
            /* Transition: '<S45>:105' */
            /* Transition: '<S45>:106' */
            /* Transition: '<S45>:107' */
            /* Transition: '<S45>:108' */
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        else if(tmpchar == ',')
        {
            /* Transition: '<S45>:111' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Xposfin;
            /* Entry 'd_PFZSDSP_Xposfin': '<S45>:76' */
            /* crc ^= M.data; */
        }
        else
        {
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            matrix_B.xpos = (matrix_B.xpos*10) + tmpchar;
        }
        break;
    case matrix_IN_d_PFZSDSP_Xposfin:
        /* During 'd_PFZSDSP_Xposfin': '<S45>:76' */
        if((tmpchar >='0') && (tmpchar <= '9'))
        {
            /* Transition: '<S45>:119' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Ypos;
            /* Entry 'd_PFZSDSP_Ypos': '<S45>:124' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            matrix_DW.crc = (uint8_T)matrix_DW.crc;
            matrix_B.ypos = tmpchar - '0';
        }
        break;
    case matrix_IN_d_PFZSDSP_Ypos:
        /* During 'd_PFZSDSP_Ypos': '<S45>:124' */
        if((tmpchar >= '0') && (tmpchar <= '9'))
        {
            /* Entry 'd_PFZSDSP_YposAdd': '<S45>:126' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            // matrix_B.ypos = matrix_B.ypos * 10;
            matrix_B.ypos = matrix_B.ypos + tmpchar;
            /* Transition: '<S45>:125' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_YposAdd;
        }
        else if(tmpchar == ',')
        {
            /* Transition: '<S45>:123' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Yposfin;
            /* Entry 'd_PFZSDSP_Yposfin': '<S45>:121' */
            /* crc ^= M.data; */
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        break;
    case matrix_IN_d_PFZSDSP_YposAdd:
        /* During 'd_PFZSDSP_YposAdd': '<S45>:126' */
        if(tmpchar == ',')
        {
            /* Transition: '<S45>:122' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Yposfin;
            /* Entry 'd_PFZSDSP_Yposfin': '<S45>:121' */
            /* crc ^= M.data; */
        }
        else{            
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_PFZSDSP_Yposfin:
        /* During 'd_PFZSDSP_Yposfin': '<S45>:121' */
        /* Transition: '<S45>:155' */
        matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_data_receive_Start;
        /* Entry 'd_PFZSDSP_data_receive_Start': '<S45>:154' */
        /* crc ^= M.data; */
        matrix_B.data[0] = tmpchar;
        matrix_DW.receive_position = matrix_B.data_length - 1 ;
        break;
    case matrix_IN_d_PFZSDSP_color:
        /* During 'd_PFZSDSP_color': '<S45>:70' */
        if(tmpchar == ',')
        {
            /* Transition: '<S45>:73' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_colorfin;
            /* Entry 'd_PFZSDSP_colorfin': '<S45>:72' */
            /* crc ^= M.data; */
        }
        else
        {
            /* Transition: '<S45>:79' */
            /* Transition: '<S45>:83' */
            /* Transition: '<S45>:93' */
            /* Transition: '<S45>:98' */
            /* Transition: '<S45>:100' */
            /* Transition: '<S45>:102' */
            /* Transition: '<S45>:104' */
            /* Transition: '<S45>:105' */
            /* Transition: '<S45>:106' */
            /* Transition: '<S45>:107' */
            /* Transition: '<S45>:108' */
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        break;
    case matrix_IN_d_PFZSDSP_colorfin:
        if((tmpchar >= '0') && (tmpchar <= '9'))
        {
            /* Transition: '<S45>:150' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_Length;
            /* Entry 'd_PFZSDSP_Length': '<S45>:135' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            matrix_B.data_length = tmpchar - '0';
        }
        else
        {
            /* Transition: '<S45>:86' */
            /* Transition: '<S45>:87' */
            /* Transition: '<S45>:83' */
            /* Transition: '<S45>:93' */
            /* Transition: '<S45>:98' */
            /* Transition: '<S45>:100' */
            /* Transition: '<S45>:102' */
            /* Transition: '<S45>:104' */
            /* Transition: '<S45>:105' */
            /* Transition: '<S45>:106' */
            /* Transition: '<S45>:107' */
            /* Transition: '<S45>:108' */
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        break;
    case matrix_IN_d_PFZSDSP_data_receive:
        /* During 'd_PFZSDSP_data_receive': '<S45>:151' */
        if(matrix_DW.receive_position > 0)
        {
            /* Transition: '<S45>:157' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_data_receive;
            /* Entry 'd_PFZSDSP_data_receive': '<S45>:151' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            
            matrix_B.data[ matrix_B.data_length - matrix_DW.receive_position] = tmpchar;
            matrix_DW.receive_position = matrix_DW.receive_position - 1;
        }
        else if((matrix_DW.receive_position <= 0) && (tmpchar == '*'))
        {
            /* Transition: '<S45>:161' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_CRC;
            /* Entry 'd_PFZSDSP_CRC': '<S45>:160' */
            matrix_DW.crc_reference = tmpchar;
        }
        else 
        {
            /* Transition: '<S45>:168' */
            /* Transition: '<S45>:170' */
            /* Transition: '<S45>:131' */
            /* Transition: '<S45>:129' */
            /* Transition: '<S45>:118' */
            /* Transition: '<S45>:149' */
            /* Transition: '<S45>:145' */
            /* Transition: '<S45>:134' */
            /* Transition: '<S45>:87' */
            /* Transition: '<S45>:83' */
            /* Transition: '<S45>:93' */
            /* Transition: '<S45>:98' */
            /* Transition: '<S45>:100' */
            /* Transition: '<S45>:102' */
            /* Transition: '<S45>:104' */
            /* Transition: '<S45>:105' */
            /* Transition: '<S45>:106' */
            /* Transition: '<S45>:107' */
            /* Transition: '<S45>:108' */
            matrix_DW.is_c1_matrix = matrix_IN_Start;
            /* Entry 'error': '<S45>:60' */
            /* error_evt; */
        }
        break;
    case matrix_IN_d_PFZSDSP_data_receive_Start:
        /* During 'd_PFZSDSP_data_receive_Start': '<S45>:154' */
    
        /* Transition: '<S45>:156' */
        matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_data_receive;
        /* Entry 'd_PFZSDSP_data_receive': '<S45>:151' */
        matrix_DW.crc = matrix_DW.crc ^ tmpchar;
        matrix_B.data[matrix_B.data_length - matrix_DW.receive_position] = tmpchar;
        matrix_DW.receive_position = matrix_DW.receive_position - 1;
   
   
        break;
    case matrix_IN_d_PFZSDSP_idfin:
        /* During 'd_PFZSDSP_idfin': '<S45>:65' */
        if((tmpchar == 'O') || (tmpchar == 'R') || (tmpchar == 'G') || (tmpchar == 'C')|| (tmpchar == 'Y'))
        {
            /* Transition: '<S45>:71' */
            matrix_DW.is_c1_matrix = matrix_IN_d_PFZSDSP_color;
            /* Entry 'd_PFZSDSP_color': '<S45>:70' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
            matrix_B.colormode = tmpchar;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    case matrix_IN_d_start:
        /* During 'd_start': '<S45>:5' */
        if(tmpchar == 'P')
        {
            /* Transition: '<S45>:9' */
            matrix_DW.is_c1_matrix = matrix_IN_d_P;
            /* Entry 'd_P': '<S45>:8' */
            matrix_DW.crc = matrix_DW.crc ^ tmpchar;
        }
        else
        {
            matrix_DW.is_c1_matrix = matrix_IN_Start;
        }
        break;
    default:
        /* During 'error': '<S45>:60' */
        /* Transition: '<S45>:62' */
        matrix_DW.is_c1_matrix = matrix_IN_Start;
        /* Entry 'Start': '<S45>:1' */
        break;
    }
    return matrix_DW.is_c1_matrix;
}
