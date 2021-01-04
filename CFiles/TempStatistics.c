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
void update_avg_min_max(linked_node* head,day_temp_data* data);
void update_statistics(linked_node* head,day_temp_data* data);
void calc_var(day_temp_data* data);
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
    double min = temp_to_double(temp_node->temp);
    double max = min;
    double avg = min;
    tmin[0] =temp_node->hour;
    tmin[1] =temp_node->min;
    tmax[0] = tmin[0];
    tmax[1] = tmin[1];
    while(1)
    {
        if(temp_node->next==NULL)
            break;
        temp_node = temp_node->next;

        temp_double = temp_to_double(temp_node->temp);
        avg += temp_double;
        var_sum1+=temp_double;
        var_sum2+=temp_double*temp_double;
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