/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu 1517718
 * Part2
 * Memory to Memory transfer
 * some codes copied from the example 
 * 
 *
 * ========================================
*/
#include "project.h"

#define BLOCKSIZE 16384
#define Height 4096

/*Define both arrays*/
uint8 sourceArray[BLOCKSIZE];
uint8 destinationArray[BLOCKSIZE];

/* Flag to indicate DMA transfer is complete */
volatile uint8 flag_DMADone = 0;
int mismatch = 0;
int i, j, k;
float time = 0;
int row = 1;
int column = 0;


CY_ISR(MyISR){
    Timer_Stop();
    /*Convert to microsecond*/
    time = ((((float)(65535-Timer_ReadCounter())) / 24000000)*1000000);
}


/* Defines for DMA */
#define DMA_BYTES_PER_BURST 4
#define DMA_REQUEST_PER_BURST 0
#define DMA_SRC_BASE (CYDEV_SRAM_BASE)
#define DMA_DST_BASE (CYDEV_SRAM_BASE)


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    LCD_Start();
    LCD_ClearDisplay();
    DMADone_StartEx(MyISR);
    /*set up the source & dest array according to manual*/
    for (i = 0; i < BLOCKSIZE; i++)
    {
        sourceArray[i] = i % 256;
    }
     for (j = 0; j < BLOCKSIZE; j++)
    {
        destinationArray[j] = 0;
    }

    /* DMA method*/
    /* Variable declarations for DMA */
    /* Move these variable declarations to the top of the function */
    uint8 DMA_Chan;
    uint8 DMA_TD[5];

    /* DMA Configuration for DMA */
    DMA_Chan = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, 
        HI16(DMA_SRC_BASE), HI16(DMA_DST_BASE));
    DMA_TD[0] = CyDmaTdAllocate();
    DMA_TD[1] = CyDmaTdAllocate();
    DMA_TD[2] = CyDmaTdAllocate();
    DMA_TD[3] = CyDmaTdAllocate();
    DMA_TD[4] = CyDmaTdAllocate();
    
    CyDmaTdSetConfiguration(DMA_TD[0], 4092, DMA_TD[1], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[1], 4092, DMA_TD[2], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[2], 4092, DMA_TD[3], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[3], 4092, DMA_TD[4], CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR | CY_DMA_TD_AUTO_EXEC_NEXT);
    CyDmaTdSetConfiguration(DMA_TD[4], 16, CY_DMA_DISABLE_TD, CY_DMA_TD_SWAP_EN | CY_DMA_TD_SWAP_SIZE4 | DMA__TD_TERMOUT_EN | CY_DMA_TD_INC_SRC_ADR | CY_DMA_TD_INC_DST_ADR);
    
    CyDmaTdSetAddress(DMA_TD[0], LO16((uint32)sourceArray), LO16((uint32)destinationArray));
    CyDmaTdSetAddress(DMA_TD[1], LO16((uint32)sourceArray+4092), LO16((uint32)destinationArray+4092));
    CyDmaTdSetAddress(DMA_TD[2], LO16((uint32)sourceArray+8184), LO16((uint32)destinationArray+8184));
    CyDmaTdSetAddress(DMA_TD[3], LO16((uint32)sourceArray+12276), LO16((uint32)destinationArray+12276));
    CyDmaTdSetAddress(DMA_TD[4], LO16((uint32)sourceArray+16368), LO16((uint32)destinationArray+16368));
    CyDmaChSetInitialTd(DMA_Chan, DMA_TD[0]);
    
    CyDmaChEnable(DMA_Chan, 1);
//    Timer_Start();
//    CyDmaChSetRequest(DMA_Chan, CPU_REQ);
    
    


    /*compare the mismatches when transfer is complete*/
    //while (flag_DMADone != 1){/*while the flag is not 1, do nothing*/}
    


    /*Software Control Method*/
    Timer_Start();
    for (k = 0; k<BLOCKSIZE; k++)
    {
        destinationArray[k] = sourceArray[(row*4)-column-1];
        column ++;
        if (column == 4)
        {
            column = 0;
            row ++;
        }
    }
    Timer_Stop();
    time = ((((float)(65535-Timer_ReadCounter())) / 24000000)*1000000);
    
    /*Comparing the mismatches*/
    row = 1;
    column = 0;
    for ( k = 0; k < BLOCKSIZE; k++)
    {
        /*Compare two members. eg. destarray[3] & sourcearray[0]*/
        if(destinationArray[k] != sourceArray[(row*4)-column-1])
        {
            mismatch ++;
        }
        column ++;
        /*reach the end, move to the next row, loop back to head*/
        if(column == 4)
        {
            column = 0;
            row ++;
        }
    }

    
                                  
    

    for(;;)
    {    
        LCD_Position(0u,0u);
        LCD_PrintString("Miss: ");
        LCD_Position(0u,7u);
        LCD_PrintNumber(mismatch);
        LCD_Position(1,0);
        LCD_PrintString("Time:");
        LCD_Position(1, 7);
        LCD_PrintNumber(time);
        LCD_Position(1, 11);
        LCD_PrintString("ms");
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
