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
char bufferUART[20] = {};
int main()
{
    uint16 res = 0;
    
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

    ADC_SAR_SEQ_Start();
    ADC_SAR_SEQ_StartConvert();
    
    for(;;)
    {
        /* Check for the shifter interrupt status variable for shift commands */
        if(shift_isr_status & (1 << 0))
        {
            front_motor_Write(FRONT_DN);
            if(QuadDec_ReadCounter() >= ref_front_count + FRONT_UPPER_LIMIT)
            {  
                front_motor_Write(STOP);
                shift_isr_status &= 0xC;
                sprintf(bufferUART, "%lu, %lu\n\r", QuadDec_ReadCounter(), ref_front_count + FRONT_UPPER_LIMIT);
            }    
        } 
        else if (shift_isr_status & (1 << 1))
        {
            front_motor_Write(FRONT_UP);
            if (QuadDec_ReadCounter() <= ref_front_count - FRONT_LOWER_LIMIT)
            {
                front_motor_Write(STOP);
                shift_isr_status &= 0xC;
                sprintf(bufferUART, "%lu, %lu\n\r", QuadDec_ReadCounter(), ref_front_count - FRONT_UPPER_LIMIT);
            }
        }
        else if (shift_isr_status & (1 << 2))
        {
            rear_motor_Write(REAR_UP);
            if (ADC_SAR_SEQ_GetResult16(0x00) >= (int16)1500)
            {
                rear_motor_Write(STOP);
                shift_isr_status &= 0x3;
                sprintf(bufferUART, "%d, %d\n\r", ADC_SAR_SEQ_GetResult16(0x00), 1500);
            }
        }
        else if (shift_isr_status & (1 << 3))
        {
            rear_motor_Write(REAR_DN);
            if (ADC_SAR_SEQ_GetResult16(0x00) <= (int16)500)
            {
                rear_motor_Write(STOP);
                shift_isr_status &= 0x3;
                sprintf(bufferUART, "%d, %d\n\r", ADC_SAR_SEQ_GetResult16(0x00), 500);
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
    being triggered. */
    if(shifter_switches_INTSTAT & (1u << shifter_switches_SHIFT))
    {
        UART_UartPutString("SHIFT UP!\n\r");
        shift_isr_status &= 0xC;
        shift_isr_status |= (1 << 0);
    }
    else if (shifter_switches_INTSTAT & (2u << shifter_switches_SHIFT))
    {
        UART_UartPutString("SHIFT DOWN!\n\r");
        shift_isr_status &= 0xC;
        shift_isr_status |= (1 << 1);
    }
    else if (shifter_switches_INTSTAT & (4u << shifter_switches_SHIFT))
    {
        shift_isr_status &= 0x3;
        shift_isr_status |= (2 << 1);
    }
    else if (shifter_switches_INTSTAT & (8u << shifter_switches_SHIFT))
    {       
        shift_isr_status &= 0x3;
        shift_isr_status |= (1 << 3);
    }
    else {
        UART_UartPutString("COMMAND UNKNOWN!\n\r");
    }
    /* Clear the interupt caused by shifter_switches */
    shifter_switches_ClearInterrupt(); 
}


/* [] END OF FILE */
