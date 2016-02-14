/*******************************************************************************
* File Name: pwm_clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_pwm_clock_H)
#define CY_CLOCK_pwm_clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void pwm_clock_StartEx(uint32 alignClkDiv);
#define pwm_clock_Start() \
    pwm_clock_StartEx(pwm_clock__PA_DIV_ID)

#else

void pwm_clock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void pwm_clock_Stop(void);

void pwm_clock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 pwm_clock_GetDividerRegister(void);
uint8  pwm_clock_GetFractionalDividerRegister(void);

#define pwm_clock_Enable()                         pwm_clock_Start()
#define pwm_clock_Disable()                        pwm_clock_Stop()
#define pwm_clock_SetDividerRegister(clkDivider, reset)  \
    pwm_clock_SetFractionalDividerRegister((clkDivider), 0u)
#define pwm_clock_SetDivider(clkDivider)           pwm_clock_SetDividerRegister((clkDivider), 1u)
#define pwm_clock_SetDividerValue(clkDivider)      pwm_clock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define pwm_clock_DIV_ID     pwm_clock__DIV_ID

#define pwm_clock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define pwm_clock_CTRL_REG   (*(reg32 *)pwm_clock__CTRL_REGISTER)
#define pwm_clock_DIV_REG    (*(reg32 *)pwm_clock__DIV_REGISTER)

#define pwm_clock_CMD_DIV_SHIFT          (0u)
#define pwm_clock_CMD_PA_DIV_SHIFT       (8u)
#define pwm_clock_CMD_DISABLE_SHIFT      (30u)
#define pwm_clock_CMD_ENABLE_SHIFT       (31u)

#define pwm_clock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << pwm_clock_CMD_DISABLE_SHIFT))
#define pwm_clock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << pwm_clock_CMD_ENABLE_SHIFT))

#define pwm_clock_DIV_FRAC_MASK  (0x000000F8u)
#define pwm_clock_DIV_FRAC_SHIFT (3u)
#define pwm_clock_DIV_INT_MASK   (0xFFFFFF00u)
#define pwm_clock_DIV_INT_SHIFT  (8u)

#else 

#define pwm_clock_DIV_REG        (*(reg32 *)pwm_clock__REGISTER)
#define pwm_clock_ENABLE_REG     pwm_clock_DIV_REG
#define pwm_clock_DIV_FRAC_MASK  pwm_clock__FRAC_MASK
#define pwm_clock_DIV_FRAC_SHIFT (16u)
#define pwm_clock_DIV_INT_MASK   pwm_clock__DIVIDER_MASK
#define pwm_clock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_pwm_clock_H) */

/* [] END OF FILE */
