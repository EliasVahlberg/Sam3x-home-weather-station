/**
* @file
*     display.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     Display Init and functions used to interface with the display
*/

#pragma region functions
void                    display_setup           (                                                                                             );
void                    Init_Display            (                                                                                             );
unsigned char           Read_Status_Display     (                                                                                             );
void                    Write_Data_2_Display    (unsigned char Data                                                                           );
void                    Write_Command_2_Display (unsigned char command                                                                        );
int                     set_cursor              (int pos,                           int screen_half_select                                    );
int                     write_blank_to_display  (int length                                                                                   );
int                     display_clear           (int length,                        int pos,                int screen_half_select            );
void                    clear                   (                                                                                             );
struct screen_cordinate convert_to_scord        (int x,                             int y                                                     );
struct screen_element   create_screen_element   (int x,                             int y,                  int l,                  char* text);
int                     display_write           (struct screen_element screen_el                                                              );
int                     write_string_to_display (char* message,                     int length                                                );
int                     display_write_direct    (int x,                             int y,                  int l,                  char* text);
int                     clear_display_direct    (int x,                             int y,                  int l                             );

#pragma endregion functions

/**
* display_setup
* \brief Starts the PMC for PIOC/D, enables all pins for the display and sets 
*  corresponding input and output
*/
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

/**
* Init_Display 
* \brief Resets the display, 
*  sets the text home adress,
*  Sets the graphics home adress,
*  Sets the text area/Graphics,
*  Enables text mode and turns off graphics mode
*/
void Init_Display()
{
    *AT91C_PIOC_SODR    =   OE;    //Disable output (74chip)
    *AT91C_PIOD_CODR    =   RESET_DISP; // Clear Reset display
    general_delay(DISPLAY_DELAY_VALUE);                 //Make a Delay
    *AT91C_PIOD_SODR    =   RESET_DISP; //Set Reset display
    Write_Data_2_Display(0x00);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x40);//Set text home address
    Write_Data_2_Display(DISPLAY_GRAPH_OFFSET_L);
    Write_Data_2_Display(DISPLAY_GRAPH_OFFSET_H);
    Write_Command_2_Display(0x42); //Set graphic home address
    Write_Data_2_Display(0x1e);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x41); // Set text area
    Write_Data_2_Display(0x1e);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0x43); // Set graphic area
    Write_Command_2_Display(0x80); // text mode
    Write_Command_2_Display(DISPLAY_SET_TEXT_MODE); // Text on graphic off 
    *AT91C_PIOC_CODR    =   OE;    //Disable output (74chip)
}

/**
* Read_Status_Display 
* \brief Reads the status from the display via the databuss 
*/
unsigned char Read_Status_Display()
{
   unsigned char temp;

   *AT91C_PIOC_ODR     =   BUS_DISP;// make databus as input
   *AT91C_PIOC_SODR     =   DIR_DISP;// Set dir as input (74chip, 1 = input)
   *AT91C_PIOC_CODR    =   OE_DISP;// Clear/enable output (74chip 0 = enable)

   *AT91C_PIOC_SODR    =   CD_DISP;    // Set C/D
   *AT91C_PIOC_CODR    =   CE_DISP;    //Clear chip select display
   *AT91C_PIOC_CODR    =   RD_DISP;// Clear read display
   general_delay(DISPLAY_DELAY_VALUE);                      // Make a Delay
   temp = *AT91C_PIOC_PDSR & BUS_DISP; // Read data bus and save it in temp
   *AT91C_PIOC_SODR    =   CE_DISP;// Set chip select display
   *AT91C_PIOC_SODR    =   RD_DISP;// Set read display
   *AT91C_PIOC_SODR    =   OE_DISP;// Disable output (74chip)
   *AT91C_PIOC_CODR    =  DIR_DISP;// Set dir as output (74chip)
   return (temp) ;

}

/**
* Write_Data_2_Display 
* \brief Writes data to the display via the databuss (usually followed by a command)
*/
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
    general_delay(DISPLAY_DELAY_VALUE);   //Make a Delay
    *AT91C_PIOC_SODR    =   CE_DISP;    //Set chip enable display
    *AT91C_PIOC_SODR    =   WR_DISP;    //Set write display
    *AT91C_PIOC_SODR    =   OE_DISP;    //Disable output (74chip)

    *AT91C_PIOC_ODR     =   BUS_DISP;   //Make databus as input 

}

/**
* Write_Command_2_Display 
* \brief Writes command to the display via the databuss (usually after one or two data writes)
*/
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
       general_delay(DISPLAY_DELAY_VALUE);   //Make a Delay
    *AT91C_PIOC_SODR    =   CE_DISP;    //Set chip enable display
    *AT91C_PIOC_SODR    =   WR_DISP;    //Set write display
    *AT91C_PIOC_SODR    =   OE_DISP;    //Disable output (74chip)

    *AT91C_PIOC_ODR     =   BUS_DISP;   //Make databus as input 


}

/**
* set_cursor 
* \brief Sets the cursor pointer to a specific adress given the the pos and the screen half
* @param pos between 0 and 240 repressenting the lower half of the screen
* @param screen_half_select determines what screen half the cursor is set to
* @return 1 if it was sucsessfull, 0 if the cordinate was out of bounds 
*/
int set_cursor(int pos, int screen_half_select)
{
    if(screen_half_select>1)
        return 0;
    if(pos> ((int)DISP_Y_MAX*DISP_X_MAX/2))
        return 0;
    Write_Data_2_Display(pos);

    Write_Data_2_Display(screen_half_select);
    
    Write_Command_2_Display(DISPLAY_SET_CURSOR_CMD);
    return 1;
}

/**
* write_blank_to_display 
* \brief erases the values from the cursor possition to the cursor possition + length
* @param length how many spaces to erase
* @return 1 if it was sucsessfull, 0 if nothing was deleted, -3 if write acess is blocked 
*/
int write_blank_to_display(int length)
{
    if(display_write_disable_flag==0)
    {
        if(length == 0)
        return 0;
        for (int i = 0; i < length; i++)
        {
            Write_Data_2_Display(0);
            Write_Command_2_Display(DISPLAY_WRITE_AND_INCREMENT_CURSOR_CMD);
        }
        general_delay(100); //previously 1000
        return 1;
    }
    else
        return -3;
}

/**
* display_clear
* \brief erases the values from given screen possition 
* @param length how many spaces to erase
* @return 1 if it was sucsessfull, 0 if nothing was deleted or if it was out of bounds, -3 if write acess is blocked 
*/
int display_clear(int length, int pos, int screen_half_select)
{
    if(display_write_disable_flag==0)
    {
        int cursor = set_cursor(pos,screen_half_select);
        int write = write_blank_to_display(length);
        return 1;
    }
    else
        return -3;
}

/**
* clear
* \brief clears the whole screen if write access is enabeled 
*/
void clear()
{
    if(display_write_disable_flag==0)
    {
        Write_Data_2_Display(0x00);
        Write_Data_2_Display(0x00);
        Write_Command_2_Display(DISPLAY_SET_CURSOR_CMD);
        for (int i = 0; i < (DISP_X_MAX*DISP_Y_MAX); i++)
        {
            Write_Data_2_Display(0x00);
            Write_Command_2_Display(DISPLAY_WRITE_AND_INCREMENT_CURSOR_CMD);
            general_delay(100);
        }
        general_delay(100);
        
    }
}

/**
* convert_to_scord 
* \brief Converts from conventional conrdinates to the format Low_adress:0-240, High_adress:0-1
* @param x cordinate
* @param y cordinate
* @return struct screen_cordinate containing the converted cordinate
*/
struct screen_cordinate convert_to_scord(int x, int y)
{
    struct screen_cordinate sc;
    if(x + y*30 >DISPLAY_SCREEN_HALF*2)
        return sc; 
    sc.screen_half_val =(x+y*30>DISPLAY_SCREEN_HALF)? 1:0;
    sc.pos = (sc.screen_half_val==1)? (x+y*30-DISPLAY_SCREEN_HALF-1):x+y*30;
    return sc;

}

/**
* create_screen_element 
* \brief Constructs an struct screen_element from x/y pos length and a string to be displayed
* @param x cordinate
* @param y cordinate
* @param l length of the string
* @param text string to be displayed
* @return struct screen_element containing the input data
*/
struct screen_element create_screen_element(int x, int y, int l, char* text)
{
    struct screen_element screen_el;
    struct screen_cordinate sc =  convert_to_scord(x,y);
    screen_el.screen_cord = sc;
    screen_el.l = l;
    screen_el.text = text;    
    return screen_el;

}

/**
* display_write 
* \brief Shortens the proscess of writing to the display calling all needed functions and supplying 
*  the data given in the screen element.
* @param screen_el a struct screen_element containing all the data needed to write to the screen
* @return 1 if successful, -1 if cursor out of bounds, -2 if write faliure, -3 if write acess is disabled 
*/
int display_write(struct screen_element screen_el)
{
    if(display_write_disable_flag==0)
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
        Write_Command_2_Display(DISPLAY_SET_CURSOR_CMD);
        if(cursor == 0)
            return -1;
        if(write == 0)
            return -2;
        else
            return 1;
    }
    else
        return -3;
} 

/**
* convert_to_scord 
* \brief writes the string contained in message, converts from regular char 
*  to the display symbol on the fly
* @param message string containing the message
* @param length the length of message
* @return 1 if successful, 0 if nothing was written, -3 if write acess is disabled 
* @return 1 if successful, 0 if nothing was written, -3 if write acess is disabled 
*/
int write_string_to_display(char* message,int length)
{   
    if(display_write_disable_flag==0)
    {
        if(length == 0)
            return 0;
        for (int i = 0; i < length; i++)
        {
            unsigned char symbol = (unsigned char)(((int) message[i])-32);
            if(((int)symbol)>94 || ((int)symbol)<0)
                return 0;
            Write_Data_2_Display(symbol);
            Write_Command_2_Display(DISPLAY_WRITE_AND_INCREMENT_CURSOR_CMD);
        }
        general_delay(1000);
        return 1;
    }
    else
        return -3;
}

int display_write_direct(int x, int y, int l, char* text)
{
    screen_element sc_el = create_screen_element(x,y,l,text);
    int i = display_write(sc_el);
    return i;
}

int clear_display_direct (int x, int y, int l )
{
    screen_cord sc = convert_to_scord(x,y);
    display_clear(l,sc.pos,sc.screen_half_val);
}
