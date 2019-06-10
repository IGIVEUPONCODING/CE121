/* ========================================
 *
 * Xingyu Zhou
 * xzhou45@ucsc.edu
 * Lab5 part1
 * This is taken from the eample code
 * with small modification
 * 
 *
 * ========================================
*/
#include "project.h"
/* USB device number. */
#define USBFS_DEVICE  (0u)

/* Active endpoints of USB device. */
#define IN_EP_NUM     (1u)
/* Size of SRAM buffer to store endpoint data. */
#define BUFFER_SIZE   (64u)
#define size 255
#define length 1
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    MyADC_Start();
    /* Start USBFS operation with 5V operation. */
    USBFS_Start(USBFS_DEVICE, USBFS_5V_OPERATION);
    /* Wait until device is enumerated by host. */
    while (0u == USBFS_GetConfiguration())
    {
    }
    for(;;)
    {
        int16 ADCReading = MyADC_Read16();
        if(ADCReading > size)
        {
            ADCReading = size;
        }
        if(ADCReading < 0)
        {
            ADCReading = 0;
        }
        //limit the ADC reading to fit the range
        uint8 ADClimit = ADCReading;
        
        /* Wait until IN buffer becomes empty (host has read data). */
        while (USBFS_IN_BUFFER_EMPTY != USBFS_GetEPState(IN_EP_NUM))
        {
        }
        
        USBFS_LoadInEP(IN_EP_NUM, &ADClimit, length);
        CyDelay(1);
    }
}

/* [] END OF FILE */
