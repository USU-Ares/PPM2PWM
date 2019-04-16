/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC16F1777
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

//#include "ares_main.h"
//#include <xc.h>
//#include "mcc_generated_files/mcc.h"
//#include "ARES_PPM_to_PWM.c"


//ARES_PPM_to_PWM.c///////////////////////////////

//*
// * File:   ARES_PPM_to_PWM.c
// * Author: Derek
// * 
// * A few equations from the datasheet, assuming a clock source Fosc/4:
// *  PWM Period = [(TxPR)+1]*4*Tosc*TMR_Prescale
// *  Pulse Width = CCPRxH:CCPRxL*Tosc*TMR_Prescale
// *  Duty Cycle Ratio = (CCPRxH:CCPRxL)/[4(TxPR+1)]
// *  Resolution = log[4(TxPR+1)]/log(2) bits
// *  See page 317 of the datasheet for further details
// * 
// *  Baud rate (Page 487 of datasheet)
// *      Fclock = Fosc/[4(SSPxADD + 1)]
// * 
// * Talon SRX requirements:
// * 
// *  PWM Period:
// *      Min: 2.9 ms
// *      Max: 100 ms
// *      Typical: 20 ms
// *      Decided: 10 ms to increase control resolution
// * 
// *  High Pulse:
// *      Min: 1 ms (reverse)
// *      Max: 2 ms (forward)
// *      Mid: 1.5 ms (Stopped)
// *
// * Created on March 12, 2019, 5:09 PM
// */

#include "main.h"

extern struct PORT_Data portData;
extern struct PWM_Data pwmData;
extern struct UART_Data uartData;
extern struct PPM_Data ppmData;

//extern enum UARTLoadState uartLoadState;
//extern enum PPMLoadState ppmLoadState;

/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // RAM allocation
    //uint16_t dataBuf[BUF_SIZE];
    
    // RAM Initialization
    
    // Initialize Objects
    
    
Init_PORT_Data(&portData);
Init_PWM_Data(&pwmData);
Init_UART_Data(&uartData);
Init_PPM_Data(&ppmData);
if(PORTCbits.RC0 == 1) PIC_IS_DRIVE_CONT = true;    //Set the controller state 
    //  upon initialization. The Jumper at RC0 could potentially come off during 
    //  operation of the vehicle. To prevent a sudden change in controller mode, 
    //  RC0 is referenced only at startup.
else PIC_IS_DRIVE_CONT = false;
    
    //set outputs
    //Should be taken care of in MCC Generated Files -> TRISD = 0xC0;   //PORTD <0:5> are all outputs
    
    //TODO: make _PIC_IS_DRIVE_CONT into a dip-switch input on a pin
    while (1)
    {
        // Add your application code
        /*Load PPM and UART buffers*/
        if(PIR1bits.CCP1IF == 1) {
            PPMRead(&ppmData, &pwmData);  //periodic check for data, and read from ppm input
        }
        if(PIR1bits.RCIF == 1) {    //if a byte was received on the UART
            LoadByte(&uartData, &ppmData, &pwmData);    //then load the byte to it's destination
        }
        if((IsDriveCont() == true)&&(driveWDT > 20)) {  //driveWDT increments every 20ms, and clears upon ever pwm update 
            Init_PWM_Data(&pwmData);    //sets all reg back to _1_5MS_COMP, and clears iReg which is never used
        }
    }
}
/**
 End of File
*/