/**
* @file
*     TempStatistics.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     A simple singly linked list implementation 
*/
//link_node
//Linked_Element
#pragma region functions
void            update_avg_min_max      (linked_node* head,day_temp_data* data  );
void            update_statistics       (linked_node* head,day_temp_data* data  );
void            calc_var                (                  day_temp_data* data  );
void            clear_day_data          (                                       );
void            print_day_data          (day_temp_data t_data                   );
int             day_temp_data_to_string (day_temp_data t_data,  char* t_data_str);
#pragma endregion functions

void update_statistics(linked_node* head,day_temp_data* data)
{
    update_avg_min_max(head,data);
    data->day.year=(int) current_date[0];
    data->day.month =(int) current_date[1];
    data->day.day =(int) current_date[2];
}
void calc_statistics(linked_node* head,day_temp_data* data)
{
    update_avg_min_max(head,data);
    data->avg = var_sum1/num_of_measurements;                           //Calculate the avrage over the whole day
    data->vari = var_sum2/num_of_measurements-(data->avg)*(data->avg);  //Calculate the variance using the avrage


}

void update_avg_min_max(linked_node* head,day_temp_data* data)
{
    linked_node temp_node = *head;
    if(temp_node == NULL)
        return;
    int i = 1;
    double temp_double = 0;
    unsigned char tmin[2]; 
    unsigned char tmax[2];
    unsigned char temps[11];
    double min = temp_to_double(temp_node->temp);
    double max = min;
    double avg = min;
    tmin[0] =temp_node->hour;
    tmin[1] =temp_node->min;
    tmax[0] = tmin[0];
    tmax[1] = tmin[1];
    while(1)
    {
        if(temp_node->next==NULL||temp_node->next==*head)
            break;
        temp_node = temp_node->next;

        get_node_temps(temp_node, temps);
        for (int j = 0; j < measures_per_min; j++)
        {
            temp_double = temp_to_double(temps[i]);

            if(temp_double<min)
            {
                min = temp_double;
                tmin[0] =temp_node->hour;
                tmin[1] =temp_node->min;
            }
            if(temp_double>max)
            {
                max = temp_double;
                tmax[0] =temp_node->hour;
                tmax[1] =temp_node->min;
            }
        }
        temp_double = temps[measures_per_min];
        avg += temp_double;
        var_sum1+=temp_double;
        var_sum2+=temp_double*temp_double;
        i++;
    }
    num_of_measurements+=i;
    avg /=i;
    data->avg            = avg;
    data->max            = max;
    data->min            = min;
    data->tmin.hour      =(int) tmin[0];
    data->tmin.minute    =(int) tmin[1];
    data->tmax.hour      =(int) tmax[0];
    data->tmax.minute    =(int) tmax[1];

}

void calc_avg(linked_node* head)
{

}

void calc_max(linked_node* head)
{
    
}

void calc_min(linked_node* head)
{
    
}
void calc_var(day_temp_data* data)
{
    
}
void clear_day_data()
{
    screen_cord scord = convert_to_scord(0,0);
    set_cursor(scord.pos,scord.screen_half_val);
    display_clear(DAY_LINE0,scord.pos,scord.screen_half_val);
    
    scord = convert_to_scord(0,1);
    set_cursor(scord.pos,scord.screen_half_val);
    display_clear(DAY_LINE1,scord.pos,scord.screen_half_val);
    scord = convert_to_scord(0,2);
    set_cursor(scord.pos,scord.screen_half_val);
    display_clear(DAY_LINE2,scord.pos,scord.screen_half_val);
    scord = convert_to_scord(0,3);
    set_cursor(scord.pos,scord.screen_half_val);
    display_clear(DAY_LINE3,scord.pos,scord.screen_half_val);
    scord = convert_to_scord(0,4);
    set_cursor(scord.pos,scord.screen_half_val);
    display_clear(DAY_LINE4,scord.pos,scord.screen_half_val);
    scord = convert_to_scord(0,5);
    set_cursor(scord.pos,scord.screen_half_val);
    display_clear(DAY_LINE5,scord.pos,scord.screen_half_val);
}
void print_day_data(day_temp_data t_data)
{
    char str[DAY_TEMP_DATA_LENGTH];
    day_temp_data_to_string(t_data,str);

    int i = 0;
    screen_element sc;
    sc = create_screen_element(0,1,DAY_LINE1,(char*)str);
    display_write(sc);
    i+=DAY_LINE1;
    sc = create_screen_element(0,2,DAY_LINE2,(char*)str+i);
    display_write(sc);
    i+=DAY_LINE2;
    sc = create_screen_element(0,3,DAY_LINE3,(char*)str+i);
    display_write(sc);
    i+=DAY_LINE3;
    sc = create_screen_element(0,4,DAY_LINE4,(char*)str+i);
    display_write(sc);
    i+=DAY_LINE4;
    sc = create_screen_element(0,5,DAY_LINE5,(char*)str+i);
    display_write(sc);
}

/**
* day_temp_data_to_string 
* \brief Converts the struct day_temp_data which contains all the statistics for one day in to a string
*         where each element is separated by the char literal DAY_TEMP_PADDING    
* @param t_data source of the struct. 
* @param t_data_str the destination of the resulting string.
* @return The resulting length of the string with the following format:
* day : [0][n], min : [1][n], tmin : [2][n], avg : [3][n], max : [4][n], tmax : [5][n], var : [6][n]
*  date       , double      , time         , double      , double      , time         , double
*/
int day_temp_data_to_string(day_temp_data t_data,char* t_data_str)
{
    int i = 0;
    char temp_double[STR_DOUBLE_SIZE];
    
    //Date
    //*(t_data_str+i) = "Date: ";
    str_concat((char*)(t_data_str+i),"Date: ",6);
    i +=6;
    i += date_to_string(t_data.day, t_data_str+i);
   
    //Min
    
    //*(t_data_str+i) = "Min: ";
    str_concat((char*)(t_data_str+i),"Min : ",6);
    i +=6;
    double_to_str(t_data_str+i,t_data.min,1);
    i += STR_DOUBLE_SIZE;
    
    //Min time
    //*(t_data_str+i) = " - ";
    str_concat((char*)(t_data_str+i)," - ",3);
    i +=3;
    i += time_hm_to_string(t_data.tmin,t_data_str+i);
    
    //Avg
    //*(t_data_str+i) = "Avg: ";
    str_concat((char*)(t_data_str+i),"Avg : ",6);
    i +=6;
    double_to_str(t_data_str+i,t_data.avg,1);
    i += STR_DOUBLE_SIZE;
        
    //Max
    //*(t_data_str+i) = "Max: ";
    str_concat((char*)(t_data_str+i),"Max : ",6);
    i +=6;
    double_to_str(t_data_str+i,t_data.max,1);
    i += STR_DOUBLE_SIZE;
        
    //Max time
    //*(t_data_str+i) = " - ";
    str_concat((char*)(t_data_str+i)," - ",3);
    i +=3;
    i += time_hm_to_string(t_data.tmax,t_data_str+i);

    //variance
    //*(t_data_str+i) = "Var: ";
    str_concat((char*)(t_data_str+i),"Var : ",6);
    i +=6;
    double_to_str(t_data_str+i,t_data.vari,1);
    i += STR_DOUBLE_SIZE;
    
    return i;
    
}