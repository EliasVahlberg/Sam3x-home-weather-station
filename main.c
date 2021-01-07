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
void get_num_mes_per_min();
void day_temp_data_test(day_temp_data*   dtd,int i);

//Menu
void main_menu();
void clear_menu(int menu_t);
void clear_main_menu();
void sidebar_menu();
void clear_sidebar_menu();
#pragma endregion Functions

//DEBUG
    unsigned long loop_time = 0;
//DEBUG

void main(void)
{
  
    full_setup();
    login();
    get_num_mes_per_min();
    config_time();
    config_date();
    edit_user(current_user);

    //day_temp_data_test();
    //microseconds = 85730000000; //86390000000
    saved_day_temp_data = calloc(7,sizeof(day_temp_data));
    test_day_temp_data = calloc(7,sizeof(day_temp_data));
    head= calloc(1,sizeof(linked_node));
    tail= calloc(1,sizeof(linked_node));
    curr_min_temp_values = (measures_per_min==1)?
    calloc(1,sizeof(char)):
    calloc(measures_per_min+1,sizeof(char));
    for (int ii = 0; ii < 7; ii++)
    {
    day_temp_data_test(&(test_day_temp_data[ii]),ii);
    }
    //DEBUG
    unsigned long long prevloop = 0;
    //DEBUG
  while(1)
  {
    //DEBUG
    
    loop_time = (unsigned long)((microseconds-prevloop)/1000);
    if(loop_time>150)
    {
        loop_time++;
    }
    prevloop = microseconds;
    //DEBUG
    set_timedate();
    int keypad_input = get_user_input();
    if(!startup_flag)
    {
        if(measure_temp_flag!=last_temp_measure)
            add_temp_recording();
        if(new_minute_flag!=0)
            add_temp_recording();
    }
    loop_time = (unsigned long)((microseconds-prevloop)/1000);
    if(loop_time>10)
        loop_time++;
    light_measure();
    if(0) //Remove before handin
    {
        if(set_servo_flag&&set_servo_flag!=last_servo_update)
        {
            set_servo_flag=last_servo_update;
            get_light_rotation_data();
            set_to_max_light();
            set_servo_flag = 0;
        }
        servo_position = ((double) servo_get_position()/43.7);
    }

    if(keypad_input == 11)
    {
        toggle_graphics_mode();
    }
    
    if(new_day_flag)
    {
            set_day_statistics();
    }
    
    if(menu_type == 0)
    {
        if((keypad_input-1) >=0 &&(keypad_input-1) <=7 )
        {
            if(keypad_input<=num_saved_day_data)
            {
                clear_menu(0);
                menu_type = 1;
                print_day_data(saved_day_temp_data[(keypad_input-1)]);
            }    
        }
        else
            main_menu();
        if(keypad_input==10)
        {
            toggle_fastmode();
        }   
    }
    if(menu_type == 1)
    {
        if(keypad_input == 12)
        {
            clear_menu(1);
            menu_type = 0;
            main_menu();
        }
    }
    if(menu_type!=-1)
        sidebar_menu();
    if(menu_type==-1)
    {
        if(current_graph_type!=keypad_input&&keypad_input<=7&&keypad_input>0)
            display_staple_plot(test_day_temp_data,keypad_input,7);//Change seven if we are using real data
    }
    loopCount++;
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

    toggle_graphics_mode();
    clear_graphics();
    toggle_graphics_mode();
    
    init_date_time();
    
    startup_flag = 2;
}



void voltage_print() //TEMPORARY
{
        
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
}

void day_temp_data_test(day_temp_data* dtd,int n)//TEMPORARY
{
    char* menu_head = "Temp Statistics: ";
    int len = DAY_LINE0;
    screen_element tel1 = create_screen_element(0,0,len,menu_head);
    //display_write(tel1);

    
    date            tdate;
    time_hm         thm;
    time_hm         thm2;
    tdate.day   =   n;
    tdate.month =   88;
    tdate.year  =   77;

    thm.hour    =   18;
    thm.minute  =   1;

    thm2.hour    =   19;
    thm2.minute  =   2;

    dtd->day     =   tdate;
    dtd->min     =   25.1-2*n;
    dtd->tmin    =   thm;
    dtd->avg     =   25.1+n%3;
    dtd->tmax    =   thm2;
    dtd->max     =   25.1+n;
    dtd->vari    =   15.1+n%4;
    //print_day_data(dtd);
    num_saved_day_data++;
    curr_day_data++;

}