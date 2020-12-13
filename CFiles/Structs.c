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
    date        day;        //the date of the data represented 
    double      min;        //Minimum temperature during the day
    time_hm     tmin;       //Timestamp where the minimum valuse was recorded;
    double      avg;        //Avrage temperature during the day
    double      max;        //Maximum temperature during the day
    time_hm     tmax;       //Timestamp where the maximum valuse was recorded; 
    double      vari;       //Temperature variance during the day 
}day_temp_data;
