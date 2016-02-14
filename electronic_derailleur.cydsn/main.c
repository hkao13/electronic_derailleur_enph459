/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <project.h>

#define SHIFT_IDLE 0x00
#define F_SHIFT_UP 0x01
#define F_SHIFT_DN 0x02
#define R_SHIFT_UP 0x03
#define R_SHIFT_DN 0x04

#define STOP 0x00
#define ALL_ON 0x00
#define FRONT_UP 0x01
#define FRONT_DN 0x02
#define REAR_UP 0x01
#define REAR_DN 0x02

int shift_isr_status = 0x0u;


/* Initialize the interrupt fuction prototype */
CY_ISR_PROTO(Shifter_ISR); 

int main()
{
    /* UART buffer to store output to console */
    //char bufferUART[20] = {};

    
    /* Set up interrupt vector address for function */
    shifter_isr_StartEx(Shifter_ISR);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /* Init the UART module for console debugging */
    UART_Start();  
    UART_UartPutString("PSoC UART Communication Established\n\r");
    
    PWM_Start();
    PWM_WriteCompare1(255u);
    PWM_WriteCompare2(255u);

    for(;;)
    {
        /* Place your application code here. */
        CyDelay(100u);
        if(shift_isr_status == 1)
        {
            UART_UartPutString("UP\n\r");
            front_motor_Write(FRONT_UP);
            CyDelay(1500u);
        } 
        else if (shift_isr_status == 2)
        {
            UART_UartPutString("DN\n\r");
            front_motor_Write(FRONT_DN);
            CyDelay(1500u);
        }
        shift_isr_status &= 0;
        front_motor_Write(STOP);
        
    }
}

/* ISR for Shifter commands after gpio_isr is triggered */
CY_ISR(Shifter_ISR)
{
    /* Clear pending interupts from gpio_isr */
    shifter_isr_ClearPending();
    
    shift_isr_status &= 0;
    
    /* Check shifter_switches interrupt status register to see which interrupt is
    being triggered. P0.0 for up shift and P0.1 for down shift */
    if(shifter_switches_INTSTAT & (1u << shifter_switches_SHIFT))
    {
        //UART_UartPutString("SHIFT UP!\n\r");
        shift_isr_status |= (1 << 0);
    }
    else if (shifter_switches_INTSTAT & (2u << shifter_switches_SHIFT))
    {
        //UART_UartPutString("SHIFT DOWN!\n\r");
        shift_isr_status |= (1 << 1);
    }
    else {
        UART_UartPutString("COMMAND UNKNOWN!\n\r");
    }
    /* Clear the interupt caused by shifter_switches */
    shifter_switches_ClearInterrupt(); 
}

/* [] END OF FILE */
