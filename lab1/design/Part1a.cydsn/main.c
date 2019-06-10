/* ========================================
 * Xingyu Zhou xzhou45@ucsc.edu
 * Lab1 Part1a
 * This program uses 3 method to toggle LED
 * to turn on and off.
 * Three methods are:
 * 1.Per-Pin.
 * 2.Component API.
 * 3.Control Register.
 * ========================================
*/
#include "project.h"


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    //Per-Pin Method
    for(;;)
    {
    CyPins_SetPin(Pin_1_0);
    CyDelay(100);
    CyPins_ClearPin(Pin_1_0);
    CyDelay(500);
    CyPins_SetPin(Pin_2_0);
    CyDelay(100);
    CyPins_ClearPin(Pin_2_0);
    CyDelay(500);
    CyPins_SetPin(Pin_3_0);
    CyDelay(100);
    CyPins_ClearPin(Pin_3_0);
    CyDelay(500);
//    CyPins_SetPin(Pin_1_0);
//    CyDelay(500);
//    CyPins_ClearPin(Pin_2_0);
//    CyDelay(500);
//    CyPins_SetPin(Pin_3_0);
//    CyDelay(500);
//    CyPins_SetPin(Pin_1_0);
//    CyDelay(500);
//    CyPins_SetPin(Pin_2_0);
//    CyDelay(500);
//    CyPins_ClearPin(Pin_3_0);
//    CyDelay(1000);
    
    //Component API method
//    Pin_1_Write(0);
//    Pin_1_Write(1);

//    Pin_2_Write(1);
//    Pin_3_Write(1);
//    CyDelay(1000);
//    Pin_1_Write(1);
//    Pin_2_Write(0);
//    Pin_3_Write(1);
//    CyDelay(1000);
//    Pin_2_Write(1);
//    Pin_3_Write(0);
//    Pin_2_Write(1);
//    CyDelay(1000);
//    Pin_3_Write(1);
   
//   //Control Register method
//   for (int i=0;i<=1;i++){
//    Control_Reg_1_Write(i);
//}
    
        
    
    }
}

/* [] END OF FILE */
