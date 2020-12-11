/**
* @file
*     display.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/

#pragma region
#define RESET_DISP (1    ) //DP 25 PIOD

#define B4_DISP    (1<<6 ) //DP 38 

#define OE (1<<2)

#define RV_DISP    (1<<19) //DP 44 PIOC
#define FS_DISP    (1<<18) //DP 45 PIOC
#define WR_DISP    (1<<17) //DP 46 PIOC
#define RD_DISP    (1<<16) //DP 47 PIOC
#define CE_DISP    (1<<15) //DP 48 PIOC
#define CD_DISP    (1<<14) //DP 49 PIOC
#define DIR_DISP   (1<<13) //DP 50 PIOC
#define OE_DISP    (1<<12) //DP 51 PIOC
#define BUS_DISP   (0x3FC) //DP 34-41
#define CONFIG_DISP (0xff000) //DP 44-51

#define DISP_X_MAX (30)

#define DISP_Y_MAX (16)


int delay_value = 100;
int clearKey = 12;
int p0=0,p1=0,p2=0;
int prevKey = 0;
int currentKey = 0;
int keyset = 0;
int delayValue = 10000;
int display_keymap_xy[13]={0,17,18,19,20,21,22,23,24,25,10,16,3};


int screen_half = 240;


void display_setup(void);
void Init_Display(void);
unsigned char Read_Status_Display(void);
void Write_Data_2_Display(unsigned char Data);
void Write_Command_2_Display(unsigned char command);

int set_cursor(int pos, int screen_half_select);
int write_blank_to_display(int length);
int display_clear(int length, int pos, int screen_half_select);
void clear();

struct screen_cordinate convert_to_scord(int x, int y);
struct screen_element create_screen_element(int x, int y, int l, char* text);
int display_write(struct screen_element screen_el);
int write_string_to_display(char* message,int length);
#pragma endregion


void display_setup()
{
    *AT91C_PMC_PCER = (1 << 13);     //Enable PMC, starting peripheral clock
    *AT91C_PMC_PCER = (1 << 14);     //Enable PMC, starting peripheral clock

    *AT91C_PIOD_PER     |= OE;        //PIN27 OE
    *AT91C_PIOD_OER     |= OE;        //PIN27 OE


    *AT91C_PIOC_PER     |= BUS_DISP;  //Enable pin34-41
    *AT91C_PIOC_PPUDR   |= BUS_DISP;
    *AT91C_PIOC_OER     |= BUS_DISP;
    
    
    *AT91C_PIOC_PER     |= DIR_DISP;  //Enable pin50
    *AT91C_PIOC_PPUDR   |= DIR_DISP;
    *AT91C_PIOC_OER     |= DIR_DISP;
    
    *AT91C_PIOC_PER     |= OE_DISP;  //Enable pin51
    *AT91C_PIOC_PPUDR   |= OE_DISP;
    *AT91C_PIOC_OER     |= OE_DISP;
    
    *AT91C_PIOD_PER     |= RESET_DISP;  //Enable pin25
    *AT91C_PIOD_PPUDR   |= RESET_DISP;
    *AT91C_PIOD_OER     |= RESET_DISP;

    *AT91C_PIOC_PER     |= CONFIG_DISP;  //Enable pin25
    *AT91C_PIOC_PPUDR   |= CONFIG_DISP;
    *AT91C_PIOC_OER     |= CONFIG_DISP;

}

void Init_Display(void)
{
    *AT91C_PMC_PCER    |= (1 << 13);     //Enable PMC, starting peripheral clock PIOC  DISP
    *AT91C_PMC_PCER    |= (1 << 14);     //Enable PMC, starting peripheral clock PIOD  DISP

    *AT91C_PIOC_SODR    =   OE;    //Disable output (74chip)
    *AT91C_PIOD_CODR    =   RESET_DISP; // Clear Reset display
    general_delay(delay_value);                 //Make a Delay
    *AT91C_PIOD_SODR    =   RESET_DISP; //Set Reset display
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x40);//Set text home address
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x40);
    Write_Command_2_Display(0x42); //Set graphic home address
    Write_Data_2_Display(0x1e);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x41); // Set text area
    Write_Data_2_Display(0x1e);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x43); // Set graphic area
    Write_Command_2_Display(0x80); // text mode
    Write_Command_2_Display(0x94); // Text on graphic off 
    *AT91C_PIOC_CODR    =   OE;    //Disable output (74chip)
}

unsigned char Read_Status_Display(void)
{
   unsigned char temp;

   *AT91C_PIOC_ODR     =   BUS_DISP;// make databus as input
   *AT91C_PIOC_SODR     =   DIR_DISP;// Set dir as input (74chip, 1 = input)
   *AT91C_PIOC_CODR    =   OE_DISP;// Clear/enable output (74chip 0 = enable)

   *AT91C_PIOC_SODR    =   CD_DISP;    // Set C/D
   *AT91C_PIOC_CODR    =   CE_DISP;    //Clear chip select display
   *AT91C_PIOC_CODR    =   RD_DISP;// Clear read display
   general_delay(delay_value);                      // Make a Delay
   temp = *AT91C_PIOC_PDSR & BUS_DISP; // Read data bus and save it in temp
   *AT91C_PIOC_SODR    =   CE_DISP;// Set chip select display
   *AT91C_PIOC_SODR    =   RD_DISP;// Set read display
   *AT91C_PIOC_SODR    =   OE_DISP;// Disable output (74chip)
   *AT91C_PIOC_CODR    =  DIR_DISP;// Set dir as output (74chip)
   return (temp) ;

}

void Write_Data_2_Display(unsigned char data)
{

    Read_Status_Display();              //Wait until Read_Status_Display returns an OK
    *AT91C_PIOC_OER     =   BUS_DISP;   //COPPY databus as output
    *AT91C_PIOC_CODR    =   BUS_DISP;   //Clear databus
    unsigned int dataInt=   (unsigned int) (data<<2);
    *AT91C_PIOC_SODR    =   dataInt;    //Set Data to databus
    *AT91C_PIOC_CODR     =   DIR_DISP;   //Set dir as output (74chip)
    *AT91C_PIOC_CODR    =   OE_DISP;    //Enable output (74chip)

    *AT91C_PIOC_OER     =   BUS_DISP;   //Set databus as output
    *AT91C_PIOC_CODR    =   CD_DISP;    //Clear C/D signal High (0 = Data)
    *AT91C_PIOC_CODR    =   CE_DISP;    //Clear chip select display
    *AT91C_PIOC_CODR    =   WR_DISP;    //Clear write display
    general_delay(delay_value);   //Make a Delay
    *AT91C_PIOC_SODR    =   CE_DISP;    //Set chip enable display
    *AT91C_PIOC_SODR    =   WR_DISP;    //Set write display
    *AT91C_PIOC_SODR    =   OE_DISP;    //Disable output (74chip)

    *AT91C_PIOC_ODR     =   BUS_DISP;   //Make databus as input 

}

void Write_Command_2_Display(unsigned char command)
{

    Read_Status_Display();//Wait until Read_Status_Display returns an OK Status_Display returns an OK
    *AT91C_PIOC_OER     =   BUS_DISP;   //COPPY databus as output
    *AT91C_PIOC_CODR    =   BUS_DISP;   //Clear databus
    unsigned int commandInt=   (unsigned int) (command<<2);
    *AT91C_PIOC_SODR    =   commandInt;    //Set Data to databus
    *AT91C_PIOC_CODR     =   DIR_DISP;   //Set dir as output (74chip)
    *AT91C_PIOC_CODR    =   OE_DISP;    //Enable output (74chip)


    *AT91C_PIOC_OER     =   BUS_DISP;   //Set databus as output
    *AT91C_PIOC_SODR    =   CD_DISP;    //set C/D signal High (1 = Command)
    *AT91C_PIOC_CODR    =   CE_DISP;    //Clear chip select display
    *AT91C_PIOC_CODR    =   WR_DISP;    //Clear write display
       general_delay(delay_value);   //Make a Delay
    *AT91C_PIOC_SODR    =   CE_DISP;    //Set chip enable display
    *AT91C_PIOC_SODR    =   WR_DISP;    //Set write display
    *AT91C_PIOC_SODR    =   OE_DISP;    //Disable output (74chip)

    *AT91C_PIOC_ODR     =   BUS_DISP;   //Make databus as input 


}

int set_cursor(int pos, int screen_half_select)
{
    if(screen_half_select>1)
        return 0;
    if(pos> ((int)DISP_Y_MAX*DISP_X_MAX/2))
        return 0;
    Write_Data_2_Display(pos);

    Write_Data_2_Display(screen_half_select);
    
    Write_Command_2_Display(0x24);
    return 1;
}

int write_blank_to_display(int length)
{
        if(length == 0)
        return 0;
        for (int i = 0; i < length; i++)
        {

        Write_Data_2_Display(0);
        Write_Command_2_Display(0xc0);
        }
        
    
    general_delay(1000);
    return 1;
}

int display_clear(int length, int pos, int screen_half_select)
{
    int cursor = set_cursor(pos,screen_half_select);
    int write = write_blank_to_display(length);
}

void clear()
{
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x24);
    for (int i = 0; i < (DISP_X_MAX*DISP_Y_MAX); i++)
    {
        Write_Data_2_Display(0x00);
        Write_Command_2_Display(0xc0);
        general_delay(100);
    }
        general_delay(100);
    
}

struct screen_cordinate convert_to_scord(int x, int y)
{
    struct screen_cordinate sc;
    if(x + y*30 >screen_half*2)
        return sc;

    sc.screen_half_val =(x+y*30>screen_half)? 1:0;
    sc.pos = (sc.screen_half_val==1)? (x+y*30-screen_half):x+y*30;
    return sc;

}

struct screen_element create_screen_element(int x, int y, int l, char* text)
{
    struct screen_element screen_el;
    struct screen_cordinate sc =  convert_to_scord(x,y);
    screen_el.screen_cord = sc;
    screen_el.l = l;
    screen_el.text = text;    
    return screen_el;

}

int display_write(struct screen_element screen_el)
{

     display_clear(
     screen_el.l,
     screen_el.screen_cord.pos,
     screen_el.screen_cord.screen_half_val
     );
    int cursor = set_cursor(
    screen_el.screen_cord.pos,
    screen_el.screen_cord.screen_half_val
    );
    
    int write = write_string_to_display(screen_el.text,screen_el.l);

    Write_Data_2_Display(0x0);
    Write_Command_2_Display(0x24);
    if(cursor == 0)
        return -1;
    if(write == 0)
        return -2;
    else
        return 1;
} 

int write_string_to_display(char* message,int length)
{   
    if(length == 0)
        return 0;
        for (int i = 0; i < length; i++)
        {
        unsigned char symbol = (unsigned char)(((int) message[i])-32);
        if(((int)symbol)>94 || ((int)symbol)<0)
            return 0;
        
        Write_Data_2_Display(symbol);
        Write_Command_2_Display(0xc0);
        }
        
    
    general_delay(1000);
    return 1;
}