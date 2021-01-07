/**
* @file
*     MenuSystem.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
#pragma region Functions
void main_menu();
void clear_menu(int menu_t);
void clear_main_menu();
void sidebar_menu();
void clear_sidebar_menu();
#pragma endregion Functions

void main_menu()
{
    display_write_direct(0,0,15,"Weather system");
    display_write_direct(0,2,14,"Recorded days:");
    if(num_saved_day_data!=0)
    {
        int i =(curr_day_data-1),n=3;
        do
        {
            char str[11];
            str[0] = (char)(n-3+49);
            str[1] = ':';
            str[2] = ' ';
            date_to_string(saved_day_temp_data[i].day,(char*)(str+3));
            display_write_direct(0,n,11,str);
            if(num_saved_day_data!=7&&i==0)
                break;
            i=(i==0)?7:i-1;
            n++;
        } while (i!=curr_day_data);
        char str2[17] ="Press 1-7 to view";
        display_write_direct(0,n+1,17,str2);
    }

}


void clear_menu(int menu_t)
{
    switch (menu_t)
    {
    case 0:
    clear_main_menu();   
        break;
    case 1:
    clear_day_data();   
        break;
    
    default:
        break;
    }
}

void clear_main_menu()
{
    screen_cord scord;
    for (int  i = 0; i < 15; i++)
    {
    scord = convert_to_scord(0,i);
    set_cursor(scord.pos,scord.screen_half_val);
    display_clear(20,scord.pos,scord.screen_half_val);
    }
    
}

void sidebar_menu()
{
    display_date_time();  //Displays the date and time
    if(!fast_mode_flag && fast_mode_flag)
    {
        while(1)              //Records a new temp value and displays it
        {
            start_pulse();
            if(temp_rdy_flag)
                break;       
            temp_reset();
        }
    }
    temperature_alarm(); //Checks if a temperature alarm should be set before writing
    if(temp_alarm_flag!=0)
        return;
    char str_message1[16] = "Temp: ";
    double_to_str_fixed_length(str_message1+6,curr_temp,4);
    screen_element temp1 = create_screen_element(20,3,10,str_message1); 
    display_write(temp1);


    if(current_user!=NULL)
    {
    screen_element sc_username = create_screen_element(20,5,10,current_user->username);
    display_write(sc_username);
    }

    if(mem_full_flag)
    {
    char str1[9] = "Memory is";
    struct screen_element memfull = create_screen_element(20,7,9,str1); 
    display_write(memfull);
    char str2[5] = "full.";
    memfull = create_screen_element(20,8,5,str2);
    display_write(memfull);

    }
    
    clear_display_direct(20,10,10);
    if(fast_mode_flag)
    {
    char str3[8] = "Fastmode";
    struct screen_element mode = create_screen_element(20,10,8,str3); 
    display_write(mode);
    }
    else
    {
    char str3[10] ="Normalmode";
    struct screen_element mode = create_screen_element(20,10,10,str3); 
    display_write(mode);
    }
        
}

void clear_sidebar_menu()
{

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

void get_num_mes_per_min()
{
    char* str = "Enter measurements per minute";
    screen_element sc = create_screen_element(0,3,29,str);
    display_write(sc);
    int k = 0;
    while(!k)
    {
        k = get_user_input();
    }
    measures_per_min = (k>10)?10:k;
    display_clear(29,sc.screen_cord.pos,sc.screen_cord.screen_half_val);
}
