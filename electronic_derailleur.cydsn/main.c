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

#define NORMAL = 0x0
#define TUNING = 0x1

#define STOP 0x00
#define ALL_ON 0x00
#define FRONT_UP 0x01
#define FRONT_DN 0x02
#define REAR_UP 0x01
#define REAR_DN 0x02

#define ADDRESS1 0x50      /* Address of the EEPROM slave */

int shift_isr_status = 0x0u;

/* Initialize the interrupt fuction prototype */
CY_ISR_PROTO(Shifter_ISR); 


void EEPROM_Write(uint8* Data, int numBytes);
void EEPROM_Read(uint8* Data, int numBytes);

char bufferUART[20] = {};

typedef struct FrontDerailleur
{   
    uint8 position; // Binary position, 0 for small, 1 for large chainring.
    uint16 current_pos; // Quad encoder value for current/last saved position.
    uint16 tuned_pos[2]; // Tuned quad encoder values for small and large chainring positions
    uint16 eep_offset; // FD EEPROM offset save address.
} frntder;

frntder frontDerailInit(uint8* dataI2C);
void frontDerailToData(uint8* dataI2C, frntder fd);

int main()
{
    int mode = 0;
    
    /* UART buffer to store output to console */
    char bufferUART[20] = {};
    
    uint8 dataI2C[] = {};
    
    
    uint8 temp[] = {};
    temp[0] = 0;
    temp[1] = (uint8)((int)32767 >> 8);
    temp[2] = (uint8)((int)32767 & 0xff);
    temp[3] = (uint8)((int)(32767-1000) >> 8);
    temp[4] = (uint8)((int)(32767-1000) & 0xff);
    temp[5] = (uint8)((int)(32767+1000) >> 8);
    temp[6] = (uint8)((int)(32767+1000) & 0xff);
    

    /* Set up interrupt vector address for function */
    shifter_isr_StartEx(Shifter_ISR);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    /* Init the UART module for console debugging */
    UART_Start();  
    UART_UartPutString("PSoC UART Communication Established\n\r");
    
    I2C_Start();
    
    /* Startup of the PWM block */
    PWM_Start();
    PWM_WriteCompare1(255u);
    PWM_WriteCompare2(255u);
    
    /* Startup of the quadrature decoder block for front derailleur */
    QuadDec_Start(); 
    QuadDec_TriggerCommand(QuadDec_MASK, QuadDec_CMD_RELOAD);  
        
    ADC_SAR_SEQ_Start();
    ADC_SAR_SEQ_StartConvert();
    
    //EEPROM_Write(temp, 2);  
    EEPROM_Read(dataI2C, 7);
    
    frntder frontd;
    frontd = frontDerailInit(dataI2C);
    
    sprintf(bufferUART, "%d, %d, %d, %d\n\r", frontd.position, frontd.current_pos, frontd.tuned_pos[0], frontd.tuned_pos[1]);
    UART_UartPutString(bufferUART);
    
    //sprintf(bufferUART, "New Params - current pos: %d, small: %d, large, %d\n\r", frontd.current_pos, frontd.tuned_pos[0], frontd.tuned_pos[1]);
    //UART_UartPutString(bufferUART);
    
    while(mode == 0)
    {

        /* Check for the shifter interrupt status variable for shift commands */
        // Front derailleur shift down
        if(shift_isr_status & (1 << 0))
        { 
            if (frontd.position >= 1)
            {
                UART_UartPutString("HI\n\r");
                frontd.position= 1;
                shift_isr_status &= 0xC;
            }
            else
            {   
                front_motor_Write(FRONT_UP);
                if(QuadDec_ReadCounter() >= frontd.tuned_pos[1])
                {  
                    front_motor_Write(STOP);
                    frontd.current_pos = QuadDec_ReadCounter();
                    shift_isr_status &= 0xC;
                    frontd.position ++;
                    
                    UART_UartPutString("FRONT UP\n\r");
                    sprintf(bufferUART, "%d, %d, %d\n\r", frontd.position, frontd.current_pos, frontd.tuned_pos[1]);
                    UART_UartPutString(bufferUART);
                }         
            } 
            frontDerailToData(dataI2C, frontd);
            EEPROM_Write(dataI2C, 7);
        } 
        // Front derailleur shift up
        else if (shift_isr_status & (1 << 1))
        {
            if(frontd.position == 0)
            {
                frontd.position = 0;
                shift_isr_status &= 0xC;
            }
            else
            {      
                front_motor_Write(FRONT_DN);
                if (QuadDec_ReadCounter() <= frontd.tuned_pos[0])
                {
                    front_motor_Write(STOP);
                    frontd.current_pos = QuadDec_ReadCounter();
                    shift_isr_status &= 0xC;
                    frontd.position --;
                    
                    UART_UartPutString("FRONT DN\n\r");
                    sprintf(bufferUART, "%d, %d, %d\n\r", frontd.position, frontd.current_pos, frontd.tuned_pos[0]);
                    UART_UartPutString(bufferUART);
                }
                frontDerailToData(dataI2C, frontd);
                EEPROM_Write(dataI2C, 7);
            } 
        }
        
        /*
        // Rear derailleur shift up
        else if (shift_isr_status & (1 << 2))
        {
            rear_motor_Write(REAR_UP);
            if (ADC_SAR_SEQ_GetResult16(0x00) >= (int16)1500)
            {
                rear_motor_Write(STOP);
                shift_isr_status &= 0x3;
                sprintf(bufferUART, "%d, %d\n\r", ADC_SAR_SEQ_GetResult16(0x00), 1500);
            }
            // Check the least significant nibble for the maximum allowed value
            if ((shifter_pos & 0x0fu) > 0x0Cu)
            {
                shifter_pos &= 0xfC;
            }
            // Otherwise increment the least significant nibble
            else
            {
                shifter_pos += 1;
            }
            dataI2C[0] = shifter_pos;
            EEPROM_Write(dataI2C, 15);
        }
        // Rear derailleur shift down
        else if (shift_isr_status & (1 << 3))
        {
            rear_motor_Write(REAR_DN);
            if (ADC_SAR_SEQ_GetResult16(0x00) <= (int16)500)
            {
                rear_motor_Write(STOP);
                shift_isr_status &= 0x3;
                sprintf(bufferUART, "%d, %d\n\r", ADC_SAR_SEQ_GetResult16(0x00), 500);
            }
            // Check least significant nibble for lowest position or overflow
            if ((shifter_pos & 0x0fu) == 0x00u)
            {
                shifter_pos &= 0xf0u;
            }
            else if ((shifter_pos & 0x0fu) >= 11u)
            {
                shifter_pos &= 0xf0u;
            }
            // Otherwise decrement the least significant nibble
            else
            {
                shifter_pos -= 1;
            }
            dataI2C[0] = shifter_pos;
            EEPROM_Write(dataI2C, 15);
        
        }
        */
    }
    int count = 0;
    while (mode == 1)
    {
        // Var to keep track of tuning steps
        
        // Inital output of user instructions
        if (count == 0)
        {
            UART_UartPutString("Small Chainring, Press SPACE to set position!!!\n\r");
            frontd.current_pos = QuadDec_ReadCounter();
            frontd.position = 0;
            count = 1;
        }
        
        // Poll for the SPACE key input to set front derailleur tuning paramteters. 
        char input = UART_UartGetChar();
        if (input == 0x20)
        {
            if (count == 1)
            {
                frontd.tuned_pos[0] = frontd.current_pos;
                frontd.position = 0;
                frontDerailToData(dataI2C, frontd);
                EEPROM_Write(dataI2C, 7);
                count = 2;
                sprintf(bufferUART, "Small Chainring Position Set: %d\n\r", frontd.tuned_pos[0]);
                UART_UartPutString(bufferUART);
                UART_UartPutString("Large Chainring, Press SPACE to set position\n\r");
                frontd.position = 1;
                
            }
            else if (count == 2)
            {
                frontd.tuned_pos[1] = frontd.current_pos;
                frontd.position = 1;
                frontDerailToData(dataI2C, frontd);
                EEPROM_Write(dataI2C, 7);
                count = 3;
                sprintf(bufferUART, "Large Chainring Position Set: %d\n\r", frontd.tuned_pos[1]);
                UART_UartPutString(bufferUART);
                UART_UartPutString("Tuning Complete, Press SPACE to finish\n\r");
                sprintf(bufferUART, "%d, %d, %d, %d\n\r", frontd.position, frontd.current_pos, frontd.tuned_pos[0], frontd.tuned_pos[1]);
                UART_UartPutString(bufferUART);
            }
            else if (count == 3)
            {
            }
        }
        
        if(shift_isr_status & (1 << 0))
        {
            front_motor_Write(FRONT_UP);
            CyDelay(100);
            front_motor_Write(STOP);
            frontd.current_pos = QuadDec_ReadCounter();
            shift_isr_status &= 0xC;
            sprintf(bufferUART, "%d, %d, %d, %d\n\r", frontd.position, frontd.current_pos, frontd.tuned_pos[0], frontd.tuned_pos[1]);
            UART_UartPutString(bufferUART);
        }
        if(shift_isr_status & (1 << 1))
        {
            front_motor_Write(FRONT_DN);
            CyDelay(100);
            front_motor_Write(STOP);
            frontd.current_pos = QuadDec_ReadCounter();
            shift_isr_status &= 0xC;
            sprintf(bufferUART, "%d, %d, %d, %d\n\r", frontd.position, frontd.current_pos, frontd.tuned_pos[0], frontd.tuned_pos[1]);
            UART_UartPutString(bufferUART);
        }
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
        shift_isr_status &= 0xC;
        shift_isr_status |= (1 << 0);
    }
    else if (shifter_switches_INTSTAT & (2u << shifter_switches_SHIFT))
    {
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

frntder frontDerailInit(uint8* dataI2C)
{
    frntder fd;
    fd.position = dataI2C[0];
    fd.current_pos = (uint16)(dataI2C[1]<<8)|(dataI2C[2]);
    fd.tuned_pos[0] = (uint16)(dataI2C[3]<<8)|(dataI2C[4]);
    fd.tuned_pos[1] = (uint16)(dataI2C[5]<<8)|(dataI2C[6]);
    fd.eep_offset = 0;
    return fd;
}

void frontDerailToData(uint8* dataI2C, frntder fd)
{
    dataI2C[0] = fd.position;
    dataI2C[1] = (uint8)(fd.current_pos >> 8);
    dataI2C[2] = (uint8)(fd.current_pos & 0xff);
    dataI2C[3] = (uint8)(fd.tuned_pos[0] >> 8);
    dataI2C[4] = (uint8)(fd.tuned_pos[0] & 0xff);
    dataI2C[5] = (uint8)(fd.tuned_pos[1] >> 8);
    dataI2C[6] = (uint8)(fd.tuned_pos[1] & 0xff);
}

/* This function writes to an EEPROM. The offset pointer to the EEPROM is fixed by this function to 0x00,0x00. 
To change the write location, OffsetAddress values can be changed*/
void EEPROM_Write(uint8* Data, int numBytes)
{
    
    int offsetAddress[2]={0,0};
    int byteCnt_w = 15;
    uint8 writeData[15] = {0x00,0x00};
    int i;

    for(i=0;i<13;i++)
    {
     writeData[i+2] = Data[i];
    }

	I2C_I2CMasterWriteBuf(ADDRESS1, (uint8 *) writeData, byteCnt_w,  I2C_I2C_MODE_COMPLETE_XFER );
	for(;;)
	{
		if(0u != (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT))
		{
			break;
		}
					
	}	
}

/* This fuction reads data from the EEPROM from offset pointer location 0x00, 0x00. The value of 
offsetAddress1 can be changed to read from  a different location of the EEPROM */
void EEPROM_Read(uint8* Data, int numBytes)
{

    int offsetAddress[2]={0,0};
	int byteCnt_w = 2;
	I2C_I2CMasterWriteBuf(ADDRESS1, (uint8 *) offsetAddress, byteCnt_w, I2C_I2C_MODE_NO_STOP );
	for(;;)
	{
		if(0u != (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT))
		{
			break;
		}		
	}	
	numBytes = 13;
	I2C_I2CMasterReadBuf(ADDRESS1, (uint8 *) Data, numBytes, I2C_I2C_MODE_REPEAT_START ); 

	for(;;)
	{
		if(0u != (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_RD_CMPLT))
		{
			break;
		}
	}
}


/* [] END OF FILE */
