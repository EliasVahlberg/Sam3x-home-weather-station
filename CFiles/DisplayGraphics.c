/**
* @file
*     display.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     Display Init and functions used to interface with the display
*/

#pragma region functions
void toggle_graphics_mode();
void set_cursor_graph(int x, int y); 
void display_graphics_write(char input);
void clear_graphics();
void display_staple_plot(day_temp_data* data,int val_type,int num_days);
void draw_horizontal_line(int x_start, int y_start, int len, int width);
void draw_vertical_line(int x_start, int y_start, int len, int width);
void draw_values(day_temp_data* data,int val_type,int num_days);
void draw_temp_value(double val, int x, int width);

#pragma endregion functions

void toggle_graphics_mode()
{
    if(menu_type==-1)
    {
        Write_Command_2_Display(DISPLAY_SET_TEXT_MODE);
        clear();
    }
    else
        {
            clear();
            clear_graphics();
            Write_Command_2_Display(DISPLAY_SET_GRAPHTEXT_MODE);
        }
    menu_type=(menu_type==-1)?0:-1;
}

void set_cursor_graph(int x, int y) 
{
    char high_adress = ((x+y*30)/DISPLAY_HIGH_ADRESS);
    char low_adress = (x+y*30)%DISPLAY_HIGH_ADRESS;
    if(high_adress>low_adress)
    {
        high_adress--;
        low_adress-=(high_adress+1);
    }
    else 
        low_adress -= (high_adress);
    low_adress += DISPLAY_GRAPH_OFFSET_L;
    high_adress += DISPLAY_GRAPH_OFFSET_H;
    
    Write_Data_2_Display(low_adress);

    Write_Data_2_Display(high_adress);
    
    Write_Command_2_Display(DISPLAY_SET_CURSOR_CMD);
}

void display_graphics_write(char input)
{
    Write_Data_2_Display(input);
    Write_Command_2_Display(DISPLAY_WRITE_AND_INCREMENT_CURSOR_CMD);
}

void clear_graphics()
{
    set_cursor_graph(0,0);
    write_blank_to_display(DISPLAY_SCREEN_HALF*(8*2+8));
    /*
    Write_Data_2_Display(0);
    Write_Data_2_Display(8 + DISPLAY_GRAPH_OFFSET_H);
    3840/8 = 
    
    Write_Command_2_Display(DISPLAY_SET_CURSOR_CMD);

    for (int i = 0; i < DISPLAY_NUM_PIXELS/2; i++)
    {
        display_graphics_write((char)0xff);
        
    }*/
    
    
}

void display_staple_plot(day_temp_data* data,int val_type,int num_days)
{
    clear_graphics();
    draw_horizontal_line(2,112,26,2);
    draw_vertical_line(2,16,DISPLAY_NUM_PIXELS_Y-24,2);
    draw_values(data,val_type,num_days);
    current_graph_type=val_type;
    display_write_direct(0,14,23,"DAY:1  2  3  4  5  6  7");
    for (int i = 0; i < 5; i++)
    {
        char str[2];
        int_to_str_fixed_length(str,2,30-i*4);
        display_write_direct(0,4+i*2,2,str);
    }
    
    

}

void draw_horizontal_line(int x_start, int y_start, int len, int width)
{
    for ( int i = 0; i < width; i++)
    {
        set_cursor_graph((char)x_start,(char)(y_start+i));
        for (int ii = 0; ii < len; ii++)
        {
            general_delay(50);
         display_graphics_write((char)0xff);   
        }
        
    }
    set_cursor_graph(0,0);
    
}

void draw_vertical_line(int x_start, int y_start, int len, int width)
{
    unsigned char c = 0,d=128;
    for (int i = 0; i < width; i++) //creates a char that represents width of the line
    {
        c+=d;
        d/=2;
    }
    
    for ( int i = 0; i < len; i++)
    {
        d=i;
        general_delay(50);
        set_cursor_graph((char)x_start,(char)(y_start+i));
        display_graphics_write(c);
    }
    if(d%20!=2)
    set_cursor_graph(0,0);
}

void draw_values(day_temp_data* data,int val_type,int num_days)
{
    for (int i = 0; i < num_days; i++)
    {
         draw_temp_value(35.5,4+i*3,0);
    }
    switch (val_type)
    {
    case 1:
        for (int i = 0; i < num_days; i++)
        {
         draw_temp_value(data[i].avg,4+i*3,8);
        }
        
        break;
    case 2:
        for (int i = 0; i < num_days; i++)
        {
         draw_temp_value(data[i].min,4+i*3,8);
        }
        break;
    case 3:
        for (int i = 0; i < num_days; i++)
        {
         draw_temp_value(data[i].max,4+i*3,8);
        }
        break;
    case 4:
        for (int i = 0; i < num_days; i++)
        {
         draw_temp_value(data[i].vari,4+i*3,8);
        }
        break;
    
    default:
        break;
    }
}

void draw_temp_value(double val, int x, int width)
{
    double max = (double )(255+TEMP_D_OFFSET)/10;
    double min = (double )(TEMP_D_OFFSET)/10;
    double k = (112-16)/max;
    if(val<min)
        val = min;
    if(val>max)
        val = max;
    int y = k*(val)+16;
    int len = 112-y;
    draw_vertical_line(x,y,len,width);
    
}
