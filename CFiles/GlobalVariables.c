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
int menu_type                   = 0; //When in main menu:0, temp day stat menu: 1,  graphics mode: -1, 
int fast_mode_flag              = 0;
int startup_flag                = 3;
int set_servo_flag              = 0;
int current_graph_type          = -1;
#pragma endregion GlobalFlags

#pragma region Servo 
double      servo_position             = 0.0    ; //
int         prev_servo_call            = 0      ;

int         last_servo_update          =0;
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
double     curr_temp                   = 0.0    ; //Stores the last read temperature.
double     tcDelta                     = 0.0    ; //The differance between register A and B in the TC0
#pragma endregion TemperatureSensor

#pragma region  keypad
unsigned short int rowsArr[] = {ROW0, ROW1, ROW2, ROW3};
unsigned short int colsArr[] = {COL2, COL1, COL0};
int prev_keypad_key = 0;
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
long long hash_len_append = 0;

char edit_user_text[]           =   "Enter new Username and Password";
int edit_user_text_length       =   31;
screen_element* pre_password;
screen_element* pre_username;
screen_element* pre_login;

char* username;
char* password;

#pragma endregion LoginSystem

#pragma region DateTime
Date current_date = {00,00,00};
Time current_time = {00,00,00};
Time_hm current_time_hm = {00,00};
Time zero_time    = {00,00,00}; 
Date zero_date    = {00,00,00}; 
volatile unsigned long long microseconds = 0; 
const int month_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const int leap_feb = 29;
volatile int time_config_flag = 1;

screen_element screen_time;
screen_element screen_date;

char time_str[TIME_LEN];
char date_str[DATE_LEN];

#pragma endregion DateTime

#pragma region LinkedList
linked_node* head;
linked_node* tail;
int list_size = 0;
linked_node circular_start;

#pragma endregion LinkedList

#pragma region TempStatistics
day_temp_data* saved_day_temp_data;     //Container used to store statistics of the last 7 days
day_temp_data* test_day_temp_data;     //Container used to store statistics of the test mode days
int num_saved_day_data = 0;             //Number of days of recordings stored
int num_saved_day__test_data = 0;             //Number of days of recordings stored
int curr_day_data = 0;                  //The index value of the current day in the list containing temp stats
unsigned char* curr_min_temp_values;    //Stores all the temp values of the current min (so that they are added together in the list)
unsigned char previous_minute =0;       //Stores the previous minute which is added to the temp data at every new minute
unsigned char previous_hour =0;       //The hour stored at the same time as previous minute to safeguard any possible mixup

double var_sum1 = 0.0;                  //Regular sum of all temp values of the current day
double var_sum2 = 0.0;                  //Squared sum of all temp values of the current day

int num_of_measurements = 0;            //The number of measurements recorded this day
int new_day_flag = 0;
int measure_temp_flag = 0;              //Is set after a fixed period determined (60/measures_per_min seconds)
int last_temp_measure = 0;              //The second of the current minute that the temp was last recorded at (prevents it from saving multiple times per sec)
int new_minute_flag = 0;

int measures_per_min = 1;               //The number of measurements recorded every minute
int temp_minute_count = 0;              //A counter that keeps track of how many measurements has been taken this minute
double temp_minute_avg =0.0;            //The avrage over one minute 

int maximum_list_size = 0;              //The largest allowed list

#pragma endregion TempStatistics

#pragma region Menu
int mem_full_flag = 0;
#pragma endregion Menu

