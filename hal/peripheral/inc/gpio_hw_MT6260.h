#ifndef _GPIO_HW_MT6260_H 
#define _GPIO_HW_MT6260_H 
//#include "drv_features_gpio.h" 
#include "reg_base.h" 


#if !defined(DRV_GPIO_OFF)



/*D die macro ****************************************************************************************/

#define   DDIE_GPIO_PULLSEL_UP_IS_1
/*if no define 
ADIE_GPIO_PULLSEL_UP_IS_1 
or DDIE_GPIO_PUPD_UP_IS_1 
or DDIE_GPIO_PULLSEL_UP_IS_1 
means: down is 1*/

#define    GPIO_MODE0                               (GPIO_base+0x0C00)
#define    GPIO_MODE0_SET                           (GPIO_base+0x0C04)
#define    GPIO_MODE0_CLR                           (GPIO_base+0x0C08)
#define    GPIO_MODE_OFFSET_TO_NEXT                 0x10
#define    GPIO_MODE_REG_MAX_NUM                    9
#define    GPIO_MODE_REG_BITS                       32
#define    GPIO_MODE_ONE_CONTROL_BITS               4
#define    GPIO_MODE_ONE_REG_CONTROL_NUM            8
#define    GPIO_MODE_REG0    GPIO_HWORD_REG_VAL(MODE, 0,  1,  2,  3,  4,  5,  6,  7, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG1    GPIO_HWORD_REG_VAL(MODE,  8,  9,  10,  11,  12,  13,  14,  15, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG2    GPIO_HWORD_REG_VAL(MODE,  16,  17,  18,  19,  20,  21,  22,  23, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG3    GPIO_HWORD_REG_VAL(MODE,  24,  25,  26,  27,  28,  29,  30,  31, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG4    GPIO_HWORD_REG_VAL(MODE, 32,  33,  34,  35,  36,  37,  38,  39, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG5    GPIO_HWORD_REG_VAL(MODE,  40,  41,  42,  43,  44,  45,  46,  47, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG6    GPIO_HWORD_REG_VAL(MODE,  48, 49,  50,  51,  52,  53,  54,  55, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG7    GPIO_HWORD_REG_VAL(MODE,  56,  57,  58,  59,  60,  61,  62,  63, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG8    GPIO_HWORD_REG_VAL(MODE, 64,  65,  66,  67,  68,  69,  70,  71, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_REG9    GPIO_HWORD_REG_VAL(MODE,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_MODE_ALL_VALUE     GPIO_MODE_REG0,GPIO_MODE_REG1,GPIO_MODE_REG2,GPIO_MODE_REG3,GPIO_MODE_REG4,GPIO_MODE_REG5,GPIO_MODE_REG6,GPIO_MODE_REG7,GPIO_MODE_REG8,GPIO_MODE_REG9   

#define    GPIO_DIR0                                (GPIO_base+0x0000)
#define    GPIO_DIR0_SET                            (GPIO_base+0x0004)
#define    GPIO_DIR0_CLR                            (GPIO_base+0x0008)
#define    GPIO_DIR_OFFSET_TO_NEXT                  0x10
#define    GPIO_DIR_REG_MAX_NUM                     4
#define    GPIO_DIR_REG_BITS                        16
#define    GPIO_DIR_ONE_CONTROL_BITS                1
#define    GPIO_DIR_ONE_REG_CONTROL_NUM             16
#define    GPIO_DIR_REG0    GPIO_HWORD_REG_VAL(DIR, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIR_REG1    GPIO_HWORD_REG_VAL(DIR,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIR_REG2    GPIO_HWORD_REG_VAL(DIR, 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIR_REG3    GPIO_HWORD_REG_VAL(DIR,  48, 49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIR_REG4    GPIO_HWORD_REG_VAL(DIR, 64,  65,  66,  67,  68,  69,  70,  71,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIR_ALL_VALUE     GPIO_DIR_REG0,GPIO_DIR_REG1,GPIO_DIR_REG2,GPIO_DIR_REG3,GPIO_DIR_REG4   

#define    GPIO_PULL0                               (GPIO_base+0x0100)
#define    GPIO_PULL0_SET                           (GPIO_base+0x0104)
#define    GPIO_PULL0_CLR                           (GPIO_base+0x0108)
#define    GPIO_PULL_OFFSET_TO_NEXT                 0x10
#define    GPIO_PULL_REG_MAX_NUM                    4
#define    GPIO_PULL_REG_BITS                       16
#define    GPIO_PULL_ONE_CONTROL_BITS               1
#define    GPIO_PULL_ONE_REG_CONTROL_NUM            16
#define    GPIO_PULL_REG0    GPIO_HWORD_REG_VAL(PULL, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_REG1    GPIO_HWORD_REG_VAL(PULL,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_REG2    GPIO_HWORD_REG_VAL(PULL, 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_REG3    GPIO_HWORD_REG_VAL(PULL,  48, 49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_REG4    GPIO_HWORD_REG_VAL(PULL, 64,  65,  66,  67,  68,  69,  70,  71,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_ALL_VALUE     GPIO_PULL_REG0,GPIO_PULL_REG1,GPIO_PULL_REG2,GPIO_PULL_REG3,GPIO_PULL_REG4   

#define    GPIO_DINV0                               (GPIO_base+0x0200)
#define    GPIO_DINV_OFFSET_TO_NEXT                 0x10
#define    GPIO_INV_REG_MAX_NUM                     4
#define    GPIO_INV_REG_BITS                        16
#define    GPIO_INV_ONE_CONTROL_BITS                1
#define    GPIO_INV_ONE_REG_CONTROL_NUM             16
#define    GPIO_INV_REG0    GPIO_HWORD_REG_VAL(INV, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_INV_REG1    GPIO_HWORD_REG_VAL(INV,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_INV_REG2    GPIO_HWORD_REG_VAL(INV, 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_INV_REG3    GPIO_HWORD_REG_VAL(INV,  48, 49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_INV_REG4    GPIO_HWORD_REG_VAL(INV, 64,  65,  66,  67,  68,  69,  70,  71,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_INV_ALL_VALUE     GPIO_INV_REG0,GPIO_INV_REG1,GPIO_INV_REG2,GPIO_INV_REG3,GPIO_INV_REG4   

#define    GPIO_OUTPUT_LEVEL0                       (GPIO_base+0x0300)
#define    GPIO_OUTPUT_LEVEL0_SET                   (GPIO_base+0x0304)
#define    GPIO_OUTPUT_LEVEL0_CLR                   (GPIO_base+0x0308)
#define    GPIO_OUTPUT_LEVEL_OFFSET_TO_NEXT         0x10
#define    GPIO_OUTPUT_LEVEL_REG_MAX_NUM            4
#define    GPIO_OUTPUT_LEVEL_REG_BITS               16
#define    GPIO_OUTPUT_LEVEL_ONE_CONTROL_BITS       1
#define    GPIO_OUTPUT_LEVEL_ONE_REG_CONTROL_NUM    16
#define    GPIO_OUTPUT_LEVEL_REG0    GPIO_HWORD_REG_VAL(OUTPUT_LEVEL, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_OUTPUT_LEVEL_REG1    GPIO_HWORD_REG_VAL(OUTPUT_LEVEL,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_OUTPUT_LEVEL_REG2    GPIO_HWORD_REG_VAL(OUTPUT_LEVEL, 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_OUTPUT_LEVEL_REG3    GPIO_HWORD_REG_VAL(OUTPUT_LEVEL,  48, 49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_OUTPUT_LEVEL_REG4    GPIO_HWORD_REG_VAL(OUTPUT_LEVEL, 64,  65,  66,  67,  68,  69,  70,  71,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_OUTPUT_LEVEL_ALL_VALUE     GPIO_OUTPUT_LEVEL_REG0,GPIO_OUTPUT_LEVEL_REG1,GPIO_OUTPUT_LEVEL_REG2,GPIO_OUTPUT_LEVEL_REG3,GPIO_OUTPUT_LEVEL_REG4   

#define    GPIO_DIN0                                (GPIO_base+0x0400)
#define    GPIO_DIN_OFFSET_TO_NEXT                  0x10
#define    GPIO_DIN_REG_MAX_NUM                     4
#define    GPIO_DIN_REG_BITS                        16
#define    GPIO_DIN_ONE_CONTROL_BITS                1
#define    GPIO_DIN_ONE_REG_CONTROL_NUM             16
#define    GPIO_DIN_REG0    GPIO_HWORD_REG_VAL(DIN, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIN_REG1    GPIO_HWORD_REG_VAL(DIN,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIN_REG2    GPIO_HWORD_REG_VAL(DIN, 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIN_REG3    GPIO_HWORD_REG_VAL(DIN,  48, 49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIN_REG4    GPIO_HWORD_REG_VAL(DIN, 64,  65,  66,  67,  68,  69,  70,  71,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIN_ALL_VALUE     GPIO_DIN_REG0,GPIO_DIN_REG1,GPIO_DIN_REG2,GPIO_DIN_REG3,GPIO_DIN_REG4   

#define    GPIO_PULLSEL0                            (GPIO_base+0x0500)
#define    GPIO_PULLSEL0_SET                        (GPIO_base+0x0504)
#define    GPIO_PULLSEL0_CLR                        (GPIO_base+0x0508)
#define    GPIO_PULL_SEL_OFFSET_TO_NEXT             0x10
#define    GPIO_PULL_SEL_REG_MAX_NUM                4
#define    GPIO_PULL_SEL_REG_BITS                   16
#define    GPIO_PULL_SEL_ONE_CONTROL_BITS           1
#define    GPIO_PULL_SEL_ONE_REG_CONTROL_NUM        16
#define    GPIO_PULL_SEL_REG0    GPIO_HWORD_REG_VAL(PULL_SEL, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_SEL_REG1    GPIO_HWORD_REG_VAL(PULL_SEL,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_SEL_REG2    GPIO_HWORD_REG_VAL(PULL_SEL, 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_SEL_REG3    GPIO_HWORD_REG_VAL(PULL_SEL,  48, 49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_SEL_REG4    GPIO_HWORD_REG_VAL(PULL_SEL, 64,  65,  66,  67,  68,  69,  70,  71,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PULL_SEL_ALL_VALUE     GPIO_PULL_SEL_REG0,GPIO_PULL_SEL_REG1,GPIO_PULL_SEL_REG2,GPIO_PULL_SEL_REG3,GPIO_PULL_SEL_REG4   

#define    GPIO_DRV0                                (GPIO_base+0x0800)
#define    GPIO_DRV_OFFSET_TO_NEXT                  0x10
#define    GPIO_DRV_REG_MAX_NUM                     3
#define    GPIO_IES0                                (GPIO_base+0x0900)
#define    GPIO_IES_OFFSET_TO_NEXT                  0x10
#define    GPIO_DRV_IES_MAX_NUM                     3
#define    GPIO_PUPD0                               (GPIO_base+0x0a00)
#define    GPIO_PUPD0_SET                           (GPIO_base+0x0a04)
#define    GPIO_PUPD0_CLR                           (GPIO_base+0x0a08)
#define    GPIO_PUPD_OFFSET_TO_NEXT                 0x10
#define    GPIO_PUPD_REG_MAX_NUM                    2
#define    GPIO_PUPD_REG_BITS                       16
#define    GPIO_PUPD_ONE_CONTROL_BITS               1
#define    GPIO_PUPD_ONE_REG_CONTROL_NUM            16
#define    GPIO_PUPD_REG0    GPIO_HWORD_REG_VAL(PUPD, NULL, NULL, NULL, NULL, NULL, NULL, 25, 27, 26, 9, 12, 11, 5, 15, 7, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PUPD_REG1    GPIO_HWORD_REG_VAL(PUPD, 3, 4, 13, 10, 14, 8, 22, 40, 37, 39, 38, 35, 36, 66, 72, 67, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PUPD_REG2    GPIO_HWORD_REG_VAL(PUPD, 71, 69, 70, 68, 50, 52, 51, 53, 54, 55, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_PUPD_ALL_VALUE     GPIO_PUPD_REG0,GPIO_PUPD_REG1,GPIO_PUPD_REG2   

#define    GPIO_R0_ADDR0                            (GPIO_base+0x0b00)
#define    GPIO_R0_SET_REG0                         (GPIO_base+0x0b04)
#define    GPIO_R0_CLR_REG0                         (GPIO_base+0x0b08)
#define    GPIO_R0_OFFSET_TO_NEXT                   0x10
#define    GPIO_R0_REG_MAX_NUM                      2
#define    GPIO_R0_REG_BITS                         16
#define    GPIO_R0_ONE_CONTROL_BITS                 1
#define    GPIO_R0_ONE_REG_CONTROL_NUM              16
#define    GPIO_R0_REG0    GPIO_HWORD_REG_VAL(R0, NULL, NULL, NULL, NULL, NULL, NULL, 25, 27, 26, 9, 12, 11, 5, 15, 7, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_R0_REG1    GPIO_HWORD_REG_VAL(R0, 3, 4, 13, 10, 14, 8, 22, 40, 37, 39, 38, 35, 36, 66, 72, 67, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_R0_REG2    GPIO_HWORD_REG_VAL(R0, 71, 69, 70, 68, 50, 52, 51, 53, 54, 55, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_R0_ALL_VALUE     GPIO_R0_REG0,GPIO_R0_REG1,GPIO_R0_REG2   

#define    GPIO_R1_ADDR0                            (GPIO_base+0x0b30)
#define    GPIO_R1_SET_REG0                         (GPIO_base+0x0b34)
#define    GPIO_R1_CLR_REG0                         (GPIO_base+0x0b38)
#define    GPIO_R1_OFFSET_TO_NEXT                   0x10
#define    GPIO_R1_REG_MAX_NUM                      2
#define    GPIO_R1_REG_BITS                         16
#define    GPIO_R1_ONE_CONTROL_BITS                 1
#define    GPIO_R1_ONE_REG_CONTROL_NUM              16
#define    GPIO_R1_REG0    GPIO_HWORD_REG_VAL(R1, NULL, NULL, NULL, NULL, NULL, NULL, 25, 27, 26, 9, 12, 11, 5, 15, 7, 6, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_R1_REG1    GPIO_HWORD_REG_VAL(R1, 3, 4, 13, 10, 14, 8, 22, 40, 37, 39, 38, 35, 36, 66, 72, 67, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_R1_REG2    GPIO_HWORD_REG_VAL(R1, 71, 69, 70, 68, 50, 52, 51, 53, 54, 55, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_R1_ALL_VALUE     GPIO_R1_REG0,GPIO_R1_REG1,GPIO_R1_REG2   

#define    CLKO_MODE0                               (GPIO_base+0x0e00)
#define    GPIO_CLK_MODE_OFFSET_TO_NEXT             0x10
#define    GPIO_CLK_MODE_REG_MAX_NUM                1
#define    GPIO_CLOCK_SEL_REG_BITS                  16
#define    GPIO_CLOCK_SEL_ONE_CONTROL_BITS          4
#define    GPIO_CLOCK_SEL_ONE_REG_CONTROL_NUM       1
#define    GPIO_F32K_CK                             0x04
#define    GPIO_HAVE_MAX_MODE                       7
#define    GPIO_DDIE_MIN_GPIO_PIN                   0
#define    GPIO_DDIE_MAX_GPIO_PIN                   72

#define    GPIO_PUPD_R_MAP   NULL,NULL,NULL,NULL,NULL,NULL,25,27,26,9,12,11,5,15,7,6,3,4,13,10,14,8,22,40,37,39,38,35,36,66,72,67,71,69,70,68,50,52,51,53,54,55


#define    GPIO_WORD_VAR0(name)    GPIO_WORD_VAR_FOR_1BIT(name, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31)
#define    GPIO_WORD_VAR1(name)    GPIO_WORD_VAR_FOR_1BIT(name, 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48, 49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63)
#define    GPIO_WORD_VAR2(name)    GPIO_WORD_VAR_FOR_1BIT(name, 64,  65,  66,  67,  68,  69,  70,  71,  72, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_DIR_IN_ALLOWED_ARRAY    GPIO_WORD_VAR0(DIR_IN),GPIO_WORD_VAR1(DIR_OUT),GPIO_WORD_VAR2(DIR_OUT)
#define    GPIO_DIR_OUT_ALLOWED_ARRAY    GPIO_WORD_VAR0(DIR_OUT),GPIO_WORD_VAR1(DIR_OUT),GPIO_WORD_VAR2(DIR_OUT)
#define    GPIO_CHECK_FOR_READ_ARRAY    0xffffffff,0xffffffff,0xffffffff
#define    GPIO_CHECK_FOR_WRITE_ARRAY    0xffffffff,0xffffffff,0xffffffff
#define    GPIO_MODE_ALLOWED_ARRAY    { GPIO_WORD_VAR0(MODE0),GPIO_WORD_VAR0(MODE1),GPIO_WORD_VAR0(MODE2),GPIO_WORD_VAR0(MODE3),GPIO_WORD_VAR0(MODE4),GPIO_WORD_VAR0(MODE5),GPIO_WORD_VAR0(MODE6),GPIO_WORD_VAR0(MODE7)} \
,{GPIO_WORD_VAR1(MODE0),GPIO_WORD_VAR1(MODE1),GPIO_WORD_VAR1(MODE2),GPIO_WORD_VAR1(MODE3),GPIO_WORD_VAR1(MODE4),GPIO_WORD_VAR1(MODE5),GPIO_WORD_VAR1(MODE6),GPIO_WORD_VAR1(MODE7)}  \
,{GPIO_WORD_VAR2(MODE0),GPIO_WORD_VAR2(MODE1),GPIO_WORD_VAR2(MODE2),GPIO_WORD_VAR2(MODE3),GPIO_WORD_VAR2(MODE4),GPIO_WORD_VAR2(MODE5),GPIO_WORD_VAR2(MODE6),GPIO_WORD_VAR2(MODE7)}





/*A die macro ****************************************************************************************/

/*if no define 
ADIE_GPIO_PULLSEL_UP_IS_1 
or DDIE_GPIO_PUPD_UP_IS_1 
or DDIE_GPIO_PULLSEL_UP_IS_1 
means: down is 1*/

#define    GPIO_ADIE_MODE0                          (ANA_CFGSYS_base+0x0144)
#define    GPIO_ADIE_MODE_OFFSET_TO_NEXT            0x04
#define    GPIO_ADIE_MODE_REG_MAX_NUM               2
#define    GPIO_ADIE_MODE_REG_BITS                  16
#define    GPIO_ADIE_MODE_ONE_CONTROL_BITS          4
#define    GPIO_ADIE_MODE_ONE_REG_CONTROL_NUM       4
#define    GPIO_ADIE_MODE_REG0    GPIO_HWORD_REG_VAL(ADIE_MODE, NULL, NULL, 74, 73, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_ADIE_MODE_REG1    GPIO_HWORD_REG_VAL(ADIE_MODE, 75, 76, 77, 78, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_ADIE_MODE_REG2    GPIO_HWORD_REG_VAL(ADIE_MODE, 80, 81, 82, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_ADIE_MODE_ALL_VALUE     GPIO_ADIE_MODE_REG0,GPIO_ADIE_MODE_REG1,GPIO_ADIE_MODE_REG2   

#define    GPIO_ADIE_DIR0                           (ANA_CFGSYS_base+0x0134)
#define    GPIO_ADIE_DIR_OFFSET_TO_NEXT             0x00
#define    GPIO_ADIE_DIR_REG_MAX_NUM                0
#define    GPIO_ADIE_DIR_REG_BITS                   16
#define    GPIO_ADIE_DIR_ONE_CONTROL_BITS           1
#define    GPIO_ADIE_DIR_ONE_REG_CONTROL_NUM        16
#define    GPIO_ADIE_DIR_REG0    GPIO_HWORD_REG_VAL(ADIE_DIR, 74, 73, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_ADIE_DIR_ALL_VALUE     GPIO_ADIE_DIR_REG0   

#define    GPIO_ADIE_OUTPUT_LEVEL0                  (ANA_CFGSYS_base+0x0138)
#define    GPIO_ADIE_OUTPUT_LEVEL_OFFSET_TO_NEXT    0x00
#define    GPIO_ADIE_OUTPUT_LEVEL_REG_MAX_NUM       0
#define    GPIO_ADIE_OUTPUT_LEVEL_REG_BITS          16
#define    GPIO_ADIE_OUTPUT_LEVEL_ONE_CONTROL_BITS  1
#define    GPIO_ADIE_OUTPUT_LEVEL_ONE_REG_CONTROL_NUM 16
#define    GPIO_ADIE_OUTPUT_LEVEL_REG0    GPIO_HWORD_REG_VAL(ADIE_OUTPUT_LEVEL, 74, 73, 79, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL)
#define    GPIO_ADIE_OUTPUT_LEVEL_ALL_VALUE     GPIO_ADIE_OUTPUT_LEVEL_REG0   

#define    GPIO_ADIE_DIN0                           (ANA_CFGSYS_base+0x013c)
#define    GPIO_ADIE_DIN_OFFSET_TO_NEXT             0x00
#define    GPIO_ADIE_DIN_REG_MAX_NUM                0
#define    GPIO_ADIE_DIN_REG_BITS                   16
#define    GPIO_ADIE_DIN_ONE_CONTROL_BITS           1
#define    GPIO_ADIE_DIN_ONE_REG_CONTROL_NUM        16
#define    GPIO_ADIE_MIN_GPIO_PIN                   73
#define    GPIO_ADIE_MAX_GPIO_PIN                   82

#define    GPIO_ADIE_SPECIAL_CASE_REG_MAX_NUM       0

#define    GPIO_ADIE_SPECIAL_CASE_REG0_ADDR          (ANA_CFGSYS_base+0x012C)
#define    GPIO_ADIE_SPECIAL_CASE_REG0_SHIFT          0
#define    GPIO_ADIE_SPECIAL_CASE_REG0    GPIO_ADIE_XP_XM_YP_YM_CHANGE_A_OR_D<<GPIO_ADIE_SPECIAL_CASE_REG0_SHIFT 
#define    GPIO_ADIE_SPECIAL_CASE_REG0_SHIFT          0
#define    GPIO_ADIE_SPECIAL_CASE_ALL_ADDR     (ANA_CFGSYS_base+0x012C) 
#define    GPIO_ADIE_SPECIAL_CASE_ALL_VALUE     GPIO_ADIE_SPECIAL_CASE_REG0 

#define GPIO_A_DIE_MAP          { 12  , 1 , 1 , 7 , 73 , (ANA_CFGSYS_base+0x0144) , (ANA_CFGSYS_base+0x0134) , (ANA_CFGSYS_base+0x0138) , (ANA_CFGSYS_base+0x013C)} ,\
 { 8 , 0 , 0 , 6 , 74 , (ANA_CFGSYS_base+0x0144) , (ANA_CFGSYS_base+0x0134) , (ANA_CFGSYS_base+0x0138) , (ANA_CFGSYS_base+0x013C)} ,\
 { 0 , 0xFF , 0xFF , 8 , 75 , (ANA_CFGSYS_base+0x0148) , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x013C)} ,\
 { 4 , 0xFF , 0xFF , 9 , 76 , (ANA_CFGSYS_base+0x0148) , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x013C)} ,\
 { 8 , 0xFF , 0xFF , 10 , 77 , (ANA_CFGSYS_base+0x0148) , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x013C)} ,\
 { 12 , 0xFF , 0xFF , 11 , 78 , (ANA_CFGSYS_base+0x0148) , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x013C)} ,\
 { 0xFF , 0xFF , 2 , 0xFF , 79 , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x0138) , 0xFFFFFFFF} ,\
 { 0 , 0xFF , 0xFF , 12 , 80 , (ANA_CFGSYS_base+0x014C) , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x013C)} ,\
 { 4 , 0xFF , 0xFF , 13 , 81 , (ANA_CFGSYS_base+0x014C) , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x013C)} ,\
 { 8 , 0xFF , 0xFF , 14 , 82 , (ANA_CFGSYS_base+0x014C) , 0xFFFFFFFF , 0xFFFFFFFF , (ANA_CFGSYS_base+0x013C)}  



#endif /*!defined(DRV_GPIO_OFF)*/
 #endif   /*_GPIO_HW_MT6260_H*/ 