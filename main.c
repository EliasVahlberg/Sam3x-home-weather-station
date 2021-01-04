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
void set_day_statistics();
void add_temp_recording();
#pragma endregion Functions

void main(void)
{
  
    full_setup();
    //login();
    //edit_user();
    //login();

    //day_temp_data_test();
    //config_time();
    //config_date();
    fast_mode_flag = 1;
    measures_per_min = 10;
    curr_day_data = calloc(1,sizeof(day_temp_data));
    microseconds = 86390000000;
    head= calloc(1,sizeof(linked_node));
    tail= calloc(1,sizeof(linked_node));
    curr_day_data_str = calloc(DAY_TEMP_DATA_LENGTH,sizeof(char));
  while(1)
  {
    if(measure_temp_flag!=last_temp_mesure)
        add_temp_recording();
    if(new_minute_flag!=0)
        add_temp_recording();

    loopCount++;
    light_measure();
    temperature = 0.0;
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

        display_date_time();
        if(new_day_flag)
        {
            set_day_statistics();
        }
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
    init_date_time();
}
void add_temp_recording()
{
     
    last_temp_mesure = measure_temp_flag;
    if(new_minute_flag==1)
    {
        temp_minute_avg /= temp_minute_count;
        (*tail)->temp = double_to_temp(temp_minute_avg);
        temp_minute_avg = 0;
        temp_minute_count = 0;
        new_minute_flag = 0;
        
    }
    if(temp_minute_count<measures_per_min)
    {
        
        while(1)
        {
            start_pulse();
            if(temp_rdy_flag)
            {
                temp_minute_avg =+ get_temp();
                if(temp_minute_count==0)
                {
                set_timedate();
                append_to_list(head, tail,temp_minute_avg);
                }
                temp_minute_count++;
               break;
            }       
        }
    }


    char str_message1[16] = "Temp: ";
    double_to_str_fixed_length(str_message1+6,curr_temp,10);
    struct screen_element temp1 = create_screen_element(0,4,16,str_message1); 
    display_write(temp1);
}
void set_day_statistics()
{
    char* table = "Date    |min |tmin |avg |max |tmax |var ";
    int table_len = 40;
    screen_element tel1 = create_screen_element(0,7,table_len,table);
    display_write(tel1);

    calc_statistics(head,curr_day_data);
    int l = DAY_TEMP_DATA_LENGTH;
    day_temp_data_to_string(*curr_day_data,curr_day_data_str);
    screen_element daydt = create_screen_element(0,9,l,curr_day_data_str);
    display_write(daydt);
    new_day_flag=0;
}