/**
* @file
*     TemperatureSensor.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/


#pragma region Functions
void    timer_counter_setup         (                   );
void    timer_counter_setup         (                   );
void    start_pulse                 (                   );
void    temp_setup                  (                   );
double  get_temp                    (                   );
void    temp_reset                  (                   );
#pragma endregion Functions


void timer_counter_setup()
{
    *AT91C_PMC_PCER         |= (1 << 12)    ; //Enable PMC, starting peripheral clock PIOB  TEMP
    *AT91C_PMC_PCER         |= (1 << 27)    ; //Enable PMC, starting peripheral clock TC0   TEMP
    *AT91C_TC0_CMR          &= ~(0x7)       ; //set Timer_Clock1 as TCCLK
    *AT91C_TC0_CCR          |= 0x5          ; //Enable and sw_reset in TC0_CCR0
    *AT91C_TC0_CMR          |= (0x2 << 16)  ; //Load counter to A when TIOA falling in (TC0_CMR0)
    *AT91C_TC0_CMR          |= (0x1 << 18)  ; //Load counter to B when TIOA rising in (TC0_CMR0)

    //##### Interrupt setup #####
    NVIC_ClearPendingIRQ(TC0_IRQn);                        //Clear pending interrupts during setup
    NVIC_SetPriority(TC0_IRQn, 0);                         // Give TC1 interrupt the highest priority of configurable interrupts
    NVIC_EnableIRQ(TC0_IRQn);                              //Enable interrupt
    
    *AT91C_PIOB_PER         =       (1 << 25); //Enable DP 2
    *AT91C_PIOB_ODR         =       (1 << 25); //Enable output
    *AT91C_PIOB_CODR         =       (1 << 25); //Enable output
    temp_reset();
    

}

void start_pulse()
{
    *AT91C_TC0_IDR = (0x1<<6);                              //Dissable interrupt during temp start pulse 
    *AT91C_PIOB_OER = AT91C_PIO_PB25;      
    delay_micro(25);
    *AT91C_PIOB_ODR = AT91C_PIO_PB25;
    *AT91C_TC0_CCR = 4;                           //sw_reset
    unsigned int interruptRead = *AT91C_TC0_SR;
    *AT91C_TC0_IER = (0x1<<6);                              //Enable interrupt for LDRBS 
    delay_milis(15);
}

void temp_reset()
{
    *AT91C_PIOB_OER = AT91C_PIO_PB25;      
    delay_milis(25);                    // > t_reset_max = 16ms
    *AT91C_PIOB_ODR = AT91C_PIO_PB25;
    delay_micro(10); //t_setup
}

void temp_setup()
{
    timer_counter_setup();
    start_pulse();

}

double get_temp()
{       
    int reg_A       =   *AT91C_TC0_RA;
    int reg_B       =   *AT91C_TC0_RB;
    tcDelta         =   (double)(reg_B-reg_A);
    curr_temp       =   (tcDelta/210.0) -273.15;//210 = 5*(40+2), (5*40 from the datasheet) +2 for accuracy
    temp_rdy_flag   =   0;
    return curr_temp;
}
