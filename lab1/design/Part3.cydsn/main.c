/* ========================================
 * Part3
 * Xingyu Zhou xzhou45@ucsc.edu
 * This program uses the LCD dispaly to display
 * the current potentiometer value and the 
 * calculated frequency
 *
 * 
 * 
 *
 * ========================================
*/


#include "project.h"

int32 next_value = 0;
int32 current_value = 0;
int32 result = 0;
int32 reading = 0;
int32 new_reading = 0;
int32 exp_freq = 0;

CY_ISR(MyISR){
    //Method 1
//    current_value = next_value;
//    current_value = Timer_1_ReadCounter();
//    if (next_value < current_value)
//    {
//        result = (next_value + 65535) - current_value;
//    }else
//    {
//        result = next_value - current_value;
//    }
    //Medthod 2 Clear counter
    result = Counter_1_ReadCounter();
    Counter_1_WriteCounter(0);
    result = result * 500;
    Timer_1_ReadStatusRegister();
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    MyADC_Start();
    MyPWM_Start();
    MyLCD_Start();
    Clock_1_Start();
    Timer_1_Start();
    Counter_1_Start();
    isr_1_StartEx(MyISR);
    MyLCD_ClearDisplay();
    MyLCD_Position(0u,0u);
    MyLCD_PrintString("Ex:");
    MyLCD_Position(1,0);
    MyLCD_PrintString("F:");
    
    for(;;)
    {
        reading = MyADC_Read32();
        if(reading > 65535)
        {
            reading = 65535;
        }
        if(reading < 0)
        {
            reading = 0;
        }
        //Get the frequency
        new_reading = ((reading * 24000) / 65535); 
        
        //Display the frequency when it's not 0;
        if(new_reading >= 2)
        {
            MyPWM_WritePeriod(new_reading);
            MyPWM_WriteCompare(new_reading/2);
        }
        //when ADC hits the smallest, frequency is the fatest, which is 23.
        else
        {
            MyPWM_WritePeriod(23);
            MyPWM_WriteCompare(11);
        }
            
        //MyLCD_Position(0u, 4u);
        //MyLCD_PrintInt16(reading);
        MyLCD_Position(1,4);
        MyLCD_PrintInt32(result);
        MyLCD_Position(0,4);
        //Dispalying the expected frequency
        exp_freq = (24000000/MyPWM_ReadPeriod());
        MyLCD_PrintInt32(exp_freq);
        
        
        
        
        /* Place your application code here. */
       
       
        
    }
}

/* [] END OF FILE */
