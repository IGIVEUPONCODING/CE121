/* ========================================
 * CE121_Lab1_Part1(b)
 * Xingyu Zhou 
 * xzhou45@ucsc.edu
 * This program allows us to adjust the
 * brightness of the LED using potentiometer.
 * 
 * 
 *
 * ========================================
*/

#include "project.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    MyPWM_Enable();
    MyPWM_Start();
    MyADC_Start();
    int32 value;
    for(;;)
    {
        //Testing PWM.
//        MyPWM_WriteCompare(0);
//        CyDelay(2000);
//        MyPWM_WriteCompare(400);
//        CyDelay(2000);
//        MyPWM_WriteCompare(600);
//        CyDelay(2000);
//        MyPWM_WriteCompare(800);
//        CyDelay(2000);
//        MyPWM_WriteCompare(999);
//        CyDelay(2000);
        //read ADC value and store it.
        value = MyADC_Read32();
        if(value < 0){
            value = 0;
        }
        if(value > 65535){
            value = 65535;
        }
        MyPWM_WriteCompare(value / 66);          //The range of the period is 1-999. 65535/999 = 65.
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
