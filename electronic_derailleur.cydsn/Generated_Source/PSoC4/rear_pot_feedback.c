/*******************************************************************************
* File Name: rear_pot_feedback.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "rear_pot_feedback.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        rear_pot_feedback_PC =   (rear_pot_feedback_PC & \
                                (uint32)(~(uint32)(rear_pot_feedback_DRIVE_MODE_IND_MASK << (rear_pot_feedback_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (rear_pot_feedback_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: rear_pot_feedback_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void rear_pot_feedback_Write(uint8 value) 
{
    uint8 drVal = (uint8)(rear_pot_feedback_DR & (uint8)(~rear_pot_feedback_MASK));
    drVal = (drVal | ((uint8)(value << rear_pot_feedback_SHIFT) & rear_pot_feedback_MASK));
    rear_pot_feedback_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: rear_pot_feedback_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  rear_pot_feedback_DM_STRONG     Strong Drive 
*  rear_pot_feedback_DM_OD_HI      Open Drain, Drives High 
*  rear_pot_feedback_DM_OD_LO      Open Drain, Drives Low 
*  rear_pot_feedback_DM_RES_UP     Resistive Pull Up 
*  rear_pot_feedback_DM_RES_DWN    Resistive Pull Down 
*  rear_pot_feedback_DM_RES_UPDWN  Resistive Pull Up/Down 
*  rear_pot_feedback_DM_DIG_HIZ    High Impedance Digital 
*  rear_pot_feedback_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void rear_pot_feedback_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(rear_pot_feedback__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: rear_pot_feedback_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro rear_pot_feedback_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 rear_pot_feedback_Read(void) 
{
    return (uint8)((rear_pot_feedback_PS & rear_pot_feedback_MASK) >> rear_pot_feedback_SHIFT);
}


/*******************************************************************************
* Function Name: rear_pot_feedback_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 rear_pot_feedback_ReadDataReg(void) 
{
    return (uint8)((rear_pot_feedback_DR & rear_pot_feedback_MASK) >> rear_pot_feedback_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(rear_pot_feedback_INTSTAT) 

    /*******************************************************************************
    * Function Name: rear_pot_feedback_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 rear_pot_feedback_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(rear_pot_feedback_INTSTAT & rear_pot_feedback_MASK);
		rear_pot_feedback_INTSTAT = maskedStatus;
        return maskedStatus >> rear_pot_feedback_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
