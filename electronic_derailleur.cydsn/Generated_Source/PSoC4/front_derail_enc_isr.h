/*******************************************************************************
* File Name: front_derail_enc_isr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_front_derail_enc_isr_H)
#define CY_ISR_front_derail_enc_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void front_derail_enc_isr_Start(void);
void front_derail_enc_isr_StartEx(cyisraddress address);
void front_derail_enc_isr_Stop(void);

CY_ISR_PROTO(front_derail_enc_isr_Interrupt);

void front_derail_enc_isr_SetVector(cyisraddress address);
cyisraddress front_derail_enc_isr_GetVector(void);

void front_derail_enc_isr_SetPriority(uint8 priority);
uint8 front_derail_enc_isr_GetPriority(void);

void front_derail_enc_isr_Enable(void);
uint8 front_derail_enc_isr_GetState(void);
void front_derail_enc_isr_Disable(void);

void front_derail_enc_isr_SetPending(void);
void front_derail_enc_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the front_derail_enc_isr ISR. */
#define front_derail_enc_isr_INTC_VECTOR            ((reg32 *) front_derail_enc_isr__INTC_VECT)

/* Address of the front_derail_enc_isr ISR priority. */
#define front_derail_enc_isr_INTC_PRIOR             ((reg32 *) front_derail_enc_isr__INTC_PRIOR_REG)

/* Priority of the front_derail_enc_isr interrupt. */
#define front_derail_enc_isr_INTC_PRIOR_NUMBER      front_derail_enc_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable front_derail_enc_isr interrupt. */
#define front_derail_enc_isr_INTC_SET_EN            ((reg32 *) front_derail_enc_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the front_derail_enc_isr interrupt. */
#define front_derail_enc_isr_INTC_CLR_EN            ((reg32 *) front_derail_enc_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the front_derail_enc_isr interrupt state to pending. */
#define front_derail_enc_isr_INTC_SET_PD            ((reg32 *) front_derail_enc_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the front_derail_enc_isr interrupt. */
#define front_derail_enc_isr_INTC_CLR_PD            ((reg32 *) front_derail_enc_isr__INTC_CLR_PD_REG)



#endif /* CY_ISR_front_derail_enc_isr_H */


/* [] END OF FILE */
