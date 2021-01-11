/**
* @file
*     SingleLinkedList.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     A simple singly linked list implementation 
*/
//link_node
//Linked_Element



#pragma region functions
unsigned char double_to_temp(double d);
double temp_to_double(unsigned char uc);
void convert_to_circular_list(linked_node* head, linked_node* tail, int size);
void circular_overwrite(linked_node* head, linked_node* tail, unsigned char* temp, int len);
linked_node create_node_with_data(linked_node* head, linked_node* tail, unsigned char* temp, int len);
void delete_first_node(linked_node* head_l);
void mem_write(unsigned char* mem_adr, unsigned char* src, int len);
void get_node_temps(linked_node l_node, unsigned char* dest);
void mem_read(unsigned char* mem_adr, unsigned char* dest, int len);

#pragma endregion functions


void append_to_list(linked_node* head, linked_node* tail, unsigned char* temp, int len)
{
    if(mem_full_flag)
    {
        circular_overwrite(head,tail,temp,len);
        return;
    }
    if(*head==NULL)
        {
            linked_node nod = create_node_with_data(head,tail,temp,len);
            if(nod==NULL)
                return;
            *head = nod;
            *tail = *head;
            return;
        }
    else
    {
        linked_node nod = create_node_with_data(head,tail,temp,len);
        if(nod==NULL)
                return;
        (*tail)->next = nod;
        (*tail) = (*tail)->next;
    }
}

linked_node create_node_with_data(linked_node* head, linked_node* tail, unsigned char* temp, int len)
{
    linked_node nod = (len==1)?
    calloc(1,sizeof(struct Linked_Element)):
    calloc(1,sizeof(struct Linked_Element)+len);
    while(nod == NULL) //Excecutes when memory is full (malloc failure)
    {
        mem_full_flag = 1;
        update_statistics(head,saved_day_temp_data);
        if(head==NULL)
            return nod;
        convert_to_circular_list(head,tail,list_size);
        circular_overwrite(head,tail,temp,len);
        return nod;
    }
    
    nod->hour = current_time_hm[0];
    nod->min = current_time_hm[1];
    nod->temp = temp[0];
    if(len>1)
        mem_write(((unsigned char*)nod)+8,(unsigned char*)(temp + 1),len);
    nod->next = NULL;

    list_size++;
    return nod;

}

void convert_to_circular_list(linked_node* head, linked_node* tail, int size)
{
    maximum_list_size = (int)(size*0.9);//Largest allowed list (0.9*list_size when heap is full to leave some overhead)
    int size_delta = size-maximum_list_size;
    for (int i = 0; i < size_delta; i++)
    {
     delete_first_node(head);
    }
    (*tail)->next = *head;
    circular_start = *tail;
    
}

void circular_overwrite(linked_node* head, linked_node* tail, unsigned char* temp, int len)
{
    if(*head==circular_start)
        update_statistics(head,saved_day_temp_data);
    *head = (*head)->next;
    linked_node nod = (*tail)->next;
    nod->hour = current_time_hm[0];
    nod->min = current_time_hm[1];
    nod->temp = temp[0];
    if(len>1)
        mem_write(((unsigned char*)nod)+8,(unsigned char*)(temp + 1),len);
    *tail = nod;
}
/**
* delete_first_node 
* \brief deletes the first node in the list and changes the head value
* @param head_l first node of the list
*/
void delete_first_node(linked_node* head_l)
{
    list_size--;
    linked_node temp = *head_l;
    (*head_l) = (*head_l)->next;
    free(temp);
}

void delete_list(linked_node* head)
{
    do
        {
        delete_first_node(head);
        } while ((*head)->next!=NULL);
        delete_first_node(head);
}

