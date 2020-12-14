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
#define DAY_TEMP_DATA_LENGTH (STR_DATE_SIZE+4*(STR_DOUBLE_SIZE)+2*(STR_TIME_SIZE)+6*STR_PADDING_SIZE)

#pragma endregion CommonFunctions

#pragma  region ArduinoBaseInclude
#define SYSTICK_CONFIG_MICRO    (84) //Systick calibration value for 1us period (MCKf / 1000000)
#pragma endregion ArduinoBaseInclude
#pragma region TemperatureSensor
     
#define TEMP_ALARM_UPPER_VALUE           (30.0                                      ) //
#define TEMP_ALARM_LOWER_VALUE           (15.0                                      ) // 
#define TEMP_ALARM_TYPE                  (2                                         ) //0: Display alarm, 1: LED alarm, 2: both
#define TEMP_ALARM_RECHECK               (5                                         ) //The number of times it rechecks the temperature (to mitigate alarms caused by bugs)


#define TEMP_ALARM_LED_PIO_PMC_BIT       (14                                        )
#define TEMP_ALARM_LED_PIO_ADRESS_ENABLE (AT91_CAST(AT91_REG *) 	AT91C_PIOD_PER  )
#define TEMP_ALARM_LED_PIO_ADRESS_OUTPUT (AT91_CAST(AT91_REG *) 	AT91C_PIOD_OER  )
#define TEMP_ALARM_LED_PIO_ADRESS_SET    (AT91_CAST(AT91_REG *) 	AT91C_PIOD_SODR )
#define TEMP_ALARM_LED_PIO_ADRESS_CLEAR  (AT91_CAST(AT91_REG *) 	AT91C_PIOD_CODR )
#define TEMP_ALARM_LED_BIT               (1                                         )

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
#define DISPLAY_DELAY_VALUE                    (100         ) //Used in the write data and write command function, value obtained from testing
#define DISPLAY_SCREEN_HALF                    (240         ) //Used in the covert_to_scord function, defines the second input to the set cursor command

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
#pragma endregion  LightSensor
