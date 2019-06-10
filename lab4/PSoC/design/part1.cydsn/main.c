/* ========================================
 * Xingyu Zhou
 * xzhou45@ucsc.edu
 * Lab4 part1
 * remote control led with pwm
 *
 * 
 * 
 *
 * ========================================
*/
#include "project.h"
#define size 255
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    MyADC_Start();
    MyUART_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        int ADCRead = MyADC_Read16();
        if(ADCRead > size)
        {
            ADCRead = size;
        }
        if(ADCRead < 0)
        {
            ADCRead = 0;
        }
        //transmit 1 byte at a time
        MyUART_PutChar(ADCRead);
        //Delay 1 sec
        CyDelay(1);
    }
}

/* [] END OF FILE */
