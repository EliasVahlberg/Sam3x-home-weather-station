/**
* @file
*     CommonFunctions.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
#pragma region Variables/Parameters

#define STR_DATE_SIZE       (8) // eg. "11/11/11" 
#define STR_DOUBLE_SIZE     (4) //10^1 + comma + 1 decimal
#define STR_TIME_SIZE       (5) // eg. "23:59" 
#define STR_PADDING_SIZE    (1)

#define DAY_TEMP_DATA_LENGTH (STR_DATE_SIZE+4*(STR_DOUBLE_SIZE)+2*(STR_TIME_SIZE)+6*STR_PADDING_SIZE)

#pragma endregion Variables/Parameters

#pragma region Functions
int day_temp_data_to_string(day_temp_data t_data,char* t_data_str);
int date_to_string(date d,char* str);
int time_hm_to_string(time_hm t,char* str);
void reverse_str(char* arr, int len);
int int_to_str(char* arr, int zeroes, int n);
int double_to_str(char* arr,double d,int decimal_precision);

#pragma endregion Functions

// {{day , min , time_min , avg , max, time_max ,var},{...} ...}
//day : [0][n], min : [1][n], tmin : [2][n], avg : [3][n], max : [4][n], tmax : [5][n], var : [6][n]
//date        , double      , time         , double      , double      , time         , double


int day_temp_data_to_string(day_temp_data t_data,char* t_data_str)
{
    int i = 0;
    char temp_double[STR_DOUBLE_SIZE];
    char padding = '|';
    
    //Date + Padding
    i += date_to_string(t_data.day, t_data_str);
    t_data_str[i++] = padding;
    //Min + padding
    i += double_to_str(t_data_str+i,t_data.min,1);
    t_data_str[i++] = padding;
    //Min time + padding
    i += time_hm_to_string(t_data.tmin,t_data_str+i);
    t_data_str[i++] = padding;
    //Avg + padding
    i += double_to_str(t_data_str+i,t_data.avg,1);
    t_data_str[i++] = padding;    
    //Max + padding
    i += double_to_str(t_data_str+i,t_data.max,1);
    t_data_str[i++] = padding;    
    //Max time + padding
    i += time_hm_to_string(t_data.tmax,t_data_str+i);
    t_data_str[i++] = padding;
    //Max variance + padding
    i += double_to_str(t_data_str+i,t_data.vari,1);

    
    return i;
    
}
int date_to_string(date d,char* str)
{
    int i = 0;
    i += int_to_str(str,2,d.day);
    str[i++]='/';
    i += int_to_str(str + i ,2,d.month);
    str[i++]='/';
    i += int_to_str(str + i ,2,d.year);
    
    return i;
    

}

int time_hm_to_string(time_hm t,char* str)
{
    int i = 0;
    i += int_to_str(str,2,t.hour);
    str[i++]=':';
    i += int_to_str(str+i,2,t.minute);

    return i;
}

/**
* \brief Reverses an array of chars (used in int/double string conversion)
* @param arr
* @param len
* @return (the reversed array via the arr variable)
*/
void reverse_str(char* arr, int len) 
{ 
    int i = 0;
    int j = len - 1;
    int temp; 

    while (i < j) { 
        temp = arr[i]; 
        arr[i] = arr[j]; 
        arr[j] = temp; 
        i++; 
        j--; 
    } 
}

/**
* \brief Converts an int to an array of chars with a predetermined length
* @param arr the allready initialized char array
* @param length the number of digits, (0 will convert the whole number, otherwise it will add zeroes iff length>log10(n)) 
* @param n the integer to be converted
* @return the array index value relative to where it started in the function
*/
int int_to_str(char* arr, int length, int n)
{
    int i = 0;
    while(n)
    {
        arr[i++] = (n % 10)+'0';
        n = n/10;
    }
    while(i<length)
        arr[i++]='0';

    reverse_str(arr,i);
    arr[i] = '\0';
    return i;
}

/**
* \brief Utilizes the int_to_str and to convert a double value to an array of chars
* @param arr the allready initialized char array (used as destination for the output)
* @param d double value to be converted
* @param decimal_precision number of digits after the decimal point to be converted
* @return void (the resultion array is stored in arr )
*/
int double_to_str(char* arr,double d,int decimal_precision)
{
    //Extracts the integer part of the double
    int int_part = (int)d;
    //Extracts the decimal part of the double
    double double_part = d-(double)int_part;
    //Start filling array with the digits
    int i = int_to_str(arr,0,int_part);
    //If the double consists of a decimal part    
    if(decimal_precision   !=  0)
    {
        arr[i++]='.';
        //multiplies the decimal part with the approriate base-10 exponential
        for (int j = 0; j < decimal_precision; j++)
        {
        double_part = double_part*10;
        }
        //fille the rest of the array
        
        i += int_to_str(arr+i,decimal_precision,(int)double_part);
    }
    return i;
}

