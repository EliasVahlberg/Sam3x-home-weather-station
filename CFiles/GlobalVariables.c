/**
* @file
*     GlobalVariables.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/

#pragma region GlobalFlags
int temp_alarm_flag             = 0;
int display_write_disable_flag  = 0; //Disables the write acces to the display (used during alarm)
#pragma endregion GlobalFlags

#pragma region Servo 
double      servo_position             = 0.0    ; //
int         prev_servo_call            = 0      ;
#pragma endregion Servo

#pragma region ArduinoBaseInclude 
//Used during systick setup and delay operations
uint32_t    syst                       = 1      ; //Systick config status register status, used to indacte that systick was unable to configure
int        systick_delay               = 0      ; //The remaining number of interrupts that need to occur before delay is complete
int        systick_trigger             = 0      ; //If a delay function is currently used with systick
#pragma endregion ArduinoBaseInclude

#pragma region LightSensor 
double light_rotation_data [LIGHT_ROT_DATA_LENGTH]; 
double      adc_CDR1_value             = 0.0    ; //
double      adc_CDR2_value             = 0.0    ; //
double      light_to_volt_coefficcient = 1254.77; //Value extracted from multiple measurements using DMM from the formula (Resolution/System voltage) = (ADC Reading/ Analog Voltage mesured)
int         light_sens_ch1_ready       = 0      ; //
int         light_sens_ch2_ready       = 0      ; //
double      adc_ch1_voltage            = 0.0    ; //
double      adc_ch2_voltage            = 0.0    ; //
#pragma endregion LightSensor

#pragma region  TemperatureSensor
int        temp_rdy_flag               = 0      ; // Indicates when ther is a temperature value ready to be read
double     curr_temp                   = 0.0    ; //Stores the last read temperature, mainly used for debugging
double     tcDelta                     = 0.0    ; //The differance between register A and B in the TC0
#pragma endregion TemperatureSensor

#pragma region  keypad
unsigned short int rowsArr[] = {ROW0, ROW1, ROW2, ROW3};
unsigned short int colsArr[] = {COL2, COL1, COL0};
#pragma endregion keypad

#pragma region LoginSystem
char login_text[]               =   "Enter Username and Password";
const int  login_text_length    =   27;
char login_user_str[]           =   "Username:";
char login_pass_str[]           =   "Password:";
const int  usr_pass_str_length  =   9;

screen_element* screen_username;
screen_element* screen_password;

int number_of_users             =   1;
char default_username[4]          = "1234";
char default_password[4]          = "1234";
int def_us_len                  = 4;
int def_pass_len                = 4;
user *default_user;
user_list* users;
user* current_user = NULL;

char edit_user_text[]           =   "Enter new Username and Password";
int edit_user_text_length       =   31;
screen_element* pre_password;
screen_element* pre_username;
screen_element* pre_login;

char* username;
char* password;

#pragma endregion LoginSystem