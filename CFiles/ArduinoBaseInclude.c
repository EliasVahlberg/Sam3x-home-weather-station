/**
* @file
*     ArduinoBaseInclude.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     This file contains functions which closely connected to the arduino base functionalities, such as Interrupt handlers,
*      delay functions and setup of systick
*     
*/

#pragma region Functions
void systick_setup      (                );
void delay_milis        (int milliseconds);
void delay_micro        (int microseconds);
void general_delay      (int Value       );
void pmc_setup          (                );
void TC0_Handler        (                );
void SysTick_Handler    (                );

#pragma endregion Functions

/**
* systick_setup 
* \brief Uses the provided function Systick Config to configure then start the systick interrupt each 84 clock-cycle
*  i.e. each microsecond, since this is the highest precission time mesurement that is required by any internal och external component
*/
void systick_setup()
{
    syst = SysTick_Config(SYSTICK_CONFIG_MICRO);
    NVIC_EnableIRQ(SysTick_IRQn);
}

/**
* delay_milis 
* \brief Holds the programm counter within this function untill the time entered in milliseconds has passed.
*  This is achived by setting the systick_trigger flag to one  and the the systick_delay to millisecond value times 1000 
*  to convert it from microseconds. These two flags are read in the systick handler which starts the decrementing of the delay value.
*  During this time the cpu is excecutes the asm("nop") command (No opperation)
*  Used to delay the processor for a precise number of milliseconds.
* @param milliseconds the number of milliseconds to be decremented in the systick handler
*/
void delay_milis(int milliseconds)
{
    systick_delay = milliseconds * 1000;
    systick_trigger = 1;
    while (systick_trigger)
    {
        asm("nop");
    }
}
/**
* delay_micro
* \brief Works functionally the same as delay_millis the only diffrence being that the input is in microseconds.
*  Used to delay the processor for a precise number of microseconds. 
* @param microseconds the number of microseconds to be decremented in the systick handler 
*/
void delay_micro(int microseconds)
{
    systick_delay = microseconds;
    systick_trigger = 1;
    while (systick_trigger)
    {
        asm("nop");
    }
}

/**
* general_delay 
* \brief A non time-sensitive delay that simply preforms the asm("nop") opperation as many times as 
*  Used to delay the processor for a small approximate time (mainly by the display)
* @param Value number of "nop" opperations 
*/
void general_delay(int Value)
{
    int i;
    for (i = 0; i < Value; i++) //
        asm("nop");             //CLK cycles: 4
}

/**
* TC0_Handler
* \brief The handler for the the timer clock which simply disables any further interrupts and sets the temp_rdy_flag 
*  to indicate that a new temperature value is redy to be fetched from the registers A and B in the TC0 
*  The TC0 interrupt gets configured in the TemperatureSensor.c file to:
*  Load counter to A when TIOA falling
*  Load counter to B when TIOA rising
*  interrupt for LDRBS 
*/
void TC0_Handler(void)
{
    *AT91C_TC0_IDR = (0x1 << 6); //Dissable interrupt LDRBS
    temp_rdy_flag = 1;
}


/**
* SysTick_Handler
* \brief The systick handler is branched to every 84 clock cycles and if the delay_trigger is active the delay val is decremented untill it 
*  reaches zero after which the delay_trigger flag is cleared.
*  Then checks if the time config flag is set otherwise just increment the microseconds global variable which keeps track of the day time in milliseconds.
*/
void SysTick_Handler(void)
{

    if (systick_trigger)
    {
        systick_delay--;
        if (systick_delay <= 0)
        {
            systick_trigger = 0;
        }
    }
    if (time_config_flag != 1)
    {
        microseconds++;
    }
}

/**
* ADC_Handler
* \brief The Analoge to digital converter handler is branched to when a light sensor value is retrived.
*  This interrupt runs once for each channel(CH1 and CH2) and sets the corresponding flag once both channels have loaded a value 
*  the interrupt is dissabled   
*/
void ADC_Handler(void)
{
    unsigned int channel_number = ((*AT91C_ADCC_LCDR >> 12) & 0xF);
    if (channel_number == 1)
        light_sens_ch1_ready = 1; //Flag to indicate that a measure value on CH1 is ready
    if (channel_number == 2)
        light_sens_ch2_ready = 1; //Flag to indicate that a measure value on CH2 is ready
    if (light_sens_ch2_ready & light_sens_ch1_ready)
        *AT91C_ADCC_IDR = (1 << 24); //Interrupt disable on the ADC
}
