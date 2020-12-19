/**
* @file
*     LinkedList.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
#pragma region functions
node create_node();
node create_node_with_data(temp_d data);
void append_node(node tail, node newnode);
void delete_first_node(node head);
unsigned char double_to_temp(double d);
temp_d create_temp_time_data(unsigned char temp, Time time);
double list_avg(node head);
#pragma endregion functions

void expand_List(node head, node tail, double temp)
{
    set_timedate();
    unsigned char temp_char = double_to_temp(temp);
    temp_d data = create_temp_time_data(temp_char, current_time_hm);
    if(head->data==NULL)
    {
            head->data = data;
            return;
    }
    else if(tail->data==NULL)
    {
        tail->data = data;
        head->next = tail;
        return;
        
    }
    else 
    {
        node newnode = create_node_with_data(data);
        append_node(head,newnode);
        
    }
}

node create_node()
{
    node temp;
    temp = calloc(1,sizeof(struct Linked_List));
    if(temp == NULL)
        return NULL;
    temp->next = NULL;
    temp->data = NULL;
    return temp;
} 

node create_node_with_data(temp_d data)
{
    node temp;
    temp = calloc(1,sizeof(struct Linked_List));
    while(temp == NULL)
    {
        return NULL;
     //   delete_first_node(head); Does not yet work
     //   temp = calloc(1,sizeof(struct Linked_List));
    }
    temp->next = NULL;
    temp->data = data;
    return temp;
}

void append_node(node head, node newnode)
{
    node tempnode = head;
    while(tempnode->next != NULL)
        tempnode = tempnode->next;

    tempnode->next = newnode;
}

void delete_first_node(node head_l)
{
    node temp = head_l;
    head_l = head_l->next;
    free(temp->data);
    free(temp);
}

unsigned char double_to_temp(double d)
{
    int n = (int)(d*10) - TEMP_D_OFFSET;
    n = (n>TEMP_D_MAX)?TEMP_D_MAX:n;
    unsigned char temp = (unsigned char) n;
    return temp;

}

temp_d create_temp_time_data(unsigned char temp, Time_hm time)
{
    temp_d dest = calloc(3,sizeof(char));
    dest[0]=temp;
    dest[1]=time[0];
    dest[2]=time[1];
    return dest;
}

double temp_to_double(unsigned char uc)
{
    
    double d = ((double)((int)uc + TEMP_D_OFFSET))/10; 
    return d;
}

double list_avg(node head)
{
    if(head == NULL)
        return 0.0;
    if(head->data == NULL)
        return 0.0;
    node temp_node = head;
    double avg = 0.0;
    int i = 0;
    while(1)
    {
        avg += temp_to_double(temp_node->data[0]);
        i++;
        if(temp_node->next!=NULL)
            {
                if(temp_node->next->data!=NULL)
                    temp_node = temp_node->next;
                else
                    break;
            }
        else
            break; 
    }
    avg /= i;
    return avg;

    
}