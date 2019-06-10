/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu
 * Lab4 part2
 * Analog Loopback Through Raspi
 * This program send a converted digital signal
 * to Raspi and look the signal back and convert
 * to analog again.
 *
 * ========================================
*/
#include "project.h"
#include "cytypes.h"
/* Defines for MyDMA */
#define MyDMA_BYTES_PER_BURST 1
#define MyDMA_REQUEST_PER_BURST 1
#define MyDMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define MyDMA_DST_BASE (CYDEV_SRAM_BASE)

/* Defines for MyDMA2 */
#define MyDMA2_BYTES_PER_BURST 1
#define MyDMA2_REQUEST_PER_BURST 1
#define MyDMA2_SRC_BASE (CYDEV_SRAM_BASE)
#define MyDMA2_DST_BASE (CYDEV_PERIPH_BASE)
#define arraySize 64
#define TRUE 1
#define FALSE 0
volatile uint8 DMA_Flag = FALSE;
/* 0 indicates the first channel*/
int channel = 1;
int channel2 = 1;
int i = 0;
int k = 0;    
char albert1[arraySize];
char albert2[arraySize];
char albert3[arraySize];
char albert4[arraySize];  
uint8 MyDMA2_Chan;
CY_ISR(DMAISR){
    DMA_Flag = TRUE;
    /*toggel between two channels*/
    channel = !channel;
}

//ISR for DMA2 is going to run only when 
//Rx finish putting stuff in my array
//So it will be disabled in the interrupt
CY_ISR(DMA2ISR){
    CyDmaChDisable(MyDMA2_Chan);
}

CY_ISR(RxISR){
    
    while((MyUART_ReadRxStatus() & MyUART_RX_STS_FIFO_NOTEMPTY) &&  (k < arraySize)){
        if(channel2 == 0){
            albert4[k] = MyUART_ReadRxData();
        }
        if(channel2 == 1){
            albert3[k] = MyUART_ReadRxData();
        }
        k++;
        //When the buffer is filled, DMA will be reenabled and then the data will be transfer to DAC.
        if(k >= arraySize){
            k %= arraySize;
            CyDmaChEnable(MyDMA2_Chan, 1);
            channel2 = !channel2;
        }
    }   
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    MyADC_Start();
    MyADC_StartConvert();
    MyVDAC_Start();
    isr_1_StartEx(RxISR);
    DMADone_StartEx(DMAISR);
    DMADone2_StartEx(DMA2ISR);
    for (int j = 0; j < arraySize; j++){
        albert1[j] = 0;
        albert2[j] = 0;
        albert3[j] = 0;
        albert4[j] = 0;
    }
    MyUART_Start();
    /* Variable declarations for MyDMA */
    /* Move these variable declarations to the top of the function */
    uint8 MyDMA_Chan;
    uint8 MyDMA_TD[2];
    
    
    /* Variable declarations for MyDMA2 */
    /* Move these variable declarations to the top of the function */
    uint8 MyDMA2_TD[2];
    /* DMA Configuration for MyDMA */
    MyDMA_Chan = MyDMA_DmaInitialize(MyDMA_BYTES_PER_BURST, MyDMA_REQUEST_PER_BURST, 
        HI16(MyDMA_SRC_BASE), HI16(MyDMA_DST_BASE));
    MyDMA_TD[0] = CyDmaTdAllocate();
    MyDMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(MyDMA_TD[0], 64, MyDMA_TD[1], MyDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(MyDMA_TD[1], 64, MyDMA_TD[0], MyDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(MyDMA_TD[0], LO16((uint32)MyADC_DEC_SAMP_PTR), LO16((uint32)&albert1));
    CyDmaTdSetAddress(MyDMA_TD[1], LO16((uint32)MyADC_DEC_SAMP_PTR), LO16((uint32)&albert2));
    CyDmaChSetInitialTd(MyDMA_Chan, MyDMA_TD[0]);
    CyDmaChEnable(MyDMA_Chan, 1);
    /* DMA Configuration for MyDMA2 */
    MyDMA2_Chan = MyDMA2_DmaInitialize(MyDMA2_BYTES_PER_BURST, MyDMA2_REQUEST_PER_BURST, 
        HI16(MyDMA2_SRC_BASE), HI16(MyDMA2_DST_BASE));
    MyDMA2_TD[0] = CyDmaTdAllocate();
    MyDMA2_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(MyDMA2_TD[0], 64, MyDMA2_TD[1], MyDMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetConfiguration(MyDMA2_TD[1], 64, MyDMA2_TD[0], MyDMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetAddress(MyDMA2_TD[0], LO16((uint32)&albert3), LO16((uint32)MyVDAC_Data_PTR));
    CyDmaTdSetAddress(MyDMA2_TD[1], LO16((uint32)&albert4), LO16((uint32)MyVDAC_Data_PTR));
    CyDmaChSetInitialTd(MyDMA2_Chan, MyDMA2_TD[0]);
    
    for(;;)
    {
        while (DMA_Flag != TRUE) {/*wait for the DMA to filled up the array*/}
        DMA_Flag = FALSE;
        //error checking--if the FIFO is empty the LED will stay on.
        if(MyUART_ReadTxStatus() & MyUART_TX_STS_FIFO_EMPTY){
            CyPins_ClearPin(LED_Error_0);
        }
        for (i = 0; i< arraySize; i++){
            /*transmit data from the first buffer*/
            if(channel == 0){
                MyUART_PutChar(albert1[i]);
            }
            if(channel == 1){
                MyUART_PutChar(albert2[i]);
            }
        }
        
    }
}

/* [] END OF FILE */
