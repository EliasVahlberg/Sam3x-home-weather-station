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
#define STR_PADDING_SIZE    (2)

#define DAY_TEMP_DATA_LENGTH (STR_DATE_SIZE+4*(STR_DOUBLE_SIZE)+2*(STR_TIME_SIZE)+6*STR_PADDING_SIZE)

#define TABLE_WIDTH         (6)
#define TABLE_X_Pos         (0)
#define STR_ROW_LENGTH      (9+(STR_DOUBLE_SIZE+TABLE_PADDING_SIZE)*(TABLE_WIDTH-1)-TABLE_PADDING_SIZE) // date size = 8 , + 1 for space, -padding for the last one

#pragma endregion Variables/Parameters

#pragma region Functions

#pragma endregion Functions

// {{day , min , time_min , avg , max, time_max ,var},{...} ...}
//day : [0][n], min : [1][n], tmin : [2][n], avg : [3][n], max : [4][n], tmax : [5][n], var : [6][n]
//date        , double      , time         , double      , double      , time         , double




/*void print_temp_table(double[][]table_data,int table_height)
{

    for (size_t i = 0; i < table_height; i++)
    {
        char row_str[STR_ROW_LENGTH];    
        char double_str[STR_DOUBLE_SIZE+TABLE_PADDING_SIZE];
        char padding[TABLE_PADDING_SIZE];
        for (int int j = 0; j < TABLE_WIDTH-1; j++)
        {
            padding = ", ";
            snprintf(double_str, STR_DOUBLE_SIZE, "%f",table_data[i][j]);
            if(j-1!=TABLE_WIDTH)
                {strcat(double_str, padding);
            strcat(row_str,double_str);
        }

    }
    
}*/
char* day_temp_data_to_string(day_temp_data t_data)
{
    char* t_data_str    = (char*)malloc(sizeof(char)*DAY_TEMP_DATA_LENGTH);
    char* temp_double   = (char*)malloc(sizeof(char)*STR_DOUBLE_SIZE);
    if(t_data_str == NULL ||temp_double == NULL)
        return NULL;
    char* temp_date     = date_to_string(t_data.day);
    char* temp_time     = time_hm_to_string(t_data.tmin);
    char* padding       = ", ";

    strcat(t_data_str,temp_date);
    strcat(t_data_str,padding);

    snprintf(temp_double,STR_DOUBLE_SIZE,"%f",t_data.min);
    strcat(t_data_str,temp_double);
    strcat(t_data_str,padding);

    strcat(t_data_str,temp_time);
    strcat(t_data_str,padding);

    snprintf(temp_double,STR_DOUBLE_SIZE,"%f",t_data.avg);
    strcat(t_data_str,temp_double);
    strcat(t_data_str,padding);
    
    snprintf(temp_double,STR_DOUBLE_SIZE,"%f",t_data.max);
    strcat(t_data_str,temp_double);
    strcat(t_data_str,padding);
    
    temp_time = time_hm_to_string(t_data.tmax);
    strcat(t_data_str,temp_time);
    strcat(t_data_str,padding);

    snprintf(temp_double,STR_DOUBLE_SIZE,"%f",t_data.vari);
    strcat(t_data_str,temp_double);
    strcat(t_data_str,padding);
    
    
    free(temp_date);
    free(temp_double);
    free(temp_time);

    
    return t_data_str;
    
}
char* date_to_string(date d)
{
    char* str  = (char*)malloc(sizeof(char)*STR_DATE_SIZE);
    char* temp = (char*)malloc(sizeof(char)*2);
    if(str == NULL || temp == NULL)
        return NULL;
    
    char slash = '/';
    snprintf(str,2,"%d",d.day);
    strcat(str,slash);

    snprintf(temp,2,"%d",d.month);
    strcat(str,temp);
    strcat(str,slash);
    
    snprintf(temp,2,"%d",d.year);
    strcat(str,temp);
    free(temp);
    return str;
    

}

char* time_hm_to_string(time_hm t)
{
    char* str   = (char*)malloc(sizeof(char)*STR_TIME_SIZE);
    char* temp  = (char*)malloc(sizeof(char)*2);
    if(str == NULL || temp == NULL)
    return NULL;
    char comma = ':';

    snprintf(temp,2,"%d",t.hour);
    strcat(str,temp);
    strcat(str,comma);

    snprintf(temp,2,"%d",t.minute);
    strcat(str,temp);
    free(temp);
    return str;
}