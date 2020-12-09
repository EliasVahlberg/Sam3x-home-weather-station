/**
* @file
*     main.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
#pragma region Variables/Parameters
#define TABLE_WIDTH         (6)
#define STR_DOUBLE_SIZE     (4) //10^1 + comma + 1 decimal
#define TABLE_PADDING_SIZE  (2)
#define TABLE_X_Pos         (0)
#define STR_ROW_LENGTH      (9+(STR_DOUBLE_SIZE+TABLE_PADDING_SIZE)*(TABLE_WIDTH-1)-TABLE_PADDING_SIZE) // date size = 8 , + 1 for space, -padding for the last one

#pragma endregion Variables/Parameters

#pragma region Functions

#pragma endregion Functions

// {{day , min , time_min , avg , max, time_max ,var},{...} ...}
//day : [0][n], min : [1][n], tmin : [2][n], avg : [3][n], max : [4][n], tmax : [5][n], var : [6][n]
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
