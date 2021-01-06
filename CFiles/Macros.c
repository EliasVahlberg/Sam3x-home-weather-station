/**
* @file
*     Macros.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/

#pragma region CommonFunctions 
//Used in day_temp_data_to_string for Req.3
#define STR_DATE_SIZE       (8) // eg. "11/11/11" 
#define STR_DOUBLE_SIZE     (4) //10^1 + comma + 1 decimal
#define STR_TIME_SIZE       (5) // eg. "23:59" 
#define STR_PADDING_SIZE    (1) // '|'
#define DAY_TEMP_PADDING    ('|')
//Width of each line in the day_temp_data
#define DAY_LINE0 17        //Date row    
#define DAY_LINE1 14        //Date row    
#define DAY_LINE2 18        //Min row
#define DAY_LINE3 10        //Avg row
#define DAY_LINE4 18        //Max row
#define DAY_LINE5 10         //Var row

#define DAY_TEMP_DATA_LENGTH (DAY_LINE1+DAY_LINE2+DAY_LINE3+DAY_LINE4+DAY_LINE5)
#pragma endregion CommonFunctions

#pragma  region ArduinoBaseInclude
#define SYSTICK_CONFIG_MICRO    (84) //Systick calibration value for 1us period (MCKf / 1000000)
#pragma endregion ArduinoBaseInclude

#pragma region TemperatureSensor
     
#define TEMP_ALARM_UPPER_VALUE           (30.0                                      ) //
#define TEMP_ALARM_LOWER_VALUE           (15.0                                      ) // 
#define TEMP_ALARM_RECHECK               (5                                         ) //The number of times it rechecks the temperature (to mitigate alarms caused by bugs)


#define TEMP_ALARM_DISPLAY_MES_LOW       ("TEMPERATURE IS TOO LOW "                 ) // 
#define TEMP_ALARM_DISPLAY_MES_HIGH      ("TEMPERATURE IS TOO HIGH"                 ) // 
#define TEMP_ALARM_DISPLAY_LENGTH        (23                                        ) //
#define TEMP_ALARM_DISPLAY_X             (4                                         ) //
#define TEMP_ALARM_DISPLAY_Y             (5                                         ) // 

#pragma endregion TemperatureSensor

#pragma region keypad  
#define ROWS    (4)
#define COLS    (3)
#define OE      (1<<2)
#define COL0    (1<<7)
#define COL1    (1<<8)
#define COL2    (1<<9)
#define ROW0    (1<<2)
#define ROW1    (1<<3)
#define ROW2    (1<<4)
#define ROW3    (1<<5)   

#pragma endregion keypad

#pragma region  display
#define DISPLAY_SET_CURSOR_CMD                 (0x24        ) //Sets the cursor on the display, position is given by the two prior WriteData calls
#define DISPLAY_WRITE_AND_INCREMENT_CURSOR_CMD (0xc0        ) //Sets the cursor on the display, position is given by the two prior WriteData calls
#define DISPLAY_DELAY_VALUE                    (25          ) //Used in the write data and write command function, value obtained from testing
#define DISPLAY_SCREEN_HALF                    (225         ) //Used in the covert_to_scord function, defines the second input to the set cursor command

#define RESET_DISP                             (1           ) //DP 25 PIOD
#define B4_DISP                                (1       <<6 ) //DP 38
#define OE                                     (1       <<2 )//
#define RV_DISP                                (1       <<19) //DP 44 PIOC
#define FS_DISP                                (1       <<18) //DP 45 PIOC
#define WR_DISP                                (1       <<17) //DP 46 PIOC
#define RD_DISP                                (1       <<16) //DP 47 PIOC
#define CE_DISP                                (1       <<15) //DP 48 PIOC
#define CD_DISP                                (1       <<14) //DP 49 PIOC
#define DIR_DISP                               (1       <<13) //DP 50 PIOC
#define OE_DISP                                (1       <<12) //DP 51 PIOC
#define BUS_DISP                               (0x3FC       ) //DP 34-41
#define CONFIG_DISP                            (0xff000     ) //DP 44-51
#define DISP_X_MAX                             (30          ) //
#define DISP_Y_MAX                             (16          ) //
#pragma endregion display

#pragma region  LightSensor
#define LIGHT_ROT_DATA_LENGTH  (36)
#define SERVO_UPDATE_PERIOD    (10) //How often the servo should update the position
#pragma endregion  LightSensor

#pragma region LoginSystem



#define LOGIN_USERNAME_X_POS    (0)
#define LOGIN_PASSWORD_X_POS    (0)
#define LOGIN_USERNAME_Y_POS    (3)
#define LOGIN_PASSWORD_Y_POS    (5)
#define LOGIN_USERNAME_MAX_LEN  (21)
#define LOGIN_PASSWORD_MAX_LEN  (21)
#define HASHED_PASS_LEN         (32)

#pragma endregion LoginSystem

#pragma region DateTime
#define MICRO_TO_SEC         (1000000)
#define MICRO_TO_SEC_FAST    (16666) //Approx 10^6/60
#define SEC_TO_MIN           (60)
#define MIN_TO_HOUR          (60)
#define HOUR_TO_DAY          (24)
#define YEAR_OFFSET          (2000)
#define MONTH_TO_YEAR        (12)
#define LEAP_YEAR_CHECK(Y) ( (((Y%4==0)&&(Y%100 != 0))||(Y%400 == 0))?1:0)

#define TIME_POS_X            (22)            
#define TIME_POS_Y            (0)
#define TIME_LEN              (8)

#define DATE_POS_X            (20)
#define DATE_POS_Y            (1)
#define DATE_LEN              (10)  
#define INT_TO_CHAR           (48)

#pragma endregion DateTime

#pragma region LinkedList
#define TEMP_D_OFFSET        (100)
#define TEMP_D_MAX            (255)
#pragma endregion LinkedList
