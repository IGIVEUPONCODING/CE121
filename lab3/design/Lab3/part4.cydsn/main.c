/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu 1517718
 * lab3 Part4
 * Data Transfer Betweem Two UARTs
 * 
 * 
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
        /*make sure the data stays with the range of 0 to 0xff*/
        tx %= arraySize;
    }
}
CY_ISR(RxISR1)
{
    rxStatus = PrimaryUART_ReadRxStatus();
    while((PrimaryUART_ReadRxStatus() & PrimaryUART_RX_STS_FIFO_NOTEMPTY) && (rx <arraySize))
    {
        receive[rx] = PrimaryUART_ReadRxData();
        if(transmit[rx] != receive[rx2])
        {
            error++;
        }
        if((rxStatus & PrimaryUART_RX_STS_PAR_ERROR) || (rxStatus & PrimaryUART_RX_STS_STOP_ERROR) || (rxStatus & PrimaryUART_RX_STS_OVERRUN))
        {
            error++;
        }
        /*if error > cur_error, it means that the wire has been plugged in again*/
        if(error>cur_error)
        {
            cur_error = error;
        }else{
            rx2++; 
            rx2 %= arraySize;
        }
        rx++;
        rx %= arraySize;
        rxStatus = PrimaryUART_ReadRxStatus();
        transcount ++;
    }
}
CY_ISR(RxISR2)
{
    if((SecondaryUART_ReadRxStatus() & SecondaryUART_RX_STS_FIFO_NOTEMPTY))
    {
        SecondaryUART_PutChar(SecondaryUART_ReadRxData());
    }
}

CY_ISR(TimeISR)
{
    bytesRate = transcount;
    /*reset the transcount so that it can be updated every second*/
    transcount = 0;
    Timer_1_ReadStatusRegister();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    tx_isr_1_StartEx(TxISR1);
    rx_isr_1_StartEx(RxISR1);
    rx_isr_2_StartEx(RxISR2);
    isr_1_StartEx(TimeISR);
    LCD_Start();
    Timer_1_Start();
    
    //initialize transmit and receive arrays
    for (i = 0; i<arraySize; i++)
    {
        transmit[i] = i%256;
        receive[i] = 0;
    }    
    
    SecondaryUART_Start();
    PrimaryUART_Start(); 
    
    for(;;)
    {
        //Displaying contents in Rx and Tx
        //As well as errors
        //CyDelay(500);
        LCD_ClearDisplay();
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
