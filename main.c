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
void startup_alloc();
void set_day_statistics();
void add_temp_recording();
#pragma endregion Functions


void main(void)
{

    full_setup();
    startup_alloc();
    int keypad_input = 0;

  while(1)
  {
    set_timedate();
    keypad_input = get_user_input();
    //Temperature sensor 
    if(!startup_flag)
    {
        if(measure_temp_flag!=last_temp_measure)
            add_temp_recording();
        if(new_minute_flag!=0)
            add_temp_recording();

        if(new_day_flag)
        {
                set_day_statistics();
        }
    }
    //END Temperature sensor

    //Servo/lightsensor
    if(set_servo_flag&&set_servo_flag!=last_servo_update&&menu_type!=2)
    {
        set_servo_flag=last_servo_update;
        get_light_rotation_data();
        set_to_max_light();
        set_servo_flag = 0;
        servo_position = ((double) servo_get_position()/43.7);
    }
    //END Servo/lightsensor

    //Graph mode
    if(keypad_input == 11&&menu_type==0)
    {
        toggle_graphics_mode();
        display_staple_plot(saved_day_temp_data,1,num_saved_day_data);
    }
    else if(menu_type==-1)
    {
        if(keypad_input==11)
            toggle_graphics_mode();
        if(current_graph_type!=keypad_input&&keypad_input<=7&&keypad_input>0)
            display_staple_plot(saved_day_temp_data,keypad_input,num_saved_day_data);
    }
    //END Graph mode

    //Menu system
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
        if(keypad_input==12)
        {
            clear();
            login();
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
    if(menu_type==1||menu_type==0)
        sidebar_menu();
    //END Menu system

    //Test mode
    if(keypad_input==9&&!fast_mode_flag&&menu_type!=-1&&menu_type!=2)
    {
        menu_type = 2;
        clear_sidebar_menu();
        clear_main_menu();
    } 
    if(menu_type ==2)
    {
        switch (keypad_input)
        {
        case 1:
            graph_data_test();
            break;
        case 2:
            temperature_sensor_test();
            break;
        case 3:
            hash_test();
            break;
        case 4:
            keypad_test();
            break;
        case 5:
            light_sensor_test();
            break;
        case 12:
            menu_type = 0;
            clear();
            break;
        
        default:
        testmode_menu();
            break;
        }   

    }
    //END Test mode
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
    
    //Indicates that the system is starting
    delay_milis(100);
    servo_set_position(9);
    delay_milis(100);
    //Indicates that the system is starting

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

    login();
    get_num_mes_per_min();
    config_time();
    config_date();
    edit_user(current_user);
}

void startup_alloc()
{
    saved_day_temp_data = calloc(7,sizeof(day_temp_data));

    head= calloc(1,sizeof(linked_node));
    tail= calloc(1,sizeof(linked_node));
    curr_min_temp_values = (measures_per_min==1)?
    calloc(1,sizeof(char)):
    calloc(measures_per_min+1,sizeof(char));
    
}

void add_temp_recording()
{
     
    last_temp_measure = measure_temp_flag;
    if(new_minute_flag==1)
    {
        temp_minute_avg /= (double)temp_minute_count;
        curr_min_temp_values[temp_minute_count] = double_to_temp(temp_minute_avg);
        append_to_list(head,tail,curr_min_temp_values,measures_per_min);
        (*tail)->min = previous_minute;
        (*tail)->hour = previous_hour;
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
                temp_minute_avg += get_temp();
                curr_min_temp_values[temp_minute_count] = double_to_temp(curr_temp);

                if(temp_minute_count==0)
                {
                set_timedate();
                previous_minute = (unsigned char) current_time_hm[1];
                previous_hour = (unsigned char) current_time_hm[0];
                }
                temp_minute_count++;
               break;
            }
            temp_reset();       
        }
    }
}

void set_day_statistics()
{
    calc_statistics(head,&(saved_day_temp_data[curr_day_data]));
    curr_day_data= (curr_day_data+1)%7;
    num_saved_day_data = (num_saved_day_data==7)?num_saved_day_data:num_saved_day_data+1;
    new_day_flag=0;
}

