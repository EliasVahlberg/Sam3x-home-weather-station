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

    head= calloc(1,sizeof(linked_node));
    tail= calloc(1,sizeof(linked_node));
    curr_min_temp_values = (measures_per_min==1)?
    calloc(1,sizeof(char)):
    calloc(measures_per_min+1,sizeof(char));
    
  while(1)
  {
    set_timedate();
    int keypad_input = get_user_input();
    if(!startup_flag)
    {
        if(measure_temp_flag!=last_temp_measure)
            add_temp_recording();
        if(new_minute_flag!=0)
            add_temp_recording();
    }
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


