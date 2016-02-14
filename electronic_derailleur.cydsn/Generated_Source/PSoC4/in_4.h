/*******************************************************************************
* File Name: in_4.h  
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

#if !defined(CY_PINS_in_4_H) /* Pins in_4_H */
#define CY_PINS_in_4_H

#include "cytypes.h"
#include "cyfitter.h"
#include "in_4_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    in_4_Write(uint8 value) ;
void    in_4_SetDriveMode(uint8 mode) ;
uint8   in_4_ReadDataReg(void) ;
uint8   in_4_Read(void) ;
uint8   in_4_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define in_4_DRIVE_MODE_BITS        (3)
#define in_4_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - in_4_DRIVE_MODE_BITS))

#define in_4_DM_ALG_HIZ         (0x00u)
#define in_4_DM_DIG_HIZ         (0x01u)
#define in_4_DM_RES_UP          (0x02u)
#define in_4_DM_RES_DWN         (0x03u)
#define in_4_DM_OD_LO           (0x04u)
#define in_4_DM_OD_HI           (0x05u)
#define in_4_DM_STRONG          (0x06u)
#define in_4_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define in_4_MASK               in_4__MASK
#define in_4_SHIFT              in_4__SHIFT
#define in_4_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define in_4_PS                     (* (reg32 *) in_4__PS)
/* Port Configuration */
#define in_4_PC                     (* (reg32 *) in_4__PC)
/* Data Register */
#define in_4_DR                     (* (reg32 *) in_4__DR)
/* Input Buffer Disable Override */
#define in_4_INP_DIS                (* (reg32 *) in_4__PC2)


#if defined(in_4__INTSTAT)  /* Interrupt Registers */

    #define in_4_INTSTAT                (* (reg32 *) in_4__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define in_4_DRIVE_MODE_SHIFT       (0x00u)
#define in_4_DRIVE_MODE_MASK        (0x07u << in_4_DRIVE_MODE_SHIFT)


#endif /* End Pins in_4_H */


/* [] END OF FILE */
