/*******************************************************************************
* File Name: shifter_switches.h  
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

#if !defined(CY_PINS_shifter_switches_H) /* Pins shifter_switches_H */
#define CY_PINS_shifter_switches_H

#include "cytypes.h"
#include "cyfitter.h"
#include "shifter_switches_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    shifter_switches_Write(uint8 value) ;
void    shifter_switches_SetDriveMode(uint8 mode) ;
uint8   shifter_switches_ReadDataReg(void) ;
uint8   shifter_switches_Read(void) ;
uint8   shifter_switches_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define shifter_switches_DRIVE_MODE_BITS        (3)
#define shifter_switches_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - shifter_switches_DRIVE_MODE_BITS))

#define shifter_switches_DM_ALG_HIZ         (0x00u)
#define shifter_switches_DM_DIG_HIZ         (0x01u)
#define shifter_switches_DM_RES_UP          (0x02u)
#define shifter_switches_DM_RES_DWN         (0x03u)
#define shifter_switches_DM_OD_LO           (0x04u)
#define shifter_switches_DM_OD_HI           (0x05u)
#define shifter_switches_DM_STRONG          (0x06u)
#define shifter_switches_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define shifter_switches_MASK               shifter_switches__MASK
#define shifter_switches_SHIFT              shifter_switches__SHIFT
#define shifter_switches_WIDTH              2u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define shifter_switches_PS                     (* (reg32 *) shifter_switches__PS)
/* Port Configuration */
#define shifter_switches_PC                     (* (reg32 *) shifter_switches__PC)
/* Data Register */
#define shifter_switches_DR                     (* (reg32 *) shifter_switches__DR)
/* Input Buffer Disable Override */
#define shifter_switches_INP_DIS                (* (reg32 *) shifter_switches__PC2)


#if defined(shifter_switches__INTSTAT)  /* Interrupt Registers */

    #define shifter_switches_INTSTAT                (* (reg32 *) shifter_switches__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define shifter_switches_DRIVE_MODE_SHIFT       (0x00u)
#define shifter_switches_DRIVE_MODE_MASK        (0x07u << shifter_switches_DRIVE_MODE_SHIFT)


#endif /* End Pins shifter_switches_H */


/* [] END OF FILE */
