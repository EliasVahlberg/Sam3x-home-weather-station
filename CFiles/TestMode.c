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
    test_day_temp_data = calloc(7,sizeof(day_temp_data));
    day_temp_data* dtd;
    int n;
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
        num_saved_day__test_data++;
        }
}
