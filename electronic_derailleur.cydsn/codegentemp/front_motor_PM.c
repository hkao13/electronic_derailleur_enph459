/*******************************************************************************
* File Name: front_motor_PM.c
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

#include "front_motor.h"

/* Check for removal by optimization */
#if !defined(front_motor_Sync_ctrl_reg__REMOVED)

static front_motor_BACKUP_STRUCT  front_motor_backup = {0u};

    
/*******************************************************************************
* Function Name: front_motor_SaveConfig
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
void front_motor_SaveConfig(void) 
{
    front_motor_backup.controlState = front_motor_Control;
}


/*******************************************************************************
* Function Name: front_motor_RestoreConfig
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
void front_motor_RestoreConfig(void) 
{
     front_motor_Control = front_motor_backup.controlState;
}


/*******************************************************************************
* Function Name: front_motor_Sleep
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
void front_motor_Sleep(void) 
{
    front_motor_SaveConfig();
}


/*******************************************************************************
* Function Name: front_motor_Wakeup
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
void front_motor_Wakeup(void)  
{
    front_motor_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
