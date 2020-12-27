/**
* @file
*     LinkedList.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     A simple singly linked list implementation 
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


/**
* \brief Summary
* @param input1
* @param input2
* @return value returned
*/

/**
* expand_List
* \brief Expands the list with the given head/tail by converting and appending
*  the temp value. Uses double_to_temp(), create_temp_time_data(),
*  create_node_with_data(), append_node().  
* @param head First node of the linked list
* @param tail Last node of the linked list
* @param temp temp data to append
*
*/
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


/**
* create_node
* \brief allocates space for a node and initialize all values as NULL, then returns said node
* @return empty allocated node
*/
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

/**
* create_node_with_data
* \brief Works the same as create_node(), diffrence being that the data is set.
* @param data struct temp_d containing temperature and timestamp.
* @return allocated node containing the input data.
*/
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

/**
* append_node
* \brief appends newnode at the first NULL pointer next value in the 
*  list starting with head.
* @param head Starting node of the list.
* @param newnode Node to be appended.
*/
void append_node(node head, node newnode)
{
    node tempnode = head;
    while(tempnode->next != NULL)
        tempnode = tempnode->next;

    tempnode->next = newnode;
}

/**
* delete_first_node 
* \brief deletes the first node in the list and changes the head value
* @param head_l first node of the list
*/
void delete_first_node(node head_l)
{
    node temp = head_l;
    head_l = head_l->next;
    free(temp->data);
    free(temp);
}

/**
* double_to_temp
* \brief Converts a double temp value (between 10 and 35.5 degrees )
* by multiplying it with 10 (to get 1 decimal precission), 
* converting it to a int and then subtracting with the offset.
* Example: [d = 21.5141, return = (char)(d*10-100) = 115 ]
* @param d  temp to be converted
* @param input2
* @return value returned
*/
unsigned char double_to_temp(double d)
{
    int n = (int)(d*10) - TEMP_D_OFFSET;
    n = (n>TEMP_D_MAX)?TEMP_D_MAX:n;
    unsigned char temp = (unsigned char) n;
    return temp;

}

/**
* \brief Summary
* @param input1
* @param input2
* @return value returned
*/
temp_d create_temp_time_data(unsigned char temp, Time_hm time)
{
    temp_d dest = calloc(3,sizeof(char));
    dest[0]=temp;
    dest[1]=time[0];
    dest[2]=time[1];
    return dest;
}

/**
* \brief Summary
* @param input1
* @param input2
* @return value returned
*/
double temp_to_double(unsigned char uc)
{
    
    double d = ((double)((int)uc + TEMP_D_OFFSET))/10; 
    return d;
}

/*Temporary!*/
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