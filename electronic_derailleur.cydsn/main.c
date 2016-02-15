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

#define STOP 0x00
#define ALL_ON 0x00
#define FRONT_UP 0x01
#define FRONT_DN 0x02
#define REAR_UP 0x01
#define REAR_DN 0x02

#define FRONT_UPPER_LIMIT 1000u
#define FRONT_LOWER_LIMIT 1000u

int shift_isr_status = 0x0u;

/* Initialize the interrupt fuction prototype */
CY_ISR_PROTO(Shifter_ISR); 

int main()
{
    /* UART buffer to store output to console */
    char bufferUART[20] = {};
    
    /* Initial front motor position variable */
    uint32 ref_front_count;

    /* Set up interrupt vector address for function */
    shifter_isr_StartEx(Shifter_ISR);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /* Init the UART module for console debugging */
    UART_Start();  
    UART_UartPutString("PSoC UART Communication Established\n\r");
    
    /* Startup of the PWM block */
    PWM_Start();
    PWM_WriteCompare1(255u);
    PWM_WriteCompare2(255u);
    
    /* Startup of the quadrature decoder block for front derailleur */
    QuadDec_Start();
    QuadDec_TriggerCommand(QuadDec_MASK, QuadDec_CMD_RELOAD);
    ref_front_count = QuadDec_ReadCounter();

    for(;;)
    {
        /* Check for the shifter interrupt status variable for shift commands */
        if(shift_isr_status == 1)
        {
            front_motor_Write(FRONT_DN);
            if(QuadDec_ReadCounter() >= ref_front_count + FRONT_UPPER_LIMIT)
            {  
                front_motor_Write(STOP);
                shift_isr_status &= 0;
                sprintf(bufferUART, "%lu, %lu\n\r", QuadDec_ReadCounter(), ref_front_count + FRONT_UPPER_LIMIT);
            }    
        } 
        else if (shift_isr_status == 2)
        {
            front_motor_Write(FRONT_UP);
            if (QuadDec_ReadCounter() <= ref_front_count - FRONT_LOWER_LIMIT)
            {
                front_motor_Write(STOP);
                shift_isr_status &= 0;
                sprintf(bufferUART, "%lu, %lu\n\r", QuadDec_ReadCounter(), ref_front_count - FRONT_UPPER_LIMIT);
            }
        }
        
        UART_UartPutString(bufferUART);   
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
