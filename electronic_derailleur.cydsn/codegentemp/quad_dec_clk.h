/*******************************************************************************
* File Name: quad_dec_clk.h
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

#if !defined(CY_CLOCK_quad_dec_clk_H)
#define CY_CLOCK_quad_dec_clk_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void quad_dec_clk_StartEx(uint32 alignClkDiv);
#define quad_dec_clk_Start() \
    quad_dec_clk_StartEx(quad_dec_clk__PA_DIV_ID)

#else

void quad_dec_clk_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void quad_dec_clk_Stop(void);

void quad_dec_clk_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 quad_dec_clk_GetDividerRegister(void);
uint8  quad_dec_clk_GetFractionalDividerRegister(void);

#define quad_dec_clk_Enable()                         quad_dec_clk_Start()
#define quad_dec_clk_Disable()                        quad_dec_clk_Stop()
#define quad_dec_clk_SetDividerRegister(clkDivider, reset)  \
    quad_dec_clk_SetFractionalDividerRegister((clkDivider), 0u)
#define quad_dec_clk_SetDivider(clkDivider)           quad_dec_clk_SetDividerRegister((clkDivider), 1u)
#define quad_dec_clk_SetDividerValue(clkDivider)      quad_dec_clk_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define quad_dec_clk_DIV_ID     quad_dec_clk__DIV_ID

#define quad_dec_clk_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define quad_dec_clk_CTRL_REG   (*(reg32 *)quad_dec_clk__CTRL_REGISTER)
#define quad_dec_clk_DIV_REG    (*(reg32 *)quad_dec_clk__DIV_REGISTER)

#define quad_dec_clk_CMD_DIV_SHIFT          (0u)
#define quad_dec_clk_CMD_PA_DIV_SHIFT       (8u)
#define quad_dec_clk_CMD_DISABLE_SHIFT      (30u)
#define quad_dec_clk_CMD_ENABLE_SHIFT       (31u)

#define quad_dec_clk_CMD_DISABLE_MASK       ((uint32)((uint32)1u << quad_dec_clk_CMD_DISABLE_SHIFT))
#define quad_dec_clk_CMD_ENABLE_MASK        ((uint32)((uint32)1u << quad_dec_clk_CMD_ENABLE_SHIFT))

#define quad_dec_clk_DIV_FRAC_MASK  (0x000000F8u)
#define quad_dec_clk_DIV_FRAC_SHIFT (3u)
#define quad_dec_clk_DIV_INT_MASK   (0xFFFFFF00u)
#define quad_dec_clk_DIV_INT_SHIFT  (8u)

#else 

#define quad_dec_clk_DIV_REG        (*(reg32 *)quad_dec_clk__REGISTER)
#define quad_dec_clk_ENABLE_REG     quad_dec_clk_DIV_REG
#define quad_dec_clk_DIV_FRAC_MASK  quad_dec_clk__FRAC_MASK
#define quad_dec_clk_DIV_FRAC_SHIFT (16u)
#define quad_dec_clk_DIV_INT_MASK   quad_dec_clk__DIVIDER_MASK
#define quad_dec_clk_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_quad_dec_clk_H) */

/* [] END OF FILE */
