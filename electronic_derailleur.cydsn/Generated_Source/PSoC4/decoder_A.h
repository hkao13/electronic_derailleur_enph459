/*******************************************************************************
* File Name: decoder_A.h  
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

#if !defined(CY_PINS_decoder_A_H) /* Pins decoder_A_H */
#define CY_PINS_decoder_A_H

#include "cytypes.h"
#include "cyfitter.h"
#include "decoder_A_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    decoder_A_Write(uint8 value) ;
void    decoder_A_SetDriveMode(uint8 mode) ;
uint8   decoder_A_ReadDataReg(void) ;
uint8   decoder_A_Read(void) ;
uint8   decoder_A_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define decoder_A_DRIVE_MODE_BITS        (3)
#define decoder_A_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - decoder_A_DRIVE_MODE_BITS))

#define decoder_A_DM_ALG_HIZ         (0x00u)
#define decoder_A_DM_DIG_HIZ         (0x01u)
#define decoder_A_DM_RES_UP          (0x02u)
#define decoder_A_DM_RES_DWN         (0x03u)
#define decoder_A_DM_OD_LO           (0x04u)
#define decoder_A_DM_OD_HI           (0x05u)
#define decoder_A_DM_STRONG          (0x06u)
#define decoder_A_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define decoder_A_MASK               decoder_A__MASK
#define decoder_A_SHIFT              decoder_A__SHIFT
#define decoder_A_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define decoder_A_PS                     (* (reg32 *) decoder_A__PS)
/* Port Configuration */
#define decoder_A_PC                     (* (reg32 *) decoder_A__PC)
/* Data Register */
#define decoder_A_DR                     (* (reg32 *) decoder_A__DR)
/* Input Buffer Disable Override */
#define decoder_A_INP_DIS                (* (reg32 *) decoder_A__PC2)


#if defined(decoder_A__INTSTAT)  /* Interrupt Registers */

    #define decoder_A_INTSTAT                (* (reg32 *) decoder_A__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define decoder_A_DRIVE_MODE_SHIFT       (0x00u)
#define decoder_A_DRIVE_MODE_MASK        (0x07u << decoder_A_DRIVE_MODE_SHIFT)


#endif /* End Pins decoder_A_H */


/* [] END OF FILE */
