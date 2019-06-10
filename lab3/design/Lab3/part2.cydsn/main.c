/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu
 * 1517718
 * Lab3 Part2
 * Transit/Receive Using ISRs
 * 
 *
 * ========================================
*/
#include "project.h"

#define arraySize 4096
#define TRUE 1
#define FALSE 0
int i;
uint8 transmit[arraySize];
uint8 receive[arraySize];
uint16 time = 0;
uint16 TxNum = 0;
uint16 RxNum = 0;
uint16 error = 0;
uint16 tx = 0;
uint8 transmitFlag = FALSE;
uint8 receiveFlag = FALSE;
uint16 rec = 0;

/*In both ISR just set up the flag
 *and increase the couter to keep
 *track of the number entered ISR.*/
CY_ISR(TxISR)
{
    if(tx < 4095)
    {
    transmitFlag = TRUE;
    TxNum++;
    }
}
CY_ISR(RxISR)
{ 
    receiveFlag = TRUE;
    RxNum++;
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_Start();
    LCD_Start();
    isr_1_StartEx(TxISR);
    isr_2_StartEx(RxISR);
    for (i = 0; i < arraySize; i++)
    {
        transmit[i] = i%256;
    }
    for (i = 0; i < arraySize; i++)
    {
        receive[i] = 0;
    }
        
    for (i = 0;i < arraySize; i++)
    {       
        if (transmitFlag == TRUE)
        {
            //reet the flag
            transmitFlag = FALSE;
            //transmitting 4 bytes at a time to optimize the times entering ISR.
            UART_PutChar(transmit[tx]);
            tx++;
            UART_PutChar(transmit[tx]);
            tx++;
            UART_PutChar(transmit[tx]);
            tx++;
            UART_PutChar(transmit[tx]);
            tx++;
        }
        while (receiveFlag == FALSE){}
        if (receiveFlag == TRUE)
        {   
            receiveFlag = FALSE;    
            receive[i] = UART_GetChar();        
        }
    }
            
    //check for mismatches
    for (i = 0; i < arraySize; i++)
    {
        if (transmit[i] != receive[i])
        {
          error++;
        }
    } 
    LCD_Position(0u,0u);
    LCD_PrintString("E:");
    LCD_Position(0u, 2u);
    LCD_PrintNumber(error);
    LCD_Position(1,0);
    LCD_PrintString("T:");
    LCD_Position(1,2);
    LCD_PrintNumber(TxNum);
    LCD_Position(1,9);
    LCD_PrintString("R:");
    LCD_Position(1,11);
    LCD_PrintNumber(RxNum);
    
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
