

/**
* @file
*     main.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
#pragma region functions
void init_date_time();
void display_date_time();
void config_date();
void config_time();
void time_to_str(Time time, char *str);
void date_to_str(Date date, char *str);
#pragma endregion functions
void init_date_time()
{
    time_config_flag = 0;
}
void set_timedate()
{
    time_config_flag = 1;
    unsigned long long temp_seconds = (unsigned long long)microseconds / MICRO_TO_SEC;
    current_time[2] = (unsigned char)(temp_seconds % SEC_TO_MIN);
    temp_seconds = temp_seconds / SEC_TO_MIN;
    current_time[1] = (unsigned char)(temp_seconds % MIN_TO_HOUR);
    temp_seconds = temp_seconds / MIN_TO_HOUR;
    current_time[0] = (unsigned char)(temp_seconds % HOUR_TO_DAY);
    temp_seconds = temp_seconds / HOUR_TO_DAY;
    if (current_time[0] >= HOUR_TO_DAY)
    {
        microseconds = 0;
        int max_day = (current_date[1] != 2 || LEAP_YEAR_CHECK(current_date[0]) != 1) ? month_days[current_date[1]] : leap_feb;
        if ((current_date[2] + 1) % max_day == 0)
        {
            if ((current_date[1] + 1) % MONTH_TO_YEAR == 0)
            {
                current_date[0] = (unsigned char)(current_date[0] + 1) % 255;
            }
            current_date[1] = (unsigned char)(current_date[1] + 1) % MONTH_TO_YEAR;
        }
        current_date[2] = (unsigned char)(current_date[2] + 1) % max_day;
    }
    time_config_flag = 0;
}
void display_date_time()
{
    time_to_str(current_time, time_str);
    screen_time = create_screen_element(TIME_POS_X, TIME_POS_Y, TIME_LEN, time_str);
    date_to_str(current_date, date_str);
    screen_date = create_screen_element(DATE_POS_X, DATE_POS_Y, DATE_LEN, date_str);
    display_write(screen_time);
    display_write(screen_date);
}
void config_date()
{
    delay_micro(20);
    date_to_str(zero_date, date_str);
    screen_date = create_screen_element(DATE_POS_X, DATE_POS_Y, DATE_LEN, date_str);
    display_write(screen_date);
    int d = 0, m = 0, y = 0, k = 0, i = 0, zeroset = 0;
    do
    {
        delay_micro(50);
        k = get_keypad_key();
        if(k==0)
        {
            zeroset=1;
            delay_micro(50);
            continue;
        }
        else if(zeroset==1)
        {
            k = (k>=11)?0:k;
            zeroset=0;
           switch (i)
            {
            case 0:d=10*k;  break;
            case 1:d+=k;    break;
            case 2:m=10*k;  break;
            case 3:m+=k;    break;
            case 4:y=10*k;  break;
            case 5:y+=k;    break;
            default:        break;
            }
            date_str[(i > 1) ? ((i > 3) ? i + 4 : i + 1) : i] = (char)(k + INT_TO_CHAR);
            display_write(screen_date);
            i++;
        }
    
    } while (i < 6);
    int maxday = (LEAP_YEAR_CHECK(y+YEAR_OFFSET)&&m==2)?leap_feb:month_days[m];
    d = (d>maxday)?0:d;
    m = (m>MONTH_TO_YEAR)?0:m;

    current_date[2] = (unsigned char)d;
    current_date[1] = (unsigned char)m;
    current_date[0] = (unsigned char)y;
}
void config_time()
{
    time_config_flag = 1;
    time_to_str(zero_time, time_str);
    screen_time = create_screen_element(TIME_POS_X, TIME_POS_Y, TIME_LEN, time_str);
    display_write(screen_time);
    int s = 0, m = 0, h = 0, k = 0, i = 0, zeroset = 0;
    do
    {
        int pow = 1;
        delay_micro(50);
        k = get_keypad_key();
        if (k == 0)
        {
            zeroset = 1;
            delay_micro(50);
            continue;
        }
        if (zeroset == 1)
        {
            zeroset = 0;
            k = (k >= 11) ? 0 : k;
            switch (i)
            {
            case 0:h=10*k;  break;
            case 1:h+=k;    break;
            case 2:m=10*k;  break;
            case 3:m+=k;    break;
            case 4:s=10*k;  break;
            case 5:s+=k;    break;
            default:        break;
            }
            time_str[(i > 1) ? ((i > 3) ? i + 2 : i + 1) : i] = (char)(k + INT_TO_CHAR);
            display_write(screen_time);
            i++;
        }
    } while (i < 6);
    s = (s >= SEC_TO_MIN) ? 0 : s;
    m = (m >= MIN_TO_HOUR) ? 0 : m;
    h = (h >= HOUR_TO_DAY) ? 0 : h;

    microseconds = ( unsigned long long)(MICRO_TO_SEC * (( unsigned long long)s + SEC_TO_MIN * (m + h * MIN_TO_HOUR)));
    delay_micro(20);
    time_config_flag = 0;
}
void time_to_str(Time time, char *str)
{
    int i = 0;
    i += int_to_str(str, 2, ((int)time[0]));
    str[i++] = ':';
    i += int_to_str(str + i, 2, ((int)time[1]));
    str[i++] = ':';
    i += int_to_str(str + i, 2, ((int)time[2]));

    return;
}
void date_to_str(Date date, char *str)
{
    int i = 0;
    i += int_to_str(str, 2, ((int)date[2]));
    str[i++] = '/';
    i += int_to_str(str + i, 2, ((int)date[1]));
    str[i++] = '/';
    i += int_to_str(str + i, 4, ((int)date[0] + YEAR_OFFSET));
    return;
}