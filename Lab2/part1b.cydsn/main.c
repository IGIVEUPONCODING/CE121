/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu
 * 1517718
 * Lab2 part1b
 * 
 * 
 *
 * ========================================
*/
#include "project.h"
#include <cytypes.h>

#define TABLE_LENGTH 128
#define DMA_BYTES_PER_BURST 1
#define DMA_REQUEST_PER_BURST 1
CYCODE const uint8 sineTable[TABLE_LENGTH * 2] = 
{
	128, 134, 140, 147, 153, 159, 165, 171,
	177, 183, 188, 194, 199, 204, 209, 214,
	218, 223, 227, 231, 234, 238, 241, 244,
	246, 248, 250, 252, 253, 254, 255, 255,
	255, 255, 255, 254, 253, 252, 250, 248,
	246, 244, 241, 238, 234, 231, 227, 223,
	218, 214, 209, 204, 199, 194, 188, 183, 
	177, 171, 165, 159, 153, 147, 140, 134, 
	128, 122, 115, 109, 103,  97,  91,  85,
	 79,  73,  68,  62,  57,  52,  47,  42,
	 37,  33,  29,  25,  22,  18,  15,  12,
	 10,   7,   6,   4,   2,   1,   1,   0,
	  0,   0,   1,   1,   2,   4,   6,   7,
	 10,  12,  15,  18,  22,  25,  29,  33,
	 37,  42,  47,  52,  57,  62,  68,  73,
	 79,  85,  91,  97, 103, 109, 115, 122,
    128, 134, 140, 147, 153, 159, 165, 171,
	177, 183, 188, 194, 199, 204, 209, 214,
	218, 223, 227, 231, 234, 238, 241, 244,
	246, 248, 250, 252, 253, 254, 255, 255,
	255, 255, 255, 254, 253, 252, 250, 248,
	246, 244, 241, 238, 234, 231, 227, 223,
	218, 214, 209, 204, 199, 194, 188, 183, 
	177, 171, 165, 159, 153, 147, 140, 134, 
	128, 122, 115, 109, 103,  97,  91,  85,
	 79,  73,  68,  62,  57,  52,  47,  42,
	 37,  33,  29,  25,  22,  18,  15,  12,
	 10,   7,   6,   4,   2,   1,   1,   0,
	  0,   0,   1,   1,   2,   4,   6,   7,
	 10,  12,  15,  18,  22,  25,  29,  33,
	 37,  42,  47,  52,  57,  62,  68,  73,
	 79,  85,  91,  97, 103, 109, 115, 122
};
uint8 sineSRAM [TABLE_LENGTH*2];
uint8 DMA_1_Chan;               /* The DMA Channel */
uint8 DMA_2_Chan;
uint8 DMA_1_TD[1];	          /* The DMA Transaction Descriptor (TD) */
uint8 DMA_2_TD[2];
int32 Phase_Shift;
int32 degree;
int flag = 0;
CY_ISR(MyISR)
{
    CyDmaChDisable(DMA_2_Chan);
    CyDelay(200);
    CyDmaChEnable(DMA_2_Chan, 1);
}
int main(void)
{
    /* Start VDAC  */
    VDAC8_Start();
    VDAC8_Shift_Start();
    ADC_Start();
    MyLCD_Start();
    MyLCD_Position(0u,0u);
    MyLCD_PrintString("Degree: ");
    CyGlobalIntEnable; /* Enable global interrupts. */
        for (int i = 0; i < TABLE_LENGTH *2; i++)
    {
        sineSRAM[i] = sineTable[i];
    }
    
    /* Defines for DMA configuration */
    #if (defined(__C51__))  /* Source base address when PSoC3 is used */    
	    #define DMA_SRC_BASE (CYDEV_FLS_BASE)   
    #else  					/* Source base address when PSoC5 is used */
	    #define DMA_1_SRC_BASE (&sineSRAM[0])
        #define DMA_2_SRC_BASE (&sineSRAM[0])
	#endif    
    #define DMA_DST_BASE (CYDEV_PERIPH_BASE)  /* Destination base address */
    
 /* Step1 : DmaInitialize - Initialize the DMA channel
	 * Bytes per burst = 1, (8 bit data transferred to VDAC one at a time)
	 * Request per burst = 1 (this will cause transfer of the bytes only with every new request)
	 * High byte of source address = Upper 16 bits of Flash Base address for PSoC 3,
	                               = HI16(&sineTable) for PSoC 5
     * High byte of destination address =  Upper 16 bits of peripheral base address */
    DMA_1_Chan = DMA_1_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(DMA_1_SRC_BASE), HI16(DMA_DST_BASE) );
    DMA_2_Chan = DMA_2_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(DMA_2_SRC_BASE), HI16(DMA_DST_BASE) );
	
	
    /* Step2 :CyDmaTdAllocate - Allocate TD */
    DMA_1_TD[0] = CyDmaTdAllocate();
    DMA_2_TD[0] = CyDmaTdAllocate();
    DMA_2_TD[1] = CyDmaTdAllocate();
    
    /* Step3 :CyDmaTdSetConfiguration - Configures the TD:
	 * tdHandle = DMA_TD[0] - TD handle previously returned by CyDmaTdAlloc()
	 * Transfer count = table_length (number of bytes to transfer for a sine wave)
	 * Next Td = DMA_TD[0] ; loop back to the same TD to generate a continous sien wave 
	 * Configuration = The source address is incremented after every burst transfer
	 */
	CyDmaTdSetConfiguration(DMA_1_TD[0], TABLE_LENGTH, DMA_1_TD[0], TD_INC_SRC_ADR);
    CyDmaTdSetConfiguration(DMA_2_TD[0], TABLE_LENGTH-1, DMA_2_TD[1], TD_INC_SRC_ADR);
    CyDmaTdSetConfiguration(DMA_2_TD[1], 1, DMA_2_TD[0], TD_INC_SRC_ADR | DMA_2__TD_TERMOUT_EN);
    
    
	
    /* Step 4 : CyDmaTdSetAddress - Configure the lower 16 bit source and destination addresses 
	 * Source address = Lower 16 bits of sineTable array
	 * Destination address = Lower 16 bits of VDAC8_Data_PTR register */
    CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)sineSRAM), LO16((uint32)VDAC8_Data_PTR) );
    CyDmaTdSetAddress(DMA_2_TD[0], LO16((uint32)sineSRAM), LO16((uint32)VDAC8_Shift_Data_PTR) );
    
    
    /* Step 5: Map the TD to the DMA Channel */
    CyDmaChSetInitialTd(DMA_1_Chan, DMA_1_TD[0]); 
    CyDmaChSetInitialTd(DMA_2_Chan, DMA_2_TD[0]);
	
    
    /* Step 6: Enable the DMA channel */
    CyDmaChEnable(DMA_1_Chan, 1);
    CyDmaChEnable(DMA_2_Chan, 1);

    for(;;)
    {
        /* Place your application code here. */
                //Read the potentiometer value
        Phase_Shift = ADC_Read32();
        if (Phase_Shift > 65535)
        {
            Phase_Shift = 65535;
        }
        if (Phase_Shift < 0)
        {
            Phase_Shift = 0;
        }
        //Max phase shift is 128, so we get 511 by 65535/128
        Phase_Shift = Phase_Shift / 511;

        //Convert to degree
        degree = (Phase_Shift * 360 / 128);
        MyLCD_Position(0u, 8u);
        MyLCD_PrintInt16(degree);
        //CyDelay(50);
        CyDmaTdSetAddress(DMA_1_TD[0], LO16((uint32)sineTable), LO16((uint32)VDAC8_Data_PTR));       
        CyDmaTdSetAddress(DMA_2_TD[0], LO16((uint32)(sineSRAM + Phase_Shift)), LO16((uint32)VDAC8_Shift_Data_PTR));
        CyDmaTdSetAddress(DMA_2_TD[1], LO16((uint32) 1 ), LO16((uint32)VDAC8_Shift_Data_PTR));
    }
}

/* [] END OF FILE */
