/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu
 * Lab3 Part1
 *
 *
 * 
 *
 * ========================================
*/
#include "project.h"


#define DATA 0xa5
#define arraySize 4096

uint8 transmit[arraySize];
uint8 receive[arraySize];
uint8 txStatus = 0;
uint8 rxStatus = 0;
uint32 time;
int i;
uint16 error = 0;
int main(void)
{    
    /* Enable global interrupts.*/
    CyGlobalIntEnable; 
    UART_Start();
    LCD_Start();
    LCD_ClearDisplay();
    //initialize two arrays.
    for (i = 0; i < arraySize; i++)
    {
        transmit[i] = i%256;
    }
    for (i = 0; i < arraySize; i++)
    {
        receive[i] = 0;
    }
    

    Timer_1_Start();
    for (i = 0; i < arraySize; i++){
        //Read UART status.
        txStatus = UART_ReadTxStatus();
        //check if the FIFO is full.
        //If it's full, stay in the loop till it has rooms.
        while(txStatus & UART_TX_STS_FIFO_FULL)
        {
            txStatus = UART_ReadTxStatus();
        }
        if(txStatus & UART_TX_STS_FIFO_NOT_FULL)
        {
            UART_WriteTxData(transmit[i]);
        }
        
        while(~(UART_ReadTxStatus()) & UART_TX_STS_COMPLETE){/*Wait for the transmit to complete and then read from Rx.*/}
        
        rxStatus = UART_ReadRxStatus();
        if(rxStatus & UART_RX_STS_FIFO_NOTEMPTY)
        {
            receive[i] = UART_ReadRxData();
        }
    }
    Timer_1_Stop();
    time = (4294967296-Timer_1_ReadCounter())/24;
    //time = ((((float)(65535-Timer_1_ReadCounter()))/24000000)*1000000);
        //check for mismatches
    for (i = 0; i < arraySize; i++)
    {
        if (transmit[i] != receive[i])
        {
          error++;
        }
    }       
    LCD_Position(0u,0u);
    LCD_PrintString("Error: ");
    LCD_Position(0u,6u);
    LCD_PrintNumber(error);
    LCD_Position(1,0);
    LCD_PrintString("Time: ");
    LCD_Position(1,5);
    LCD_PrintNumber(time);
    LCD_Position(1,10);
    LCD_PrintString("us");
    for(;;)
    {  
       //UART_PutChar(DATA);
       //CyDelay(10);
        /* Place your application code here. */
    }   

}

/* [] END OF FILE */
