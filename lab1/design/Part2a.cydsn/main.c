/* ========================================
 * CE121 Lab1_Part2(a)
 * Xingyu Zhou xzhou45@ucsc.edu
 * This program uses the ultra-sonic sensor 
 * to detect and measure the distant between
 * the sensor and the object. A LED is used 
 * to indicate the distant.
 * 
 *
 * ========================================
*/


#include "project.h"
int32 distant = 0;
int32 previous_disntant = 0;
int32 result = 0;
int32 d = 0;
CY_ISR(MyISR)
{
    previous_disntant = distant;
    distant = Timer_1_ReadCounter();
    Timer_1_ReadStatusRegister();
}

int main(void)
{   
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Clock_1_Start();
    Clock_2_Start();
    Counter_1_Start();
    Timer_1_Init();
    
    
    isr_1_StartEx(MyISR);
    for(;;)
    {          
        /*Trigger Test
        One_Bit_Reg_Write(1);
        One_Bit_Reg_Write(0);
        */
        //CyDelayUs(10);
        /*Counter Test*/       

        //Control_Reg_Reset_Write(0);
        //CyDelayUs(10);
        Control_Reg_Reset_Write(1);
        Trigger_Reg_Write(1);
        Control_Reg_Reset_Write(0);
        CyDelay(500);
        //convert the negative value to positive.
        if(distant> previous_disntant){
            result = (previous_disntant + 65535) - distant;
        }
        else{
            result = previous_disntant - distant;
        }
        //convert the result to centimeters
        d = 340 * result / 20000;
        //if the object is out of 1m, turn off LED;
        if(d>100)
        {
            CyDelay(2000);
            CyPins_SetPin(Pin_2_0);
        }
        //if the object is within 1m, turn on LED;
        if(d<100)
        {
            
            CyPins_ClearPin(Pin_2_0);
            
        }
      
        
        
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
