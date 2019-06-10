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
#define IN2_EP_NUM    (2u)

/* Defines for MyDMA */
#define MyDMA_BYTES_PER_BURST 1
#define MyDMA_REQUEST_PER_BURST 1
#define MyDMA_SRC_BASE (CYDEV_PERIPH_BASE)
#define MyDMA_DST_BASE (CYDEV_SRAM_BASE)

/* Defines for MyDMA2 */
#define MyDMA2_BYTES_PER_BURST 1
#define MyDMA2_REQUEST_PER_BURST 1
#define MyDMA2_SRC_BASE (CYDEV_PERIPH_BASE)
#define MyDMA2_DST_BASE (CYDEV_SRAM_BASE)

/*constants and variables*/
#define TRUE 1
#define FALSE 0
#define arraySize 64
#define size 255
int channel = TRUE;
int channel_2 = TRUE;
/*I2C constant*/
#define RD_BUFFER_SIZE        (2u)
#define WR_BUFFER_SIZE        (1u)

/*varibles and constants for I2C*/
uint8 read_buffer[RD_BUFFER_SIZE] = {0};
uint8 write_buffer[WR_BUFFER_SIZE] = {0};
uint8 readbyteCount = 0u;    
uint8 command_reg = 0; // Command register to capture writes from I2C
int32 Pot1_Read = 0;
int32 Pot2_Read = 0;

/*interrupts*/
CY_ISR(CH1){
    channel = !channel;
}

CY_ISR(CH2){
    channel_2 = !channel_2;
}

CY_ISR(timer){
    /* Check if the command register has been written */
    if(I2CS_SlaveStatus() & I2CS_SSTAT_WR_CMPLT)
    {
    // Copy command into command register
    //command_reg = write_buffer[0];
    //Take command from Pi to alternate sample rate. 
    //Clock_SetDivider(24000/write_buffer[0]);
    // Clear status
    I2CS_SlaveClearWriteStatus();
    I2CS_SlaveClearWriteBuf();   
    }   
    /* Check if the slave buffer has been read */
    if(I2CS_SlaveStatus() & I2CS_SSTAT_RD_CMPLT)
    {
        readbyteCount = I2CS_SlaveGetReadBufSize();
        /* If both bytes of the read buffer have been read */
        if(readbyteCount == RD_BUFFER_SIZE)
        {
            // Clear read status
            I2CS_SlaveClearReadStatus();
            I2CS_SlaveClearReadBuf();  
            read_buffer[0] = Pot1_Read;
            read_buffer[1] = Pot2_Read;      
        }
    }
    Timer_1_ReadStatusRegister();
}


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    ADC_SAR_1_Start();
    ADC_SAR_2_Start();
    ADC_Pot_Start();
    AMux_1_Start();
    ADC_Pot_StartConvert();
    ADC_SAR_1_StartConvert();
    ADC_SAR_2_StartConvert();
    DMADone1_StartEx(CH1);
    DMADone2_StartEx(CH2);
    timerISR_StartEx(timer);
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    

    // Set up slave write data buffer
    I2CS_SlaveInitWriteBuf((uint8 *) write_buffer, WR_BUFFER_SIZE);   
    // Set up slave read data buffer
    I2CS_SlaveInitReadBuf((uint8 *) read_buffer, RD_BUFFER_SIZE);
    // Start I2C slave component
    I2CS_Start();
    Timer_1_Start();
    
    /*Ping Pong Buffers*/
    uint8 albert1 [arraySize];
    uint8 albert2 [arraySize];
    uint8 albert3 [arraySize];
    uint8 albert4 [arraySize];
    
    /* Variable declarations for MyDMA */
    uint8 MyDMA_Chan;
    uint8 MyDMA_TD[2];
    /* Variable declarations for MyDMA2 */
    uint8 MyDMA2_Chan;
    uint8 MyDMA2_TD[2];
    /* DMA Configuration for MyDMA */
    MyDMA_Chan = MyDMA_DmaInitialize(MyDMA_BYTES_PER_BURST, MyDMA_REQUEST_PER_BURST, 
        HI16(MyDMA_SRC_BASE), HI16(MyDMA_DST_BASE));
    MyDMA_TD[0] = CyDmaTdAllocate();
    MyDMA_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(MyDMA_TD[0], 64, MyDMA_TD[1], MyDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(MyDMA_TD[1], 64, MyDMA_TD[0], MyDMA__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(MyDMA_TD[0], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)&albert1));
    CyDmaTdSetAddress(MyDMA_TD[1], LO16((uint32)ADC_SAR_1_SAR_WRK0_PTR), LO16((uint32)&albert2));
    CyDmaChSetInitialTd(MyDMA_Chan, MyDMA_TD[0]);
    CyDmaChEnable(MyDMA_Chan, 1);    
    /* DMA Configuration for MyDMA2 */
    MyDMA2_Chan = MyDMA2_DmaInitialize(MyDMA2_BYTES_PER_BURST, MyDMA2_REQUEST_PER_BURST, 
        HI16(MyDMA2_SRC_BASE), HI16(MyDMA2_DST_BASE));
    MyDMA2_TD[0] = CyDmaTdAllocate();
    MyDMA2_TD[1] = CyDmaTdAllocate();
    CyDmaTdSetConfiguration(MyDMA2_TD[0], 64, MyDMA2_TD[1], MyDMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetConfiguration(MyDMA2_TD[1], 64, MyDMA2_TD[0], MyDMA2__TD_TERMOUT_EN | CY_DMA_TD_INC_DST_ADR);
    CyDmaTdSetAddress(MyDMA2_TD[0], LO16((uint32)ADC_SAR_2_SAR_WRK0_PTR), LO16((uint32)&albert3));
    CyDmaTdSetAddress(MyDMA2_TD[1], LO16((uint32)ADC_SAR_2_SAR_WRK0_PTR), LO16((uint32)&albert4));
    CyDmaChSetInitialTd(MyDMA2_Chan, MyDMA2_TD[0]);
    CyDmaChEnable(MyDMA2_Chan, 1);
    for(;;)
    {       
        /* Wait until IN buffer becomes empty (host has read data). */
        while (USBFS_IN_BUFFER_EMPTY != USBFS_GetEPState(IN_EP_NUM)){}
        while (USBFS_IN_BUFFER_EMPTY != USBFS_GetEPState(IN2_EP_NUM)){}
        /*enable both IN endpoints to transfer data*/
        if(channel == FALSE){
            USBFS_LoadInEP(IN_EP_NUM, albert1, arraySize);
        }
        if(channel == TRUE){
            USBFS_LoadInEP(IN_EP_NUM, albert2, arraySize);
        }
        if(channel_2 == FALSE){
            USBFS_LoadInEP(IN2_EP_NUM, albert3, arraySize);
        }
        if(channel_2 == TRUE){
            USBFS_LoadInEP(IN2_EP_NUM, albert4, arraySize);
        }
        //Select each channel
        AMux_1_FastSelect(0);
        Pot1_Read = ADC_Pot_Read16();
        if(Pot1_Read > size)
        {
            Pot1_Read = size;
        }
        if(Pot1_Read < 0)
        {
            Pot1_Read = 0;
        }
        AMux_1_FastSelect(1);
        Pot2_Read = ADC_Pot_Read16();
        if(Pot2_Read > size)
        {
            Pot2_Read = size;
        }
        if(Pot2_Read < 0)
        {
            Pot2_Read = 0;
        }
    }
}

/* [] END OF FILE */
