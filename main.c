/**
* @file
*     main.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/

#include "CFiles\Include.h"

#pragma region Variables/Parameters
int    loopCount        = 0     ;           //
double temperature      = 0.0   ;           //Temperature retrived from the temp sensor 
#pragma endregion Variables/Parameters

#pragma region Functions
void full_setup();
#pragma endregion Functions

void main(void)
{
  
    full_setup();
    login();
    edit_user();
    login();

    char* table = "Date    |min |tmin |avg |max |tmax |var ";
    int table_len = 40;
    screen_element tel1 = create_screen_element(0,0,table_len,table);
    display_write(tel1);
    day_temp_data   dtd;
    date            tdate;
    time_hm         thm;
    time_hm         thm2;
    tdate.day   =   99;
    tdate.month =   88;
    tdate.year  =   77;

    thm.hour    =   18;
    thm.minute  =   1;

    thm2.hour    =   19;
    thm2.minute  =   2;

    dtd.day     =   tdate;
    dtd.min     =   22.11111;
    dtd.tmin    =   thm;

    dtd.avg     =   28.11111;
    dtd.tmax    =   thm2;
    dtd.max     =   38.11111;
    dtd.vari    =   12.11;
    char str_data[DAY_TEMP_DATA_LENGTH];
    day_temp_data_to_string(dtd,str_data);
    int l = DAY_TEMP_DATA_LENGTH;
    screen_element sel1 = create_screen_element(0,2,l,str_data);
    display_write(sel1);



  while(1)
  {

    start_pulse();
    if(temp_rdy_flag)
        temperature = get_temp();
    loopCount++;
    light_measure();
    if((loopCount%10)==0)
        {
            char str_message1[16] = "Temp: ";
            double_to_str_fixed_length(str_message1+6,temperature,10);
            struct screen_element temp1 = create_screen_element(0,4,16,str_message1); 
            display_write(temp1);

        }
    if((loopCount%20)==0)
        {
            char str_message2[21] = "Voltage 1: ";
            double_to_str_fixed_length(str_message2+11,adc_ch1_voltage,10);

            struct screen_element sensor1 = create_screen_element(0,5,21,str_message2); 
            display_write(sensor1);

            char str_message3[21] = "Voltage 2: ";
            double_to_str_fixed_length(str_message3+11,adc_ch2_voltage,10);
            struct screen_element sensor2 = create_screen_element(0,6,21,str_message3); 
            display_write(sensor2);
        }
        int serv = get_keypad_key();
        if(serv==12)
        {
            get_light_rotation_data();
            set_to_max_light();
        }
        //servo_set_position(serv);
        servo_position = ((double) servo_get_position()/43.7);
        if(loopCount>15)
            temperature_alarm();
  }
  
}
void full_setup()
{
    SystemInit(); 
    systick_setup();
    
    temp_setup();
    adc_setup();
    pwm_setup();
    //Used as an notification ##Remove before project is done
    delay_milis(100);
    servo_set_position(9);
    delay_milis(100);
    //
    keypad_setup();
    display_setup();
    Init_Display();
    clear();
    login_init();
}


