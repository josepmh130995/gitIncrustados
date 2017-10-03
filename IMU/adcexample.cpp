#include "msp.h"

//Luisa prueba prueba

uint16_t ADC14Result = 0U;
uint16_t ADC14Resultx = 0U;
uint16_t ADC14Resulty = 0U;
uint16_t ADC14Resultz = 0U;

//Otro comentario
int main(void)
{
    //uint32_t csregister = 0x0;

    WDTCTL = WDTPW | WDTHOLD; /* Stop watchdog timer */

    P1->DIR = BIT0;
    P1->OUT = BIT0;
    // Set P4.3 for Analog input, disabling the I/O circuit.
    P4->SEL0 = BIT3;
    P4->SEL1 = BIT3;
    P4->DIR &= ~BIT3;

    /* Configures Pin 4.0, 4.2, and 6.1 as ADC input */
    P4->SEL0 = BIT0;
    P4->SEL1 = BIT0;
    P4->DIR &= ~BIT0;
    P4->SEL0 = BIT2;
    P4->SEL1 = BIT2;
    P4->DIR &= ~BIT2;
    P6->SEL0 = BIT1;
    P6->SEL1 = BIT1;
    P6->DIR &= ~BIT1;

    //TIMER32_1->LOAD = 0x00B71B00; //~0.5s ---> a 48Mhz
    TIMER32_1->LOAD = 0x0000BB80; //~0.5s ---> a 48Mhz
    TIMER32_1->CONTROL =
            TIMER32_CONTROL_SIZE | TIMER32_CONTROL_PRESCALE_0
                    | TIMER32_CONTROL_MODE | TIMER32_CONTROL_IE
                    | TIMER32_CONTROL_ENABLE;
    NVIC_SetPriority(T32_INT1_IRQn, 1);
    NVIC_EnableIRQ(T32_INT1_IRQn);

    ADC14->CTL0 = ADC14_CTL0_PDIV_0 | ADC14_CTL0_SHS_0 | ADC14_CTL0_DIV_7 |
    ADC14_CTL0_SSEL__MCLK | ADC14_CTL0_SHT0_1 | ADC14_CTL0_ON | ADC14_CTL0_SHP;
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[1] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[2] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->MCTL[3] = ADC14_MCTLN_INCH_10 | ADC14_MCTLN_VRSEL_0;
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_ENC;
    ADC14->IER0 = ADC14_IER0_IE0 | ADC14_IER0_IE1 | ADC14_IER0_IE2
            | ADC14_IER0_IE3;
    NVIC_SetPriority(ADC14_IRQn, 1);
    NVIC_EnableIRQ(ADC14_IRQn);
    while (1)
    {

    }

    return 0;
}

extern "C"
{
void T32_INT1_IRQHandler(void)
{
    __disable_irq();
    TIMER32_1->INTCLR = 0U;
    P1->OUT ^= BIT0;
    ADC14->CTL0 = ADC14->CTL0 | ADC14_CTL0_SC; // Start
    __enable_irq();
    return;
}

void ADC14_IRQHandler(void)
{
    __disable_irq();
    ADC14Result = ADC14->MEM[0];
    ADC14Resultx = ADC14->MEM[1];
    ADC14Resulty = ADC14->MEM[2];
    ADC14Resultz = ADC14->MEM[3];
    ADC14->CLRIFGR0 = ADC14_CLRIFGR0_CLRIFG0 | ADC14_CLRIFGR0_CLRIFG1
            | ADC14_CLRIFGR0_CLRIFG2 | ADC14_CLRIFGR0_CLRIFG3;
    __enable_irq();
    return;
    int test = 0U;
}
}
