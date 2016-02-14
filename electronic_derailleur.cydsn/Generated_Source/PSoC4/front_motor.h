/*******************************************************************************
* File Name: front_motor.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_front_motor_H) /* CY_CONTROL_REG_front_motor_H */
#define CY_CONTROL_REG_front_motor_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} front_motor_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    front_motor_Write(uint8 control) ;
uint8   front_motor_Read(void) ;

void front_motor_SaveConfig(void) ;
void front_motor_RestoreConfig(void) ;
void front_motor_Sleep(void) ; 
void front_motor_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define front_motor_Control        (* (reg8 *) front_motor_Sync_ctrl_reg__CONTROL_REG )
#define front_motor_Control_PTR    (  (reg8 *) front_motor_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_front_motor_H */


/* [] END OF FILE */
