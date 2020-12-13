/**
* @file
*     ArduinoBaseInclude.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/

#pragma region Functions
void    systick_setup               (                   );
void    delay_milis                 (int milliseconds   );
void    delay_micro                 (int microseconds   );
void    general_delay               (int Value          );

void    pmc_setup                   (                   );
void    TC0_Handler                 (                   );
void    SysTick_Handler             (                   );

#pragma endregion Functions


//Interrupts every microsecond since microseconds are the smallest unit used
void  systick_setup()
{
    syst = SysTick_Config(SYSTICK_CONFIG_MICRO);
    NVIC_EnableIRQ(SysTick_IRQn);
}

void delay_milis(int milliseconds)
{
    systick_delay = milliseconds*1000;
    systick_trigger = 1;
    while (systick_trigger)
    {
        asm("nop");
    }
    
}

void delay_micro(int microseconds)
{
    systick_delay = microseconds;
    systick_trigger = 1;
    while (systick_trigger)
    {
        asm("nop");
    }
}

void general_delay(int Value){
int i;
for(i=0;i<Value;i++)        //
asm("nop");                 //CLK cycles: 4
} 

void TC0_Handler(void)
{
    *AT91C_TC0_IDR = (0x1<<6);       //Dissable interrupt LDRBS
    temp_rdy_flag = 1;
}

void SysTick_Handler(void)
{
    if(systick_trigger)
    {
        systick_delay--;
        if(systick_delay<=0)
        {
            systick_trigger = 0;
        }
    }
}

void ADC_Handler(void){
  unsigned int channel_number = ((*AT91C_ADCC_LCDR&(0xF<<12))>>12);
  if(channel_number == 1)
    light_sens_ch1_ready = 1;                           //Flag to indicate that a measure value on CH1 is ready
  if(channel_number==2)
    light_sens_ch2_ready = 1;                           //Flag to indicate that a measure value on CH2 is ready   
  if(light_sens_ch2_ready&light_sens_ch1_ready)
  *AT91C_ADCC_IDR = (1<<24);                      //Interrupt disable on the ADC
}
