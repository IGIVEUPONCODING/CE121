/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu 1517718
 * lab3 Part5
 * Receiver Clock Tolerance
 * The code is basically the same to part4
 * delete some unused function
 *
 * ========================================
*/
#include "project.h"
#define TRUE 1
#define FALSE 0
#define arraySize 256
int i;
int tx = 0;
int rx = 0;
int cur_error = 0;
int rx2 = 0;
int transcount = 0;
int bytesRate = 0;
uint8 rxStatus;
uint16 error = 0;
uint8 transmit[arraySize];
uint8 receive[arraySize];

CY_ISR(TxISR1)
{
    while((PrimaryUART_ReadTxStatus() & PrimaryUART_TX_STS_FIFO_EMPTY) && (tx < arraySize))
    {
        PrimaryUART_PutChar(transmit[tx]);
        tx++;
        tx %= arraySize;
    }
}
CY_ISR(RxISR2)
{
    rxStatus = SecondaryUART_ReadRxStatus();
    while((SecondaryUART_ReadRxStatus() & SecondaryUART_RX_STS_FIFO_NOTEMPTY) && (rx <arraySize))
    {
        receive[rx] = SecondaryUART_ReadRxData();
        if(transmit[rx] != receive[rx])
        {
            error++;
        }
        rx++;
        rx %= arraySize;
        rxStatus = SecondaryUART_ReadRxStatus();
        transcount ++;
    }
}

CY_ISR(TimeISR)
{
    bytesRate = transcount;
    transcount = 0;
    Timer_1_ReadStatusRegister();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    tx_isr_1_StartEx(TxISR1);
    rx_isr_2_StartEx(RxISR2);
    isr_1_StartEx(TimeISR);
    Clock_1_Start();
    //Clock_1_SetDivider(26);
    LCD_Start();
    Timer_1_Start();
    
    //initialize transmit and receive arrays
    for (i = 0; i<arraySize; i++)
    {
        transmit[i] = i%256;
        receive[i] = 0;
    }   
    
    PrimaryUART_Start(); 
    SecondaryUART_Start();
    
    
    for(;;)
    {
        //Displaying contents in Rx and Tx
        //As well as errors
        //CyDelay(500);
        //LCD_ClearDisplay();
        LCD_Position(0u,0u);
        LCD_PrintString("Miss:");
        LCD_Position(0u,6u);
        LCD_PrintNumber(error);
        LCD_Position(1,0);
        LCD_PrintNumber(bytesRate);
        LCD_Position(1,8);
        LCD_PrintString("bytes/s");
        
    }
}

/* [] END OF FILE */
