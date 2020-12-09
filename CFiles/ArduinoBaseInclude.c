/**
* @file
*     ArduinoBaseInclude.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/


#pragma region Variables/Parameters
uint32_t    syst                       = 1      ; //Systick config status register status, used to indacte that systick was unable to configure
int        systick_delay               = 0      ; //The remaining number of interrupts that need to occur before delay is complete
int        systick_trigger             = 0      ; //If a delay function is currently used with systick
static int systick_ms                  = 84000  ; //Systick calibration value for 1ms period (MCKf / 1000)
static int systick_us                  = 84     ; //Systick calibration value for 1us period (MCKf / 1000000)


double      adc_CDR1_value             = 0.0    ; //
double      adc_CDR2_value             = 0.0    ; //
double      light_to_volt_coefficcient = 1254.77; //Value extracted from multiple measurements using DMM from the formula (Resolution/System voltage) = (ADC Reading/ Analog Voltage mesured)
int         light_sens_ch1_ready       = 0      ; //
int         light_sens_ch2_ready       = 0      ; //
double      adc_ch1_voltage            = 0.0    ; //
double      adc_ch2_voltage            = 0.0    ; //


int        temp_rdy_flag               = 0      ; //
#pragma endregion Variables/Parameters

#pragma region Functions
void    systick_setup               (                   );
void    delay_milis                 (int milliseconds   );
void    delay_micro                 (int microseconds   );
void    general_delay               (int Value          );

void    pmc_setup                   (                   );
void    TC0_Handler                 (                   );
void    SysTick_Handler             (                   );

#pragma endregion Functions


struct screen_cordinate
{
    int pos;
    int screen_half_val;
};

struct  screen_element{
    struct screen_cordinate screen_cord; int l;
    char* text;
};
//Interrupts every microsecond since microseconds are the smallest unit used
void  systick_setup()
{
    syst = SysTick_Config(systick_us);
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
