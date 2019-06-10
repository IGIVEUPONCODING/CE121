/* ========================================
 * Xingyu Zhou
 * xzhou45@ucsc.edu
 * 1517718
 * Part3: Hardware flow control
 *
 * 
 * 
 *
 * ========================================
*/
#include "project.h"

#define arraySize 4096
#define TRUE 1
#define FALSE 0
int i;
int tx = 0;
int rx = 0;
uint8 transmit[arraySize];
uint8 receive[arraySize];
uint8 RxError;
uint8 missmatch;
uint8 rxStatus;
uint8 transferDone = FALSE;

CY_ISR(TxISR)
{
    //ISR triggered on FIFO_EMPTY. FILL UP 4 BYTES AT A TIME
    while((UART_ReadTxStatus() & UART_TX_STS_FIFO_EMPTY) && (tx < arraySize))
    {
        UART_PutChar(transmit[tx]);
        tx++;
        UART_PutChar(transmit[tx]);
        tx++;
        UART_PutChar(transmit[tx]);
        tx++;
        UART_PutChar(transmit[tx]);
        tx++;
    }
}

CY_ISR(TimeISR)
{
    rxStatus = UART_ReadRxStatus();
    while((rxStatus & UART_RX_STS_FIFO_NOTEMPTY) && (rx < arraySize))
    {
        receive[rx] = UART_ReadRxData();
        if((rxStatus & UART_RX_STS_PAR_ERROR) || (rxStatus & UART_RX_STS_STOP_ERROR) || (rxStatus & UART_RX_STS_OVERRUN))
        {
            RxError++;
        }
        rx++;
        rxStatus = UART_ReadRxStatus();
    }
    Timer_1_ReadStatusRegister();
}
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_1_StartEx(TxISR);
    isr_2_StartEx(TimeISR);
    LCD_Start();
    
    for (i = 0; i < arraySize; i++)
    {
        transmit[i] = i%256;
    }
    for (i = 0; i < arraySize; i++)
    {
        receive[i] = 0;
    }
    UART_Start();
    Timer_1_Start();
    //CyDelay(5000);
    while(tx<arraySize){/*while the transmit is not over, wait and do nothing*/}
    for (i = 0; i < arraySize; i++)
    {
        if (transmit[i] != receive[i])
        {
            missmatch++;
        }
    } 
    LCD_Position(0u,0u);
    LCD_PrintString("Miss:");
    LCD_Position(0u, 5u);
    LCD_PrintNumber(missmatch);
    LCD_Position(1,0);
    LCD_PrintString("RxE:");
    LCD_Position(1,5);
    LCD_PrintNumber(RxError);
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
