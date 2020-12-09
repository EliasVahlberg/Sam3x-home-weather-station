/**
* @file
*     keypad.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
//#include "ArduinoBaseInclude.c"

#pragma region
#define ROWS 4
#define COLS 3
 
#define OE (1<<2)
 
#define COL0 (1<<7)
#define COL1 (1<<8)
#define COL2 (1<<9)
 
#define ROW0 (1<<2)
#define ROW1 (1<<3)
#define ROW2 (1<<4)
#define ROW3 (1<<5)   

unsigned short int rowsArr[] = {ROW0, ROW1, ROW2, ROW3};
unsigned short int colsArr[] = {COL2, COL1, COL0};
#pragma endregion


int func(void)
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