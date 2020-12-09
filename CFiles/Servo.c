/**
* @file
*     Servo.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
//#include "ArduinoBaseInclude.c"

#pragma region Variables/Parameters
double      servo_position             = 0.0    ; //
#pragma endregion Variables/Parameters

#pragma region Functions
void    servo_set_position          (int x              );
int     servo_get_position          (                   );
void    pwm_setup                   (                   );
#pragma endregion Functions

void pwm_setup()
{
    *AT91C_PMC_PCER1 = (1 << 4);                                            //Enable PMC for the PWM
    *AT91C_PIOB_PER = (1 << 17);                                            //Enable Digital pin 2

    *AT91C_PIOB_PDR = AT91C_PIO_PB17;                                    // Transfer controll to the peripheral B
    *AT91C_PIOB_ABMR = AT91C_PIO_PB17;                                   //  A/B Mode register :  Selects peripheral B to take control of the pin

    *AT91C_PWMC_ENA = AT91C_PWMC_CHID1;                                  //PWM Enable on channel 1 
    *AT91C_PWMC_CH1_CMR = 0x5;                                           //Channel mode register: sets the channel prescaler(CPRE) (0x5 = MCK/32)

    *AT91C_PWMC_CH1_CPRDR = 52500;                                       //Sets the channel period to 20 ms, CPRDR = (MCK/CPRE) * Period, Period = (10^-3)*20 (20ms), MCK=84*10^6, CPRE = 32
                                                                         //  (84*10^6/32)*(10^-3)*20 = 52500
    *AT91C_PWMC_CH1_CDTYR = 2625;                                        //Duty cycle begins after 1ms: CPRDR/20
}

void servo_set_position(int x)
{
    *AT91C_PWMC_CH1_CDTYR = (1700 + (x-1)*280); //Rotates x*10 degrees
}

int servo_get_position()
{
    return *AT91C_PWMC_CH1_CDTYR;
}
