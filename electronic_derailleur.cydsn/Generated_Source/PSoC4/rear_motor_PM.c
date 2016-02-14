/*******************************************************************************
* File Name: rear_motor_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "rear_motor.h"

/* Check for removal by optimization */
#if !defined(rear_motor_Sync_ctrl_reg__REMOVED)

static rear_motor_BACKUP_STRUCT  rear_motor_backup = {0u};

    
/*******************************************************************************
* Function Name: rear_motor_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void rear_motor_SaveConfig(void) 
{
    rear_motor_backup.controlState = rear_motor_Control;
}


/*******************************************************************************
* Function Name: rear_motor_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void rear_motor_RestoreConfig(void) 
{
     rear_motor_Control = rear_motor_backup.controlState;
}


/*******************************************************************************
* Function Name: rear_motor_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void rear_motor_Sleep(void) 
{
    rear_motor_SaveConfig();
}


/*******************************************************************************
* Function Name: rear_motor_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void rear_motor_Wakeup(void)  
{
    rear_motor_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
