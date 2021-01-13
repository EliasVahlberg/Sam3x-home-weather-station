/**
* @file
*     TestMode.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/

void graph_data_test()
{
    int num_saved_day_test_data = 0;
    test_day_temp_data = calloc(7,sizeof(day_temp_data));
    if(test_day_temp_data==NULL)
        return;
    day_temp_data* dtd;
    for (int n = 0; n < 7; n++)
    {
        dtd = &(test_day_temp_data[n]);

        char* menu_head = "Temp Statistics: ";
        int len = DAY_LINE0;
        screen_element tel1 = create_screen_element(0,0,len,menu_head);
        //display_write(tel1);


        date            tdate;
        time_hm         thm;
        time_hm         thm2;
        tdate.day   =   n;
        tdate.month =   00;
        tdate.year  =   21;

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
        num_saved_day_test_data++;
    }
    toggle_graphics_mode();
    int keypad_input = 0;
    int prev_input = -1;
    display_staple_plot(test_day_temp_data,1,7);
    while(keypad_input!=12)
    {
        keypad_input = get_user_input();
        if(prev_input==0&&prev_input!=keypad_input)
            if(current_graph_type!=keypad_input&&keypad_input<=7&&keypad_input>0)
                display_staple_plot(test_day_temp_data,keypad_input,num_saved_day_test_data);
        prev_input=keypad_input;
    }
    toggle_graphics_mode();
    menu_type = 2;
    free(test_day_temp_data);
    clear();

}

void temperature_sensor_test()
{
    clear();
    display_write_direct(0,0,23,"Temperature timing test");
    display_write_direct(0,3,13,"Temperature: ");
    display_write_direct(0,4,14,"Time Delta ms: ");
    display_write_direct(0,2,17,"Press # to return");
    unsigned long long t1 = 0;
    unsigned int  t2 = 0;
    double test_temp = 0;   
    while(get_user_input()!=12)
    {
        t1 = microseconds;
        while(1)              //Records a new temp value and displays it
        {
            start_pulse();
            if(temp_rdy_flag)
            {
                get_temp();
                 break;
            }
            temp_reset();
        }
        t2= (int)((double)(microseconds -t1)/1000);
        double_to_str_fixed_length(temp_test_str,curr_temp,6);
        int_to_str_fixed_length(test_delta_time,5,t2);
        
        display_write_direct(13,3,6,temp_test_str);      
        if(t2==0)
            display_write_direct(14,4,5,"<1ms  ");  
        else 
            display_write_direct(14,4,5,test_delta_time);

    }
    clear();
    
}

void hash_test()
{
    screen_element* test_screen_username = malloc(sizeof(screen_element));
    screen_element* test_screen_password = malloc(sizeof(screen_element));
    if(test_screen_password==NULL || test_screen_username==NULL)
        return;
    clear();
    display_write_direct(0,0,39,"Hash test, enter password and username:");
    display_write_direct(0,3,9,"Username:");
    display_write_direct(0,5,9,"Password:");
    char screen_pass[LOGIN_PASSWORD_MAX_LEN];
    char screen_usrname[LOGIN_USERNAME_MAX_LEN];
    char password1[56];
    char username1[56];
    unsigned char hashed_pass[HASHED_PASS_LEN];
    unsigned char hashed_pass2[HASHED_PASS_LEN];
    unsigned char hashed_pass3[HASHED_PASS_LEN];
    int username_length = 0;
    int password_length = 0;

    (*pre_password) = create_screen_element(LOGIN_PASSWORD_X_POS, LOGIN_PASSWORD_Y_POS, usr_pass_str_length, login_pass_str);
    (*pre_username) = create_screen_element(LOGIN_USERNAME_X_POS, LOGIN_USERNAME_Y_POS, usr_pass_str_length, login_user_str);

    int d= 0, e = 0;
    (*test_screen_username) = create_screen_element(LOGIN_USERNAME_X_POS + usr_pass_str_length, LOGIN_USERNAME_Y_POS, d, screen_usrname);
    (*test_screen_password) = create_screen_element(LOGIN_PASSWORD_X_POS + usr_pass_str_length, LOGIN_PASSWORD_Y_POS, e, screen_pass);

    username_length =  login_get_user_input(username, test_screen_username);
    password_length =  login_get_user_input(password, test_screen_password);
    hash_password(password, password_length,screen_usrname,username_length,hashed_pass);
    for (int i = 0; i < 56; i++)
    {
        password1[i] = (char)0;
        username1[i] = (char)0;
    }
    for(int i = 0; i<LOGIN_USERNAME_MAX_LEN;i++)
    {
        screen_usrname[i] = (char)0;
        screen_pass[i]    = (char)0;   
    }
    
    display_clear((*test_screen_password).l, (*test_screen_password).screen_cord.pos, (*test_screen_username).screen_cord.screen_half_val);
    display_clear((*test_screen_username).l, (*test_screen_username).screen_cord.pos, (*test_screen_password).screen_cord.screen_half_val);
    clear_display_direct(0,0,39);
    display_write_direct(0,0,37,"Hash test 1, enter diffrent password:");
    username_length =  login_get_user_input(username, test_screen_username);
    password_length =  login_get_user_input(password, test_screen_password);
    hash_password(password, password_length,screen_usrname,username_length,hashed_pass2);
    for (int i = 0; i < 56; i++)
    {
        password1[i] = (char)0;
        username1[i] = (char)0;
    }
    for(int i = 0; i<LOGIN_USERNAME_MAX_LEN;i++)
    {
        screen_usrname[i] = (char)0;
        screen_pass[i]    = (char)0;   
    }
    display_clear((*test_screen_password).l, (*test_screen_password).screen_cord.pos, (*test_screen_username).screen_cord.screen_half_val);
    display_clear((*test_screen_username).l, (*test_screen_username).screen_cord.pos, (*test_screen_password).screen_cord.screen_half_val);

    clear_display_direct(0,0,37);
    display_write_direct(0,0,27,"Hash test 2, same password:");
    username_length =  login_get_user_input(username, test_screen_username);
    password_length =  login_get_user_input(password, test_screen_password);
    hash_password(password, password_length,screen_usrname,username_length,hashed_pass3);
    for (int i = 0; i < 56; i++)
    {
        password1[i] = (char)0;
        username1[i] = (char)0;
    }
    for(int i = 0; i<LOGIN_USERNAME_MAX_LEN;i++)
    {
        screen_usrname[i] = (char)0;
        screen_pass[i]    = (char)0;   
    }
    display_clear((*test_screen_password).l, (*test_screen_password).screen_cord.pos, (*test_screen_username).screen_cord.screen_half_val);
    display_clear((*test_screen_username).l, (*test_screen_username).screen_cord.pos, (*test_screen_password).screen_cord.screen_half_val);
    int test1 = 0; int test2 = 1;
    for (int i = 0; i < HASHED_PASS_LEN; i++)
    {
        if(hashed_pass[i]!=hashed_pass2[i])
            test1 = 1;
    }
    for (int i = 0; i < HASHED_PASS_LEN; i++)
    {
        if(hashed_pass[i]!=hashed_pass3[i])
            test2=0;
    }
    clear();
    if(test1)
        display_write_direct(0,0,15,"Hash test 1, OK");
    else
        display_write_direct(0,0,17,"Hash test 2, FAIL");
    if(test2)
        display_write_direct(0,1,15,"Hash test 2, OK");
    else
        display_write_direct(0,1,17,"Hash test 2, FAIL");

    display_write_direct(0,14,17,"Press # to return");
    while(get_user_input()!=12)
    {

    }
    clear();
    
    free(test_screen_username);
    free(test_screen_password);

}

void keypad_test()
{
    clear();
    display_write_direct(0,0,25,"Press keys as instructed:");
    
    display_write_direct(0,1,7,"Press 1:");
    while(get_user_input()!=1)
    {}
    display_write_direct(7,1,2,"Ok");
    
    display_write_direct(0,2,7,"Press 2:");
    while(get_user_input()!=2)
    {}
    display_write_direct(7,2,2,"Ok");
    
    display_write_direct(0,3,7,"Press 3:");
    while(get_user_input()!=3)
    {}
    display_write_direct(7,3,2,"Ok");
    
    display_write_direct(0,4,7,"Press 4:");
    while(get_user_input()!=4)
    {}
    display_write_direct(7,4,2,"Ok");
    
    display_write_direct(0,5,7,"Press 5:");
    while(get_user_input()!=5)
    {}
    display_write_direct(7,5,2,"Ok");
    
    display_write_direct(0,6,7,"Press 6:");
    while(get_user_input()!=6)
    {}
    display_write_direct(7,6,2,"Ok");
    
    display_write_direct(0,7,7,"Press 7:");
    while(get_user_input()!=7)
    {}
    display_write_direct(7,7,2,"Ok");

    display_write_direct(0,8,7,"Press 8:");
    while(get_user_input()!=8)
    {}
    display_write_direct(7,8,2,"Ok");

    display_write_direct(0,9,7,"Press 9:");
    while(get_user_input()!=9)
    {}
    display_write_direct(7,9,2,"Ok");

    display_write_direct(0,10,7,"Press *:");
    while(get_user_input()!=10)
    {}
    display_write_direct(7,10,2,"Ok");

    display_write_direct(0,11,7,"Press 0:");
    while(get_user_input()!=11)
    {}
    display_write_direct(7,11,2,"Ok");

    display_write_direct(0,12,7,"Press #:");
    while(get_user_input()!=12)
    {}
    display_write_direct(7,12,2,"Ok");


    display_write_direct(0,13,14,"All keys work.");
    display_write_direct(0,14,17,"Press # to return");
    while(get_user_input()!=12)
    {}
    clear();
}

void light_sensor_test()
{   
    clear();
    int keypad_input = 0;
    display_write_direct(0,0,18,"Light sensor test:");
    while(keypad_input!=12)
    {
        keypad_input = get_user_input();
        light_measure();
        char str_message2[17] = "Voltage 1: ";
        double_to_str_fixed_length(str_message2+11,adc_ch1_voltage,6);

        struct screen_element sensor1 = create_screen_element(0,5,21,str_message2); 
        display_write(sensor1);

        char str_message3[17] = "Voltage 2: ";
        double_to_str_fixed_length(str_message3+11,adc_ch2_voltage,6);
        struct screen_element sensor2 = create_screen_element(0,6,21,str_message3); 
        display_write(sensor2);
    }
    clear();
}

void view_memory_usage()
{
    clear();
    display_write_direct(0,0,17,"Heap Memory info:");
    char str[4];
    struct mallinfo m;
    m = __iar_dlmallinfo();
    int x = (int)m.fordblks;
    int_to_str_fixed_length(str,4,x);
    display_write_direct(0,1,13,"Free space =:");
    display_write_direct(14,1,4,str);
    
     x = (int)m.uordblks;
    int_to_str_fixed_length(str,4,x);
    display_write_direct(0,2,12,"Allocated =:");
    display_write_direct(14,2,4,str);
    
     x = (int)m.usmblks;
    int_to_str_fixed_length(str,4,x);
    display_write_direct(0,3,12,"Max space =:");
    display_write_direct(14,3,4,str);
    x = (int)m.keepcost;
    int_to_str_fixed_length(str,4,x);
    display_write_direct(0,4,13,"Releasable =:");
    display_write_direct(14,4,4,str);


    display_write_direct(0,10,17,"Press # to return");
    while(get_user_input()!=12)
    {

    }
        
}