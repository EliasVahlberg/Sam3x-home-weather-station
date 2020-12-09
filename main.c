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

double temperature = 0.0;           //Temperature retrived from the temp sensor 
#pragma endregion Variables/Parameters

#pragma region Functions
void full_setup();
#pragma endregion Functions


void main(void)
{
    full_setup();

    day_temp_data   dtd;
    date            tdate;
    time_hm         thm;
    tdate.day   =   01;
    tdate.month =   01;
    tdate.year  =   21;
    thm.hour    =   18;
    thm.minute  =   1;
    dtd.day     =   tdate;
    dtd.min     =   22.11111;
    dtd.tmin    =   thm;

    thm.hour    =   23;

    dtd.avg     =   28.11111;
    dtd.tmax    =   thm;
    dtd.max     =   38.11111;
    dtd.vari    =   12.11;
    char* str_data = day_temp_data_to_string(dtd);
    

  while(1)
  {

    if(temp_rdy_flag)
        temperature = get_temp();
        start_pulse();
    loopCount++;
    light_measure();
    if((loopCount%10)==0)
        {
            
            char str_message1[6] = "Temp: ";
            char temp_message1[10];
            char result1[16];
            snprintf(temp_message1, 10, "%f",temperature);
            strcpy(result1, str_message1);
            strcat(result1, temp_message1);
            struct screen_element temp1 = create_screen_element(0,2,16,result1); 
            display_write(temp1);

        }
    if((loopCount%20)==0)
        {
            char str_message2[11] = "Voltage 1: ";
            char temp_message2[10];
            char result2[21];
            snprintf(temp_message2, 10, "%f", adc_ch1_voltage);
            strcpy(result2, str_message2);
            strcat(result2, temp_message2);
            struct screen_element sensor1 = create_screen_element(0,3,21,result2); 
            display_write(sensor1);

            char str_message3[11] = "Voltage 2: ";
            char temp_message3[10];
            char result3[21];
            snprintf(temp_message3, 10, "%f", adc_ch2_voltage);
            strcpy(result3, str_message3);
            strcat(result3, temp_message3);
            struct screen_element sensor2 = create_screen_element(0,4,21,result3); 
            display_write(sensor2);
        }
        int serv = func();
        servo_set_position(serv);
        servo_position = ((double) servo_get_position()/43.7);
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
}


