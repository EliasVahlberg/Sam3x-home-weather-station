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
linked_node create_node_with_data(double temp, linked_node* head);

void append_to_list(linked_node* head, linked_node* tail, double temp);
void delete_first_node(linked_node* head_l);

#pragma endregion functions


void append_to_list(linked_node* head, linked_node* tail, double temp)
{
    if(*head==NULL)
        {
            *head = create_node_with_data(temp,head);
            *tail = *head;
            return;
        }
    (*tail)->next = create_node_with_data(temp,head);
    (*tail) = (*tail)->next;
}
linked_node create_node_with_data(double temp,linked_node* head)
{
    linked_node nod = calloc(1,sizeof(linked_node));
    while(nod == NULL) //Excecutes when memory is full (malloc failure)
    {
        mem_full_flag = 1;
        update_statistics(head,saved_day_temp_data);
        if(head==NULL)
            return nod;
        do
        {
        delete_first_node(head);
        } while ((*head)->next!=NULL);
        delete_first_node(head);
        
        nod = calloc(1,sizeof(linked_node));
    }
    nod->temp = double_to_temp(temp);
    nod->hour = current_time_hm[0];
    nod->min = current_time_hm[1];
    nod->next = NULL;

    list_size++;
    return nod;

}
/**
* delete_first_node 
* \brief deletes the first node in the list and changes the head value
* @param head_l first node of the list
*/
void delete_first_node(linked_node* head_l)
{
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