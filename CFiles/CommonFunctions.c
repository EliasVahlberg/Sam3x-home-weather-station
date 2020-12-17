/**
* @file
*     CommonFunctions.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     
*/

#pragma region Functions
int day_temp_data_to_string(day_temp_data t_data,char* t_data_str);
int date_to_string(date d,char* str);
int time_hm_to_string(time_hm t,char* str);
void reverse_str(char* arr, int len);
int int_to_str(char* arr, int zeroes, int n);
void int_to_str_fixed_length(char* arr, int length, int n);
int double_to_str(char* arr,double d,int decimal_precision);
void double_to_str_fixed_length(char* arr,double d,int length);
void str_concat(char* dest,char* src, int length);
void char_array_erase(char* arr,int length, char erase_val);
#pragma endregion Functions


//day : [0][n], min : [1][n], tmin : [2][n], avg : [3][n], max : [4][n], tmax : [5][n], var : [6][n]
//date        , double      , time         , double      , double      , time         , double
int day_temp_data_to_string(day_temp_data t_data,char* t_data_str)
{
    int i = 0;
    char temp_double[STR_DOUBLE_SIZE];
    
    //Date + Padding
    i += date_to_string(t_data.day, t_data_str);
    t_data_str[i++] = DAY_TEMP_PADDING;
    //Min + padding
    i += double_to_str(t_data_str+i,t_data.min,1);
    t_data_str[i++] = DAY_TEMP_PADDING;
    //Min time + padding
    i += time_hm_to_string(t_data.tmin,t_data_str+i);
    t_data_str[i++] = DAY_TEMP_PADDING;
    //Avg + padding
    i += double_to_str(t_data_str+i,t_data.avg,1);
    t_data_str[i++] = DAY_TEMP_PADDING;    
    //Max + padding
    i += double_to_str(t_data_str+i,t_data.max,1);
    t_data_str[i++] = DAY_TEMP_PADDING;    
    //Max time + padding
    i += time_hm_to_string(t_data.tmax,t_data_str+i);
    t_data_str[i++] = DAY_TEMP_PADDING;
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

void int_to_str_fixed_length(char* arr, int length, int n)
{
    int i = 0;
    while(n)
    {
        if(i==length)
            break;
        arr[i++] = (n % 10)+'0';
        n = n/10;
    }
    while(i<length)
        arr[i++]='0';

    reverse_str(arr,i);
    arr[i] = '\0';
    return;
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
        //fill the rest of the array
        
        i += int_to_str(arr+i,decimal_precision,(int)double_part);
    }
    return i;
}

void double_to_str_fixed_length(char* arr,double d,int length)
{
    //Extracts the integer part of the double
    int int_part = (int)d;
    int c=0;
    while (int_part)
    {
        int_part = int_part/10;
        c++;
    }
    c = (c<(length-1))?c:length;
    int_part = (int)d;
    //Extracts the decimal part of the double
    double double_part = d-(double)int_part;
    //Start filling array with the digits
    int_to_str_fixed_length(arr,c,int_part);
    //If the double consists of a decimal part    
    arr[c++]='.';
    if(length>=c)
    {
        for (int j = 0; j < (length-c); j++)
        {
        double_part = double_part*10;
        }
        //fill the rest of the array
        
        int_to_str_fixed_length(arr+c,length-c,(int)double_part);
    }
    return;
}

void str_concat(char* dest,char* src, int length)
{
    for (int i = 0; i < length; i++)    
    {
        dest[i] = src[i];
    }
}

void char_array_erase(char* arr,int length, char erase_val)
{
    for (int i = 0; i < length; i++)
    {
        arr[i]=erase_val;
    }
    
}
