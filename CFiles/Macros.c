
#pragma CommonFunctions 
//Used in day_temp_data_to_string for Req.3
#define STR_DATE_SIZE       (8) // eg. "11/11/11" 
#define STR_DOUBLE_SIZE     (4) //10^1 + comma + 1 decimal
#define STR_TIME_SIZE       (5) // eg. "23:59" 
#define STR_PADDING_SIZE    (1) // '|'
#define DAY_TEMP_PADDING    ('|')
#define DAY_TEMP_DATA_LENGTH (STR_DATE_SIZE+4*(STR_DOUBLE_SIZE)+2*(STR_TIME_SIZE)+6*STR_PADDING_SIZE)



#pragma endregion CommonFunctions

#pragma  

#pragma endregion CommonFunctions