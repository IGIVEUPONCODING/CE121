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
#include "project.h"

/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
#define OUT_EP_NUM    (2u)

/* Defines for DMA1 */
#define DMA1_BYTES_PER_BURST 1
#define DMA1_REQUEST_PER_BURST 1
#define DMA1_SRC_BASE (CYDEV_PERIPH_BASE)
#define DMA1_DST_BASE (CYDEV_SRAM_BASE)    
/* Defines for DMA2 */
#define DMA2_BYTES_PER_BURST 1
#define DMA2_REQUEST_PER_BURST 1
#define DMA2_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA2_DST_BASE (CYDEV_PERIPH_BASE)

/*constants and variables*/
#define TRUE 1
#define FALSE 0
#define arraySize 64
uint8 DMA_Flag = FALSE;
int channel = 1;
int channel_2 = 1;

/*interrupts*/
CY_ISR(IN){
//    DMA_Flag = TRUE;
    channel = !channel;
}

CY_ISR(OUT){
    channel_2 = !channel_2;
}
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    MyADC_Start();
    MyADC_StartConvert();
    MyVDAC_Start();
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    DMADone1_StartEx(IN);
    DMADone2_StartEx(OUT);
    uint8 albert1 [arraySize];
    uint8 albert2 [arraySize];
    uint8 albert3 [arraySize];
    uint8 albert4 [arraySize];
    uint16 length = 0;  
    /* Variable declarations for DMA1 */
    /* Move these variable declarations to the top of the function */
    uint8 DMA1_Chan;
    uint8 DMA1_TD[2];
    /* Variable declarations for DMA2 */
    /* Move these variable declarations to the top of the function */
    uint8 DMA2_Chan;
    uint8 DMA2_TD[2];
    /* Wait until device is enumerated by host. */
    while (0u == USBFS_GetConfiguration())
    {
    }
    /* Enable OUT endpoint to receive data from host. */
    USBFS_EnableOutEP(OUT_EP_NUM);
    /* DMA Configuration for DMA1 */
    DMA1_Chan = DMA1_DmaInitialize(DMA1_BYTES_PER_BURST, DMA1_REQUEST_PER_BURST, 
        HI16(DMA1_SRC_BASE), HI16(DMA1_DST_BASE));
    DMA1_TD[0] = CyDmaTdAllocate();
    DMA1_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA1_TD[0], 64, DMA1_TD[1], DMA1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(DMA1_TD[1], 64, DMA1_TD[0], DMA1__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(DMA1_TD[0], LO16((uint32)MyADC_DEC_SAMP_PTR), LO16((uint32)&albert1));
    CyDmaTdSetAddress(DMA1_TD[1], LO16((uint32)MyADC_DEC_SAMP_PTR), LO16((uint32)&albert2));
    CyDmaChSetInitialTd(DMA1_Chan, DMA1_TD[0]);
    CyDmaChEnable(DMA1_Chan, 1);

    /* DMA Configuration for DMA2 */
    DMA2_Chan = DMA2_DmaInitialize(DMA2_BYTES_PER_BURST, DMA2_REQUEST_PER_BURST, 
        HI16(DMA2_SRC_BASE), HI16(DMA2_DST_BASE));
    DMA2_TD[0] = CyDmaTdAllocate();
    DMA2_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(DMA2_TD[0], 64, DMA2_TD[1], DMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetConfiguration(DMA2_TD[1], 64, DMA2_TD[0], DMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR);
    CyDmaTdSetAddress(DMA2_TD[0], LO16((uint32)&albert3), LO16((uint32)MyVDAC_Data_PTR));
    CyDmaTdSetAddress(DMA2_TD[1], LO16((uint32)&albert4), LO16((uint32)MyVDAC_Data_PTR));
    CyDmaChSetInitialTd(DMA2_Chan, DMA2_TD[0]);
    CyDmaChEnable(DMA2_Chan, 1);

    for(;;)
    {
        /* Check if configuration is changed. */
        if (0u != USBFS_IsConfigurationChanged())
        {
            /* Re-enable endpoint when device is configured. */
            if (0u != USBFS_GetConfiguration())
            {
                /* Enable OUT endpoint to receive data from host. */
                USBFS_EnableOutEP(OUT_EP_NUM);
            }
        }
        uint8 errorcheck = channel;
        CyPins_SetPin(LED_0);
        /* Wait until IN buffer becomes empty (host has read data). */
        while (USBFS_IN_BUFFER_EMPTY != USBFS_GetEPState(IN_EP_NUM))
        {
            //If the channel switched before the buffer is empty
            //something went wrong
            if(errorcheck != channel){
                CyPins_ClearPin(LED_0);
            }
        }
        if(channel == 0){
            USBFS_LoadInEP(IN_EP_NUM, albert1, arraySize);
        }
        if(channel == 1){
            USBFS_LoadInEP(IN_EP_NUM, albert2, arraySize);
        }
        USBFS_EnableOutEP(OUT_EP_NUM);
        if(USBFS_OUT_BUFFER_EMPTY == USBFS_GetEPState(OUT_EP_NUM)){
            /* Read number of received data bytes. */
            length = USBFS_GetEPCount(OUT_EP_NUM);
            if(channel_2 == 0){
                USBFS_ReadOutEP(OUT_EP_NUM, albert3, length);
            }
            if(channel_2 == 1){
                USBFS_ReadOutEP(OUT_EP_NUM, albert4, length);
            }
        }
    }
}

/* [] END OF FILE */
