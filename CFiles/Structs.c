/**
* @file
*     Structs.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/

typedef struct screen_cordinate
{
    int pos;
    int screen_half_val;
}screen_cord;

typedef struct  screen_element{
    struct screen_cordinate screen_cord; 
    int l;
    char* text;
}screen_element;

typedef struct date{
    int year;
    int month;
    int day;
}date;

typedef struct timestamp_minute{
    int hour;
    int minute;
}time_hm;

typedef struct timestamp_second{
    int hour;
    int minute;
    int second;
}time_hms;

typedef struct day_temp_data{
    date        day;        //The date of the data represented 
    double      min;        //Minimum temperature during the day
    time_hm     tmin;       //Timestamp where the minimum value was recorded
    double      avg;        //Average temperature during the day
    double      max;        //Maximum temperature during the day
    time_hm     tmax;       //Timestamp where the maximum value was recorded 
    double      vari;       //Temperature variance during the day 
}day_temp_data;

typedef struct User{
    char* username;
    int u_len;
    unsigned char* password;
    int p_len;
    int acess_level;  //0 = normal, 1= admin
}user;

typedef struct User_List{
    struct User* this_user;
    struct User_List* next_user;
}user_list;

typedef unsigned char Date[3];
typedef unsigned char Time[3];
typedef unsigned char Time_hm[2];
typedef unsigned char* temp_d;


typedef struct Linked_List* node;
struct Linked_List{
    node next;
    temp_d data;
};


typedef struct Linked_Element* linked_node;
struct Linked_Element{
    linked_node next;
    unsigned char hour;
    unsigned char min;
    unsigned char temp;
    
};


