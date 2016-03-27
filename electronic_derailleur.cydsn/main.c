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
#define FRONT_UP 0x02
#define FRONT_DN 0x01
#define REAR_UP 0x01
#define REAR_DN 0x02

#define FRONT_UPPER_LIMIT 1500u
#define FRONT_LOWER_LIMIT 1500u

#define ADDRESS1 0x50      /* Address of the EEPROM slave */

int shift_isr_status = 0x0u;

/* Initialize the interrupt fuction prototype */
CY_ISR_PROTO(Shifter_ISR); 


void EEPROM_Write(uint8* Data, int numBytes);
void EEPROM_Read(uint8* Data, int numBytes);

char bufferUART[20] = {};

typedef struct FrontDerailleur
{   
    uint16 position; // Binary position, 0 for small, 1 for large chainring.
    uint16 current_pos; // Quad encoder value for current/last saved position.
    uint16 tuned_pos[2]; // Tuned quad encoder values for small and large chainring positions
    uint16 eep_offset; // FD EEPROM offset save address.
} frntder;

frntder frontDerailInit(uint8* dataI2C);

int main()
{
    uint8 dataI2C[] = {};
    /* Initial front motor position variable */
    uint32 ref_front_count = 0;
   
    
    /*
    uint8 temp[] = {};
    temp[0] = 0;
    temp[1] = (uint8)((int)32767 >> 8);
    temp[2] = (uint8)((int)32767 & 0xff);
    temp[3] = (uint8)((int)(32767-1000) >> 8);
    temp[4] = (uint8)((int)(32767-1000) & 0xff);
    temp[5] = (uint8)((int)(32767+1000) >> 8);
    temp[6] = (uint8)((int)(32767+1000) & 0xff);
    */
    
    //int writeFlag = 0;
    uint8 shifter_pos;
    
    /* UART buffer to store output to console */
    char bufferUART[20] = {};
    

    /* Set up interrupt vector address for function */
    //shifter_isr_StartEx(Shifter_ISR);
    
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
    ref_front_count = QuadDec_ReadCounter();
    
        
    ADC_SAR_SEQ_Start();
    ADC_SAR_SEQ_StartConvert();
    
    //EEPROM_Write(temp, 2);
    
    EEPROM_Read(dataI2C, 7);
    shifter_pos = dataI2C[0];
    int front_shift_pos = shifter_pos >> 4;
    int rear_shift_pos = shifter_pos & 0x0fu;
    
    frntder frontd;
    frontd = frontDerailInit(dataI2C);

    //sprintf(bufferUART, "Inital Shifter Position - F: %d, R: %d\n\r", front_shift_pos, rear_shift_pos);
    //UART_UartPutString(bufferUART);
    //sprintf(bufferUART, "bin: %d, cur: %d, tun0: %d, tun1: %d\n\r", fd.position, (uint16)(dataI2C[1]<<8)|(dataI2C[2]), fd.tuned_pos[0], fd.tuned_pos[1]);
    
    for(;;)
    {

        /* Check for the shifter interrupt status variable for shift commands */
        // Front derailleur shift down
        if(shift_isr_status & (1 << 0))
        {
            
            UART_UartPutString("FRONT UP\n\r");
            if (front_shift_pos >= 1)
            {
                front_shift_pos = 1;
                shift_isr_status &= 0xC;
            }
            else
            {
                
                front_motor_Write(FRONT_UP);
                if(QuadDec_ReadCounter() >= ref_front_count + FRONT_UPPER_LIMIT)
                {  
                    front_motor_Write(STOP);
                    shift_isr_status &= 0xC;
                    front_shift_pos ++;
                
                // Check for maximum front derialluer postion
                //if (front_shift_pos >= 1)
                //{
                //    front_shift_pos = 1;
                //}
                // Otherwise increment the shifter position
                //else
                //{
                //    front_shift_pos ++;
                //}
                }
          
            }
 
            //dataI2C[0] = ((front_shift_pos & 0x0fu) << 4)  | rear_shift_pos;
            dataI2C[0] = 0;
            EEPROM_Write(dataI2C, 3);
            EEPROM_Read(dataI2C, 3);
            shifter_pos = dataI2C[0];
            front_shift_pos = shifter_pos >> 4;
            rear_shift_pos = shifter_pos & 0x0fu;
            sprintf(bufferUART, "Inital Shifter Position - F: %d, R: %d\n\r", front_shift_pos, rear_shift_pos);
            UART_UartPutString(bufferUART);
            
            
                
            sprintf(bufferUART, "%lu, %lu\n\r", QuadDec_ReadCounter(), ref_front_count + FRONT_UPPER_LIMIT);
            UART_UartPutString(bufferUART);
                
            //front_motor_Write(STOP);
            //shift_isr_status &= 0xC;
            

        
        } 
        // Front derailleur shift up
        else if (shift_isr_status & (1 << 1))
        {
            UART_UartPutString("FRONT DN\n\r");
            front_motor_Write(FRONT_DN);
            if (QuadDec_ReadCounter() <= ref_front_count - FRONT_LOWER_LIMIT)
            {
                front_motor_Write(STOP);
                shift_isr_status &= 0xC;
                
                // Check most significant nibble for edge case or counter overflow
                if(front_shift_pos == 0)
                {
                    front_shift_pos = 0;
                }
                else if (front_shift_pos >= 2)
                {
                    front_shift_pos = 0;
                }
                // Otherwise decrement the shifter position
                else
                {
                    front_shift_pos --;
                }
                dataI2C[0] = ((front_shift_pos & 0x0fu) << 4)  | rear_shift_pos;
                EEPROM_Write(dataI2C, 3);
                
                EEPROM_Read(dataI2C, 3);
                shifter_pos = dataI2C[0];
                front_shift_pos = shifter_pos >> 4;
                rear_shift_pos = shifter_pos & 0x0fu;
                
                sprintf(bufferUART, "Inital Shifter Position - F: %d, R: %d\n\r", front_shift_pos, rear_shift_pos);
                UART_UartPutString(bufferUART);
                sprintf(bufferUART, "%lu, %lu\n\r", QuadDec_ReadCounter(), ref_front_count - FRONT_UPPER_LIMIT);
                UART_UartPutString(bufferUART);
            } 
        }
        
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
    //sprintf(bufferUART, "Write - F: %d, R: %d\n\r", writeData[0], writeData[1]);
    //UART_UartPutString(bufferUART);
	
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
    //sprintf(bufferUART, "REad - F: %d, R: %d\n\r", Data[0], Data[1]);
    //UART_UartPutString(bufferUART);
}


/* [] END OF FILE */
