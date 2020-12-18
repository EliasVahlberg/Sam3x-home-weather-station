/**
* @file
*     MD5.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/


//Illustration purposes:  #define PLUSTIMES(X, Y)     (X+Y*X)

//128 byte buffer length
#define MD5_BUFFER (1024)

struct MD5_Context
{
    struct
    {
        unsigned int A, B, C, D;
    } md5_data;
    unsigned char *buffer;
    unsigned int size;
    unsigned int bits;
};

#pragma region MacroFunctions
//MD5 Core functions
#define MD5_F(X, Y, Z) ((X & Y) | ((~X) & Z))
#define MD5_G(X, Y, Z) ((X & Y) | ((~Z) & Y))
#define MD5_H(X, Y, Z) (X ^ Y ^ Z)
#define MD5_I(X, Y, Z) (Y ^ (X | (Z)))

//Unsigned rotate left z
#define MD5_ROTATE_LEFT(X, Y) (((X) << (Y)) | ((X) >> (32 - Y)))

//MD5 Composit functions used in round 1-4 based on : "The MD5 Message-Digest Algorithm,R. Rivest , MIT LMIT Laboratory for Computer Science and RSA Data Security, Inc., "[1]
#define MD5_ROUND1(A, B, C, D, X, S, T)                             \
    {                                                               \
        (A) += MD5_F((B), (C), (D)) + (X) + (unsigned long int)(T); \
        (A) = MD5_ROTATE_LEFT((A), (S));                            \
        (A) += (B);                                                 \
    }

#define MD5_ROUND2(A, B, C, D, X, S, T)                             \
    {                                                               \
        (A) += MD5_G((B), (C), (D)) + (X) + (unsigned long int)(T); \
        (A) = MD5_ROTATE_LEFT((A), (S));                            \
        (A) += (B);                                                 \
    }

#define MD5_ROUND3(A, B, C, D, X, S, T)                             \
    {                                                               \
        (A) += MD5_H((B), (C), (D)) + (X) + (unsigned long int)(T); \
        (A) = MD5_ROTATE_LEFT((A), (S));                            \
        (A) += (B);                                                 \
    }

#define MD5_ROUND4(A, B, C, D, X, S, T)                             \
    {                                                               \
        (A) += MD5_I((B), (C), (D)) + (X) + (unsigned long int)(T); \
        (A) = MD5_ROTATE_LEFT((A), (S));                            \
        (A) += (B);                                                 \
    }
//Could be used instead of shortInt_to_longInt_array_conversion
#define MD5_ARRAY_ELEM_COPY(a, b, i)                                                                                                                  \
    {                                                                                                                                                 \
        (a) = ((unsigned int)(b)[(i)]) | ((unsigned int)(b)[(i) + 1] << 8) | ((unsigned int)(b)[(i) + 2] << 16) | ((unsigned int)(b)[(i) + 3] << 24); \
    }
#define MD5_TO_LONG_INT(x, y1, y2, y3, y4)                                                                        \
    {                                                                                                             \
        (x) = ((unsigned int)y1) | ((unsigned int)y2 << 8) | ((unsigned int)y3 << 16) | ((unsigned int)y4 << 24); \
    }
#pragma endregion MacroFunctions

#pragma region StaticVars
//given in the source [1]
const unsigned short int md5_K[64] = {
    0 /* 1  (FF)*/, 1 /* 2  (FF)*/, 2 /* 3  (FF)*/, 3 /* 4  (FF)*/, 4 /* 5  (FF)*/, 5 /* 6  (FF)*/, 6 /* 7  (FF)*/, 7 /* 8  (FF)*/, 8 /* 9  (FF)*/, 9 /* 10 (FF)*/, 10 /* 11 (FF)*/, 11 /* 12 (FF)*/, 12 /* 13 (FF)*/, 13 /* 14 (FF)*/, 14 /* 15 (FF)*/, 15 /* 16 (FF)*/,
    1 /* 17 (GG)*/, 6 /* 18 (GG)*/, 11 /* 19 (GG)*/, 0 /* 20 (GG)*/, 5 /* 21 (GG)*/, 10 /* 22 (GG)*/, 15 /* 23 (GG)*/, 4 /* 24 (GG)*/, 9 /* 25 (GG)*/, 14 /* 26 (GG)*/, 3 /* 27 (GG)*/, 8 /* 28 (GG)*/, 13 /* 29 (GG)*/, 2 /* 30 (GG)*/, 7 /* 31 (GG)*/, 12 /* 32 (GG)*/,
    5 /* 33 (HH)*/, 8 /* 34 (HH)*/, 11 /* 35 (HH)*/, 14 /* 36 (HH)*/, 1 /* 37 (HH)*/, 4 /* 38 (HH)*/, 7 /* 39 (HH)*/, 10 /* 40 (HH)*/, 13 /* 41 (HH)*/, 0 /* 42 (HH)*/, 3 /* 43 (HH)*/, 6 /* 44 (HH)*/, 9 /* 45 (HH)*/, 12 /* 46 (HH)*/, 15 /* 47 (HH)*/, 2 /* 48 (HH)*/,
    0 /* 49 (II)*/, 7 /* 50 (II)*/, 14 /* 51 (II)*/, 5 /* 52 (II)*/, 12 /* 53 (II)*/, 3 /* 54 (II)*/, 10 /* 55 (II)*/, 1 /* 56 (II)*/, 8 /* 57 (II)*/, 15 /* 58 (II)*/, 6 /* 59 (II)*/, 13 /* 60 (II)*/, 4 /* 61 (II)*/, 11 /* 62 (II)*/, 2 /* 63 (II)*/, 9 /* 64 (II)*/
};

// Discribed in the source [1] as "Constants for MD5Transform routine."
const unsigned short int md5_S[16] = {
    7 /*S Round1 1*/, 12 /*S Round1 2*/, 17 /*S Round1 3*/, 22 /*S Round1 4*/,
    5 /*S Round2 1*/, 9 /*S Round2 2*/, 14 /*S Round2 3*/, 20 /*S Round2 4*/,
    4 /*S Round3 1*/, 11 /*S Round3 2*/, 16 /*S Round3 3*/, 23 /*S Round3 4*/,
    6 /*S Round4 1*/, 10 /*S Round4 2*/, 15 /*S Round4 3*/, 21 /*S Round4 4*/};
// Discribed in the source [1]
const unsigned short int md5_X_I[64] = {
    0 /* R1 1  */, 1 /* R1 2  */, 2 /* R1 3  */, 3 /* R1 4  */, 4 /* R1 5  */, 5 /* R1 6  */, 6 /* R1 7  */, 7 /* R1 8  */, 8 /* R1 9  */, 9 /* R1 10 */, 10 /* R1 11 */, 11 /* R1 12 */, 12 /* R1 13 */, 13 /* R1 14 */, 14 /* R1 15 */, 15 /* R1 16 */,
    1 /* R2 1  */, 6 /* R2 2  */, 11 /* R2 3  */, 0 /* R2 4  */, 5 /* R2 5  */, 10 /* R2 6  */, 15 /* R2 7  */, 4 /* R2 8  */, 9 /* R2 9  */, 14 /* R2 10 */, 3 /* R2 11 */, 8 /* R2 12 */, 13 /* R2 13 */, 2 /* R2 14 */, 7 /* R2 15 */, 12 /* R2 16 */,
    5 /* R3 1  */, 8 /* R3 2  */, 11 /* R3 3  */, 14 /* R3 4  */, 1 /* R3 5  */, 4 /* R3 6  */, 7 /* R3 7  */, 10 /* R3 8  */, 13 /* R3 9  */, 0 /* R3 10 */, 3 /* R3 11 */, 6 /* R3 12 */, 9 /* R3 13 */, 12 /* R3 14 */, 15 /* R3 15 */, 2 /* R3 16 */,
    0 /* R4 1  */, 7 /* R4 2  */, 14 /* R4 3  */, 5 /* R4 4  */, 12 /* R4 5  */, 3 /* R4 6  */, 10 /* R4 7  */, 1 /* R4 8  */, 8 /* R4 9  */, 15 /* R4 10 */, 6 /* R4 11 */, 13 /* R4 12 */, 4 /* R4 13 */, 11 /* R4 14 */, 2 /* R4 15 */, 9 /* R4 16 */
};
//Precomputed table:
//Floor integer values from: 2^32*abs(sin(i rad)), 2^32 since we are working with 32bit values
const unsigned long int md5_T[64] = {
    0xd76aa478 /* 1  (FF)*/, 0xe8c7b756 /* 2  (FF)*/, 0x242070db /* 3  (FF)*/, 0xc1bdceee /* 4  (FF)*/, 0xf57c0faf /* 5  (FF)*/, 0x4787c62a /* 6  (FF)*/, 0xa8304613 /* 7  (FF)*/, 0xfd469501 /* 8  (FF)*/, 0x698098d8 /* 9  (FF)*/, 0x8b44f7af /* 10 (FF)*/, 0xffff5bb1 /* 11 (FF)*/, 0x895cd7be /* 12 (FF)*/, 0x6b901122 /* 13 (FF)*/, 0xfd987193 /* 14 (FF)*/, 0xa679438e /* 15 (FF)*/, 0x49b40821 /* 16 (FF)*/,
    0xf61e2562 /* 17 (GG)*/, 0xc040b340 /* 18 (GG)*/, 0x265e5a51 /* 19 (GG)*/, 0xe9b6c7aa /* 20 (GG)*/, 0xd62f105d /* 21 (GG)*/, 0x2441453 /* 22 (GG)*/, 0xd8a1e681 /* 23 (GG)*/, 0xe7d3fbc8 /* 24 (GG)*/, 0x21e1cde6 /* 25 (GG)*/, 0xc33707d6 /* 26 (GG)*/, 0xf4d50d87 /* 27 (GG)*/, 0x455a14ed /* 28 (GG)*/, 0xa9e3e905 /* 29 (GG)*/, 0xfcefa3f8 /* 30 (GG)*/, 0x676f02d9 /* 31 (GG)*/, 0x8d2a4c8a /* 32 (GG)*/,
    0xfffa3942 /* 33 (HH)*/, 0x8771f681 /* 34 (HH)*/, 0x6d9d6122 /* 35 (HH)*/, 0xfde5380c /* 36 (HH)*/, 0xa4beea44 /* 37 (HH)*/, 0x4bdecfa9 /* 38 (HH)*/, 0xf6bb4b60 /* 39 (HH)*/, 0xbebfbc70 /* 40 (HH)*/, 0x289b7ec6 /* 41 (HH)*/, 0xeaa127fa /* 42 (HH)*/, 0xd4ef3085 /* 43 (HH)*/, 0x4881d05 /* 44 (HH)*/, 0xd9d4d039 /* 45 (HH)*/, 0xe6db99e5 /* 46 (HH)*/, 0x1fa27cf8 /* 47 (HH)*/, 0xc4ac5665 /* 48 (HH)*/,
    0xf4292244 /* 49 (II)*/, 0x432aff97 /* 50 (II)*/, 0xab9423a7 /* 51 (II)*/, 0xfc93a039 /* 52 (II)*/, 0x655b59c3 /* 53 (II)*/, 0x8f0ccc92 /* 54 (II)*/, 0xffeff47d /* 55 (II)*/, 0x85845dd1 /* 56 (II)*/, 0x6fa87e4f /* 57 (II)*/, 0xfe2ce6e0 /* 58 (II)*/, 0xa3014314 /* 59 (II)*/, 0x4e0811a1 /* 60 (II)*/, 0xf7537e82 /* 61 (II)*/, 0xbd3af235 /* 62 (II)*/, 0x2ad7d2bb /* 63 (II)*/, 0xeb86d391 /* 64 (II)*/};

//maximum padding 64 bytes / 512 Bits 
static unsigned char md5_padding [64] = { 
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
#pragma endregion StaticVars

#pragma region Functions
       void MD5_HASH                               (unsigned char* message,        int length,                         unsigned char* output       );
       void MD5_init                               (struct MD5_Context* contxt                                                                     );
static void md5_block_transform_state              (struct MD5_Context* contxt                                                                     );
static void char_to_longInt_array_conversion       (unsigned long int* longarray,  unsigned char* chararray,           int length                  );
static void md5_encode                             (unsigned char * buffer,        struct MD5_Context* contxt                                      );
       void md5_final_encode                       (unsigned char* output,         struct MD5_Context* contxt                                      );
static void md5_copy_arr                           (unsigned char *dest_arr,       unsigned char *src_arr,             unsigned int length         );
static void md5_erase_arr                          (unsigned char *src_arr,        unsigned char erase_vale,           unsigned int length         );
static void md5_append_size                        (unsigned char* buffer,         unsigned int,                       unsigned int bits           );
#pragma endregion Functions

#pragma region GlobalVars
unsigned long A, AA;
unsigned long B, BB;
unsigned long C, CC;
unsigned long D, DD;
#pragma endregion GlobalVars


void MD5_HASH(unsigned char* message, int length, unsigned char* output)
{
    struct MD5_Context* contxt;
    contxt              =  (malloc(sizeof(struct MD5_Context)));
    if (contxt == NULL)
    {
        //replace with HARDFAULT handler
        return;
    }
    MD5_init(contxt);
    while(1)
    {
        md5_copy_arr(contxt->buffer+contxt->size,message + contxt->bits,MD5_BUFFER - contxt->size);
        contxt->size += MD5_BUFFER - contxt->size;
        md5_block_transform_state(contxt);                                                                                 
        if(length - contxt->bits > 64)
            break;
    }
    md5_final_encode(output, contxt);
    free(contxt->buffer);
    free(contxt);
                                                                                                                //create md5_final and md5_addsize
}

void MD5_init(struct MD5_Context* contxt)
{

    contxt->buffer      = (unsigned char*) (malloc(MD5_BUFFER));
    if (contxt->buffer == NULL)
    {
        //replace with HARDFAULT handler
        return;
    }
    md5_erase_arr(contxt->buffer,'\0',MD5_BUFFER);
    contxt->size = 0;
    contxt->bits = 0;
    //A,B,C,D are set to a sufficiently arbitrary number
    //(A = 01 23 45 67 ,B = 89 ab cd ef, C = fe dc ba 98, D = 76 54 32 10)
    contxt->md5_data.A  = 0x67452301;
    contxt->md5_data.B  = 0xefcdab89;
    contxt->md5_data.C  = 0x98badcfe;
    contxt->md5_data.D  = 0x10325476;
    
}

static void md5_block_transform_state(struct MD5_Context* contxt)
{
    unsigned char buffer[64];
    int i;
    for (i = 0; contxt->size > i+63; i+=63)
    {
    md5_copy_arr(buffer,contxt->buffer+i,64);
    md5_encode(buffer,contxt);
    contxt->bits += 64;  
    }
         md5_copy_arr (buffer, contxt->buffer + i, contxt->size - i);
         md5_copy_arr (contxt->buffer, buffer, contxt->size - i);
         contxt->size -= i;
}

static void char_to_longInt_array_conversion( unsigned long int* longarray,unsigned char* chararray, int length)
{
    int i, j;
    for (i = 0, j = 0; j < length; i++, j += 4) //Maxad for loop
    {
        longarray[i] =
            ((unsigned long int)chararray[j]) |
            (((unsigned long int)chararray[j + 1]) << 8) |
            (((unsigned long int)chararray[j + 2]) << 16) |
            (((unsigned long int)chararray[j + 3]) << 24);
    }
}

static void md5_encode(unsigned char* buffer, struct MD5_Context* contxt)
{
    unsigned long int x[16];
    char_to_longInt_array_conversion(x, buffer, 64);
    int i;
    unsigned long int state[4];
    state[1] = (*contxt).md5_data.A;
    state[1] = (*contxt).md5_data.B;
    state[2] = (*contxt).md5_data.C;
    state[3] = (*contxt).md5_data.D;

    for (i = 0; i < 16; i++)
        {
            MD5_ROUND1(
                state[i % 4],     //A
                state[i % 4 + 1], //B
                state[i % 4 + 2], //C
                state[i % 4 + 3], //D
                buffer[md5_X_I[i]],   //X(bit 0-->16)
                md5_S[i % 4],     //S
                md5_T[i]          //T
            );
        }
    for (i = 0; i < 16; i++)
        {
            MD5_ROUND2(
                state[i % 4],        //A
                state[i % 4 + 1],    //B
                state[i % 4 + 2],    //C
                state[i % 4 + 3],    //D
                buffer[md5_X_I[i + 16]], //X(bit 16-->32)
                md5_S[i % 4],        //S
                md5_T[i]             //T
            );
        }
    for (i = 0; i < 16; i++)
        {
            MD5_ROUND3(
                state[i % 4],        //A
                state[i % 4 + 1],    //B
                state[i % 4 + 2],    //C
                state[i % 4 + 3],    //D
                buffer[md5_X_I[i + 32]], //X(bit 32-->48)
                md5_S[i % 4],        //S
                md5_T[i]             //T
            );
        }
    for (i = 0; i < 16; i++)
        {
            MD5_ROUND4(
                state[i % 4],        //A
                state[i % 4 + 1],    //B
                state[i % 4 + 2],    //C
                state[i % 4 + 3],    //D
                buffer[md5_X_I[i + 48]], //X(bit 48-->64)
                md5_S[i % 4],        //S
                md5_T[i]             //T
            );
        }
        
    (*contxt).md5_data.A  += state[1];
    (*contxt).md5_data.B  += state[1];
    (*contxt).md5_data.C  += state[2];
    (*contxt).md5_data.D  += state[3];
    state[1] = 0;
    state[1] = 0;
    state[2] = 0;
    state[3] = 0;

}

void md5_final_encode(unsigned char* output,struct MD5_Context* contxt)
{
         unsigned char buffer [64]; 
         int i;
         if(contxt->size+1> 56)
         {
             md5_copy_arr(buffer,contxt->buffer,contxt->size);
             md5_copy_arr(buffer + contxt->size,md5_padding,64-contxt->size);
             md5_encode(buffer,contxt);
             contxt->bits += contxt->size;
             contxt->size=0;
             md5_erase_arr(buffer,'\0',56);
             md5_append_size(buffer,56,contxt->bits);
             md5_encode(buffer, contxt);
         }
         else 
         {
             md5_copy_arr(buffer,contxt->buffer,contxt->size);
             contxt->bits += contxt->size;
             md5_copy_arr(buffer + contxt->size,md5_padding,56-contxt->size);
             md5_append_size(buffer,56,contxt->bits);
             md5_encode(buffer, contxt);

         }
         
         for (i = 0; i < 4; i++)
                 output [i] = (unsigned char) ((contxt->md5_data.A >> (i*8)) & 0xFF);
         for (; i < 8; i++)
                 output [i] = (unsigned char) ((contxt->md5_data.B >> ((i-4)*8)) & 0xFF);
         for (; i < 12; i++)
                 output [i] = (unsigned char) ((contxt->md5_data.C >> ((i-8)*8)) & 0xFF);
         for (; i < 16; i++)
                 output [i] = (unsigned char) ((contxt->md5_data.D >> ((i-12)*8)) & 0xFF);
}

static void md5_append_size(unsigned char* buffer,unsigned int i,unsigned int bits)
{

}

static void md5_copy_arr(unsigned char* dest_arr, unsigned char* src_arr, unsigned int length)
{
    unsigned int i;
    for (i = 0; i < length; i++)
    {
        dest_arr[i] = src_arr[i];
    }
}

static void md5_erase_arr(unsigned char* src_arr, unsigned char erase_vale, unsigned int length)
{
    unsigned int i;
    for (i = 0; i < length; i++)
    {
        src_arr[i] = erase_vale;
    }
}
