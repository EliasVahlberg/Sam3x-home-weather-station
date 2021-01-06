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
int     temperature_alarm           (                   );
void    temp_display_alarm          (                   );
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
    delay_milis(4);                                        //Awaits an tc0 interrupt (changed from 15 to 4 after testing)
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

int temperature_alarm()
{
    if(temp_alarm_flag!=0) //Checks if alarms should be disabled
    {
        if(!(curr_temp>= TEMP_ALARM_UPPER_VALUE||curr_temp<=TEMP_ALARM_LOWER_VALUE))
        {
            display_write_disable_flag = 0;
            screen_cord sc_temp_alarm = convert_to_scord(TEMP_ALARM_DISPLAY_X,TEMP_ALARM_DISPLAY_Y);
            display_clear(TEMP_ALARM_DISPLAY_LENGTH,sc_temp_alarm.pos,sc_temp_alarm.screen_half_val);
            temp_alarm_flag=0;
        }
    }
    else if(curr_temp>= TEMP_ALARM_UPPER_VALUE||curr_temp<=TEMP_ALARM_LOWER_VALUE) //Check if alarm should be enabled
    {
        temp_alarm_flag = (curr_temp>= TEMP_ALARM_UPPER_VALUE)?2:1;

        //Checker that determines if it is bug or an actual temperature change
        int f =0;
        double d = 0.0;
        for (int  i = 0; i < TEMP_ALARM_RECHECK; i++)
        {
            while(1)
            {
                start_pulse();
                if(temp_rdy_flag)
                    {
                        d = get_temp();
                        break;
                    }
                delay_milis(4);
            }
            f = (d>= TEMP_ALARM_UPPER_VALUE)?2:
            ((d<=TEMP_ALARM_LOWER_VALUE)?1:0);
            if(f!=temp_alarm_flag) //Checks if the new temporary "flag" f is equal to the last, if not it's considered a bug and will terminate the alarm
            {
                temp_alarm_flag = 0;
                return temp_alarm_flag;
            }
        }
            temp_display_alarm();
    }
    return temp_alarm_flag;
    
}

void temp_display_alarm()
{
    display_write_disable_flag = 0;
    clear();
    screen_element temp_alarm_element;
    if(temp_alarm_flag==1)
        temp_alarm_element=create_screen_element(TEMP_ALARM_DISPLAY_X,TEMP_ALARM_DISPLAY_Y,TEMP_ALARM_DISPLAY_LENGTH,TEMP_ALARM_DISPLAY_MES_LOW);
    else
        temp_alarm_element=create_screen_element(TEMP_ALARM_DISPLAY_X,TEMP_ALARM_DISPLAY_Y,TEMP_ALARM_DISPLAY_LENGTH,TEMP_ALARM_DISPLAY_MES_HIGH);
    
    display_write(temp_alarm_element);
    display_write_disable_flag = 1;

}
