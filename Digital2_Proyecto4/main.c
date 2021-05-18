
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"

//**********************PROTOTIPOS DE FUNCIONES***********************

void UARTconfig(void);
void UARTIntHandler(void);
void SendString(char* frase);
void display(uint8_t valor);

/**
 * main.c
 */

//*********************DECLARACION DE VARIABLES************************
uint8_t Parqueo1;
uint8_t Parqueo2;
uint8_t Parqueo3;
uint8_t Parqueo4;
uint8_t Disp = 0;
uint8_t suma = 0;

//****************************PROGRAMA**********************************
void main(void)
//*******************************SETUP*************************************
{   //Se configura reloj a 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    //Se activan los puertos E, C y A correspondientes a los que se usan para los puertos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //Se configuran como salida los LEDS de los distintos parqueos
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
    //Se configuran como entrada los push de los distintos parqueos
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    IntMasterEnable();
    UARTconfig();

//******************************MAIN LOOP********************************
    while(1){
        Parqueo1 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);
        Parqueo2 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5);
        Parqueo3 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_6);
        Parqueo4 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_7);

        //**PARQUEO 1**
        if (Parqueo1 == 0){ //Parqueo 1 esta ocupado
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5, 16); //Se enciende led roja
            Disp &= ~(1); //Clear del bit 0
        }
        else{               //Parqueo 1 esta disponible
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5, 32); //Se enciende led verde
            Disp |= 1;   //Set del bit 0
        }

        //**PARQUEO 2**
        if (Parqueo2 == 0){ //Parqueo 2 esta ocupado
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, 4); //Se enciende led roja
            Disp &= ~(2); //Clear del bit 1
        }
        else{               //Parqueo 2 esta disponible
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3, 8); //Se enciende led verde
            Disp |= 2;   //Set del bit 1
        }

        //**PARQUEO 3**
        if (Parqueo3 == 0){ //Parqueo 3 esta ocupado
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5, 16); //Se enciende led roja
            Disp &= ~(4); //Clear del bit 2
        }
        else{               //Parqueo 3 esta disponible
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4|GPIO_PIN_5, 32); //Se enciende led verde
            Disp |= 4;   //Set del bit 2
        }

        //**PARQUEO 4**
        if (Parqueo4 == 0){ //Parqueo 4 esta ocupado
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7, 64); //Se enciende led roja
            Disp &= ~(8); //Clear del bit 3
        }
        else{               //Parqueo 4 esta disponible
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_7, 128); //Se enciende led verde
            Disp |= 8;   //Set del bit 3
        }

        if (1 & Disp){
                    suma+=1;
                }
        if (2 & Disp){
                    suma+=1;
                }
        if (4 & Disp){
                    suma+=1;
                }
        if (8 & Disp){
                    suma+=1;
                }
        display(suma);
        UARTCharPut(UART2_BASE, Disp);
        suma = 0;

    }

}

//**********************************FUNCIONES*********************************************


void UARTIntHandler(void){
    UARTIntClear(UART2_BASE, UART_INT_RT | UART_INT_RX);


}

void UARTconfig(void){
    //Aqui se configura el UART2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2); //Activar clock para UART2
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);//Activar clock para puerto D de la tiva
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6|GPIO_PIN_7); //Se activan los pines 6 y 7 del puerto D
    UARTConfigSetExpClk(UART2_BASE,SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTIntEnable(UART2_BASE, UART_INT_RT | UART_INT_RX); //Se activa interrupcion cada vez que se reciba un dato
    UARTIntRegister(UART2_BASE, UARTIntHandler); //Se le coloca el nombre a la funcion del handler
}

void SendString(char* frase){
    //Funcion para poder enviar string mediante UART
    while (*frase){
        UARTCharPut(UART2_BASE, *frase++);
    }
}

void display(uint8_t valor){
    switch(valor){
        case 0:
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 95); //Se encienden los pines para mostrar un 0
            break;
        case 1:
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 80); //Se encienden los pines para mostrar un 0
                    break;
        case 2:
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 62); //Se encienden los pines para mostrar un 0
                    break;
        case 3:
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 122); //Se encienden los pines para mostrar un 0
                    break;
        case 4:
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 113); //Se encienden los pines para mostrar un 0
                    break;
    }

}
