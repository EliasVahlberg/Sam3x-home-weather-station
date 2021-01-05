/**
* @file
*     CommonFunctions.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief A collection of non-specific functions used in other parts of the system 
          mainly regarding string, double and char conversions along with some struct conversions.
*     
*/

#pragma region Functions
int             date_to_string             (date d,                char* str                               );
int             time_hm_to_string          (time_hm t,             char* str                               );
void            reverse_str                (char* arr,             int len                                 );
int             int_to_str                 (char* arr,             int zeroes,     int n                   );
void            int_to_str_fixed_length    (char* arr,             int length,     int n                   );
int             double_to_str              (char* arr,             double d,       int decimal_precision   );
void            double_to_str_fixed_length (char* arr,             double d,       int length              );
void            str_concat                 (char* dest,            char* src,      int length              );
void            char_array_erase           (char* arr,             int length,     char erase_val          );
unsigned char   double_to_temp             (double d                                                       );
double          temp_to_double             (unsigned char uc                                               );
#pragma endregion Functions



/**
* date_to_string
* \brief Converts a struct date to a string with the format dd/mm/yyyy
* @param d source of the struct date 
* @param str the destination of the string 
* @return the added offset used in further appending
*/
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

/**
* time_hm_to_string 
* \brief Converts a struct time_hm (hm = hour:minute) to a string with the format hh:mm
* @param t source of the struct time_hm
* @param str the destination of the string 
* @return the added offset used in further appending
*/
int time_hm_to_string(time_hm t,char* str)
{
    int i = 0;
    i += int_to_str(str,2,t.hour);
    str[i++]=':';
    i += int_to_str(str+i,2,t.minute);

    return i;
}

/**
* reverse_str 
* \brief Reverses an array of chars (used in int/double string conversion)
* @param arr the distination of the string
* @param len the length of the string in arr
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
* int_to_str
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
* int_to_str_fixed_length
* \brief Works as int_to_str the difference beeing that if the int is longer than length it is cut off highest digit first
*         e.g. [length = 3, n = 12345, arr: "345"]
* @param arr the allready initialized char array
* @param length the number of digits
* @param n the integer to be converted
*/
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
* double_to_str 
* \brief Utilizes the int_to_str and to convert a double value to an array of chars
* @param arr the allready initialized char array (used as destination for the output)
* @param d double value to be converted
* @param decimal_precision number of digits after the decimal point to be converted
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

/**
* double_to_str_fixed_length 
* \brief Works the same as double_to_str the diffrence being that the whole length is fixed and will cut off highest digit first then smallest decimal
* e.g. [length = 2, d = 123.45, arr: "23"], [length = 4, d = 123.45, arr:"123."]
* @param arr the allready initialized char array (used as destination for the output)
* @param d double value to be converted
* @param length number of symbols to be converted (decimal point included)
*/
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

/**
* str_concat 
* \brief Concatenates two string (char arrays) from the adress given in dest 
* @param dest the destination adress from where to begin appending
* @param src source of the string to be concatenated
* @param length number of symbols to be concatenated
*/
void str_concat(char* dest,char* src, int length)
{
    for (int i = 0; i < length; i++)    
    {
        dest[i] = src[i];
    }
}

/**
* char_array_erase 
* \brief sets all elements in the char array to the value erase_val effectively erasing the array from memory 
* @param arr source of the string to be erased
* @param length number of symbols to erased
* @param erase_val the value set to all array elements in arr (most commonly "\0" i.e. integer 0)
*/
void char_array_erase(char* arr,int length, char erase_val)
{
    for (int i = 0; i < length; i++)
    {
        arr[i]=erase_val;
    }
    
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
    n = (n>TEMP_D_MAX)?TEMP_D_MAX:
    (n<0)?0:
    n;
    unsigned char temp = (unsigned char)(n &0xff);
    return temp;

}
/**
* \brief The inverse function to double_to_temp
* @param uc unsigned char temp value
* @return temp as a double
*/
double temp_to_double(unsigned char uc)
{
    
    double d = ((double)((int)uc + TEMP_D_OFFSET))/10; 
    return d;
}


