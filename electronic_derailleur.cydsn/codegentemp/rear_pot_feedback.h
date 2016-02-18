/*******************************************************************************
* File Name: rear_pot_feedback.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_rear_pot_feedback_H) /* Pins rear_pot_feedback_H */
#define CY_PINS_rear_pot_feedback_H

#include "cytypes.h"
#include "cyfitter.h"
#include "rear_pot_feedback_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    rear_pot_feedback_Write(uint8 value) ;
void    rear_pot_feedback_SetDriveMode(uint8 mode) ;
uint8   rear_pot_feedback_ReadDataReg(void) ;
uint8   rear_pot_feedback_Read(void) ;
uint8   rear_pot_feedback_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define rear_pot_feedback_DRIVE_MODE_BITS        (3)
#define rear_pot_feedback_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - rear_pot_feedback_DRIVE_MODE_BITS))

#define rear_pot_feedback_DM_ALG_HIZ         (0x00u)
#define rear_pot_feedback_DM_DIG_HIZ         (0x01u)
#define rear_pot_feedback_DM_RES_UP          (0x02u)
#define rear_pot_feedback_DM_RES_DWN         (0x03u)
#define rear_pot_feedback_DM_OD_LO           (0x04u)
#define rear_pot_feedback_DM_OD_HI           (0x05u)
#define rear_pot_feedback_DM_STRONG          (0x06u)
#define rear_pot_feedback_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define rear_pot_feedback_MASK               rear_pot_feedback__MASK
#define rear_pot_feedback_SHIFT              rear_pot_feedback__SHIFT
#define rear_pot_feedback_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define rear_pot_feedback_PS                     (* (reg32 *) rear_pot_feedback__PS)
/* Port Configuration */
#define rear_pot_feedback_PC                     (* (reg32 *) rear_pot_feedback__PC)
/* Data Register */
#define rear_pot_feedback_DR                     (* (reg32 *) rear_pot_feedback__DR)
/* Input Buffer Disable Override */
#define rear_pot_feedback_INP_DIS                (* (reg32 *) rear_pot_feedback__PC2)


#if defined(rear_pot_feedback__INTSTAT)  /* Interrupt Registers */

    #define rear_pot_feedback_INTSTAT                (* (reg32 *) rear_pot_feedback__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define rear_pot_feedback_DRIVE_MODE_SHIFT       (0x00u)
#define rear_pot_feedback_DRIVE_MODE_MASK        (0x07u << rear_pot_feedback_DRIVE_MODE_SHIFT)


#endif /* End Pins rear_pot_feedback_H */


/* [] END OF FILE */
