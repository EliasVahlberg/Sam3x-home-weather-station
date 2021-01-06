/**
* @file
*     keypad.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     Keypad Init and functions used to get information from the keypad
*/

#pragma region Functions
int get_keypad_key();
void keypad_setup();
#pragma endregion Functions


/**
 * get_keypad_key
* \brief Retrives the key that is pressed by setting the cols and clearing them one by one,
*  if multiple keys are pressed the highest one is saved  
* @return int value representing the possition on the keypad e.g. ["1"=1...,"9"=9,"*"=10,"0"=11,"#"=12]
*  0 representing no key press
*/
int get_keypad_key()
{
    int val = 0;
    *AT91C_PIOD_CODR = OE;

    for (int i = 0; i < COLS; i++)
    {
        *AT91C_PIOC_OER     = colsArr[i];
        *AT91C_PIOC_SODR    = colsArr[i];
    }

    for (int i = 0; i < COLS; i++)
    {
        *AT91C_PIOC_CODR = colsArr[i];
        for (int j = 0; j < ROWS; j++)
        {
            if(!(*AT91C_PIOC_PDSR & (rowsArr[j]) ))
                val = j*3+i+1;

        }
        *AT91C_PIOC_SODR = colsArr[i];

    }
    
    for (int i = 0; i < COLS; i++)
    for (int i = 0; i < COLS; i++)
    {
        *AT91C_PIOC_CODR    = colsArr[i];
        *AT91C_PIOC_IER     = colsArr[i];
    }
    *AT91C_PIOD_SODR = OE;
    return val;


}

int get_user_input()
{
    int key = get_keypad_key();
    if(prev_keypad_key==0&&key==0) //Delayed recheck
    {
        delay_micro(100);
        key = get_keypad_key();
    }
    if(prev_keypad_key==0||key==0)
    {
        prev_keypad_key = key;
        return key;
    }
    else
        return 0;
}
/**
* keypad_setup
* \brief Init for the keypad,
* enables the pins 27, 34-37 and 39-41.
* Dissables the pullup resistors
*/
void keypad_setup()
{
    
    *AT91C_PIOD_PER     |= OE;        //PIN27 OE
    *AT91C_PIOD_OER     |= OE;        //PIN27 OE
    
    *AT91C_PIOC_PER     |= COL0;      //Enable pin39
    *AT91C_PIOC_PPUDR   |= COL0;
    
    *AT91C_PIOC_PER     |= COL1;      //Enable pin40
    *AT91C_PIOC_PPUDR   |= COL1;
    
    *AT91C_PIOC_PER     |= COL2;      //Enable pin41
    *AT91C_PIOC_PPUDR   |= COL2;
    
    *AT91C_PIOC_PER     |= ROW0;      //Enable pin34
    *AT91C_PIOC_PPUDR   |= ROW0;
    
    *AT91C_PIOC_PER     |= ROW1;      //Enable pin35
    *AT91C_PIOC_PPUDR   |= ROW1;
    
    *AT91C_PIOC_PER     |= ROW2;      //Enable pin36
    *AT91C_PIOC_PPUDR   |= ROW2;
    
    *AT91C_PIOC_PER     |= ROW3;      //Enable pin37
    *AT91C_PIOC_PPUDR   |= ROW3;
    
}