/*******************************************************************************
* File Name: in_3.h  
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

#if !defined(CY_PINS_in_3_H) /* Pins in_3_H */
#define CY_PINS_in_3_H

#include "cytypes.h"
#include "cyfitter.h"
#include "in_3_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    in_3_Write(uint8 value) ;
void    in_3_SetDriveMode(uint8 mode) ;
uint8   in_3_ReadDataReg(void) ;
uint8   in_3_Read(void) ;
uint8   in_3_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define in_3_DRIVE_MODE_BITS        (3)
#define in_3_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - in_3_DRIVE_MODE_BITS))

#define in_3_DM_ALG_HIZ         (0x00u)
#define in_3_DM_DIG_HIZ         (0x01u)
#define in_3_DM_RES_UP          (0x02u)
#define in_3_DM_RES_DWN         (0x03u)
#define in_3_DM_OD_LO           (0x04u)
#define in_3_DM_OD_HI           (0x05u)
#define in_3_DM_STRONG          (0x06u)
#define in_3_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define in_3_MASK               in_3__MASK
#define in_3_SHIFT              in_3__SHIFT
#define in_3_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define in_3_PS                     (* (reg32 *) in_3__PS)
/* Port Configuration */
#define in_3_PC                     (* (reg32 *) in_3__PC)
/* Data Register */
#define in_3_DR                     (* (reg32 *) in_3__DR)
/* Input Buffer Disable Override */
#define in_3_INP_DIS                (* (reg32 *) in_3__PC2)


#if defined(in_3__INTSTAT)  /* Interrupt Registers */

    #define in_3_INTSTAT                (* (reg32 *) in_3__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define in_3_DRIVE_MODE_SHIFT       (0x00u)
#define in_3_DRIVE_MODE_MASK        (0x07u << in_3_DRIVE_MODE_SHIFT)


#endif /* End Pins in_3_H */


/* [] END OF FILE */
