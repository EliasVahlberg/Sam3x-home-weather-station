/**
* @file
*     MD5.c
* @authors 
*     Elias Vahlberg
*     Isak Ringdahl
* \brief 
*     ...
*/
#include <stdint.h>
#pragma region structs

typedef uint32_t *md5_buffer;
#pragma endregion structs

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
#define CONVERT_UINT32(ui, uc)                                                                                                         \
    {                                                                                                                                  \
        (ui) = ((unsigned int)(uc)[0]) | ((unsigned int)(uc)[1] << 8) | ((unsigned int)(uc)[2] << 16) | ((unsigned int)(uc)[3] << 24); \
    }
#pragma endregion MacroFunctions

#pragma region Macros

#define A_init 0x67452301
#define B_init 0xefcdab89
#define C_init 0x98badcfe
#define D_init 0x10325476
#define PADDED_M_LENGTH 56
#define FINAL_M_LENGTH 64
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define MD5_T_F_00 0xd76aa478
#define MD5_T_F_01 0xe8c7b756
#define MD5_T_F_02 0x242070db
#define MD5_T_F_03 0xc1bdceee
#define MD5_T_F_04 0xf57c0faf
#define MD5_T_F_05 0x4787c62a
#define MD5_T_F_06 0xa8304613
#define MD5_T_F_07 0xfd469501
#define MD5_T_F_08 0x698098d8
#define MD5_T_F_09 0x8b44f7af
#define MD5_T_F_10 0xffff5bb1
#define MD5_T_F_11 0x895cd7be
#define MD5_T_F_12 0x6b901122
#define MD5_T_F_13 0xfd987193
#define MD5_T_F_14 0xa679438e
#define MD5_T_F_15 0x49b40821

#define MD5_T_G_00 0xf61e2562
#define MD5_T_G_01 0xc040b340
#define MD5_T_G_02 0x265e5a51
#define MD5_T_G_03 0xe9b6c7aa
#define MD5_T_G_04 0xd62f105d
#define MD5_T_G_05 0x2441453
#define MD5_T_G_06 0xd8a1e681
#define MD5_T_G_07 0xe7d3fbc8
#define MD5_T_G_08 0x21e1cde6
#define MD5_T_G_09 0xc33707d6
#define MD5_T_G_10 0xf4d50d87
#define MD5_T_G_11 0x455a14ed
#define MD5_T_G_12 0xa9e3e905
#define MD5_T_G_13 0xfcefa3f8
#define MD5_T_G_14 0x676f02d9
#define MD5_T_G_15 0x8d2a4c8a

#define MD5_T_H_00 0xfffa3942
#define MD5_T_H_01 0x8771f681
#define MD5_T_H_02 0x6d9d6122
#define MD5_T_H_03 0xfde5380c
#define MD5_T_H_04 0xa4beea44
#define MD5_T_H_05 0x4bdecfa9
#define MD5_T_H_06 0xf6bb4b60
#define MD5_T_H_07 0xbebfbc70
#define MD5_T_H_08 0x289b7ec6
#define MD5_T_H_09 0xeaa127fa
#define MD5_T_H_10 0xd4ef3085
#define MD5_T_H_11 0x4881d05
#define MD5_T_H_12 0xd9d4d039
#define MD5_T_H_13 0xe6db99e5
#define MD5_T_H_14 0x1fa27cf8
#define MD5_T_H_15 0xc4ac5665

#define MD5_T_I_00 0xf4292244
#define MD5_T_I_01 0x432aff97
#define MD5_T_I_02 0xab9423a7
#define MD5_T_I_03 0xfc93a039
#define MD5_T_I_04 0x655b59c3
#define MD5_T_I_05 0x8f0ccc92
#define MD5_T_I_06 0xffeff47d
#define MD5_T_I_07 0x85845dd1
#define MD5_T_I_08 0x6fa87e4f
#define MD5_T_I_09 0xfe2ce6e0
#define MD5_T_I_10 0xa3014314
#define MD5_T_I_11 0x4e0811a1
#define MD5_T_I_12 0xf7537e82
#define MD5_T_I_13 0xbd3af235
#define MD5_T_I_14 0x2ad7d2bb
#define MD5_T_I_15 0xeb86d391

#pragma endregion Macros

#pragma region Functions
static void md5_hash(unsigned char *message, long long length, unsigned char *output);
static void md5_definitive_hash(unsigned char *message, long long length,unsigned char *salt, long long salt_len,int itterations, unsigned char *output);
static void md5_salt_hash(unsigned char *message, long long length,unsigned char *salt, long long salt_len, unsigned char *output);
static void md5_itterating_hash(unsigned char *message, long long length, unsigned char *output,int itterations);
static int append_padding_bits(unsigned char *m_padding, int l_padding);
static void append_length(unsigned char *m, long long length);
static void init_buffer(md5_buffer buff);
static void process_hash(md5_buffer buff, uint32_t *X);
static void finalize_hash(md5_buffer buff, unsigned char *output);
static void u_char_copy_erase(unsigned char *dest,unsigned char *src, int len);
static void u_char_erase(unsigned char* src, int len);
static void u_int32_erase(uint32_t* src, int len);

#pragma endregion Functions

static void md5_definitive_hash(unsigned char *message, long long length,unsigned char *salt, long long salt_len,int itterations, unsigned char *output)
{
    unsigned char temp_out_in_put[16];
    md5_salt_hash(message,length,salt,salt_len,temp_out_in_put);
    md5_itterating_hash(temp_out_in_put,16,output,itterations);
}

static void md5_salt_hash(unsigned char *message, long long length,unsigned char *salt, long long salt_len, unsigned char *output)
{
    unsigned char salt_output[16];
    unsigned char* salt_copy = calloc((int)salt_len,sizeof(char));
    if(salt_copy == NULL)
        return;
    for (int i = 0; i < salt_len; i++)
     salt_copy[i] = salt[i];
    
    md5_hash(salt,salt_len,salt_output);
    unsigned char* salted_message = calloc(length+16,sizeof(char));
    if(salt_copy == NULL)
        return;
    for (int i = 0; i < length+16; i++)
    {
        salted_message[i] = (i>=length)?salt_output[i-length]:message[i];
        if(i<length)
            message[i] = (unsigned char) 0;
        else
            salt_output[i-length] = (unsigned char) 0;
    }
    md5_hash(salted_message,length+16,output);
    for (int i = 0; i < salt_len; i++)
        salt[i]=salt_copy[i];
    
    free(salted_message);
    free(salt_copy);
}

static void md5_itterating_hash(unsigned char *message, long long length, unsigned char *output,int itterations)
{
    unsigned char* temp_out_in_put1 = calloc(16,sizeof(char));
    unsigned char* temp_out_in_put2 = calloc(16,sizeof(char));
    if(itterations != 0)
        md5_hash(message,length,temp_out_in_put1);
    else
    {
        md5_hash(message,length,output);
        return;
    }
    int i;
    for (i = 0; i < itterations; i++)
    {
        if(i%2==0)
            md5_hash(temp_out_in_put1,16,temp_out_in_put2);
        else
            md5_hash(temp_out_in_put2,16,temp_out_in_put1);       
    }
    for (int j = 0; j < 16; j++)
            output[j] = (i%2==0)?temp_out_in_put1[j]:temp_out_in_put2[j];
    i=0;
    u_char_erase(temp_out_in_put1,16);
    u_char_erase(temp_out_in_put2,16);
    free(temp_out_in_put1);
    free(temp_out_in_put2);
}

static void md5_hash(unsigned char *message, long long length, unsigned char *output)
{
    unsigned char *m;
    int i = 0, l = 0;
    if (length % 64 != 56)
    {
        l = (length % 64 < 56) ? (length + (64 - length % 64)) : (length + (64 - length % 64)) + (64);
        m = (unsigned char *)calloc(l, sizeof(char));
        if (m == NULL)
            return; //Malloc fail
        u_char_copy_erase(m, message, length);
        i = append_padding_bits((unsigned char *)(m + length), l - length - 8);
    }
    else
    {
        l = length + 8;
        m = (unsigned char *)calloc(l, sizeof(char));
        if (m == NULL)
            return; //Malloc fail
        u_char_copy_erase(m, message, length);
    }
    i += length;
    length *= 8;
    append_length((unsigned char *)(m + i), length);
    md5_buffer buff = calloc(4,sizeof(uint32_t));
    uint32_t *X = calloc(16, sizeof(uint32_t));
    init_buffer(buff);
    for (int j = 0; j < l; j +=64)
    {
        for (int jj = 0; jj < 16; jj++)
        {
            CONVERT_UINT32(X[jj], &(m[j * 64 + jj]));
        }
        process_hash(buff, X);
    }
    finalize_hash(buff,output);

    u_char_erase(m,l);
    u_int32_erase(X,16);
    u_int32_erase(buff,4);
    i=0;
    l=0;
    free(buff);
    free(m);
    free(X);

}

static int append_padding_bits(unsigned char *m_padding, int l_padding)
{
    int i = 0;
    m_padding[i++] = (unsigned char)128;
    for (; i < l_padding; i++)
    {
        m_padding[i] = '\0';
    }
    return i;
}

static void append_length(unsigned char *m, long long length)
{

    for (int i = 0; i < 8; ++i)
    {
        m[i] = (unsigned char)((((unsigned long long) length) >> (56 - (8*i))) & 0xFFu);
    }
}

static void init_buffer(md5_buffer buff)
{
    buff[0] = (uint32_t) A_init;
    buff[1] = (uint32_t) B_init;
    buff[2] = (uint32_t) C_init;
    buff[3] = (uint32_t) D_init;
}

static void process_hash(md5_buffer buff, uint32_t *X)
{
    uint32_t a =buff[0], b =buff[1], c =buff[2], d =buff[3];
    
    MD5_ROUND1(a, b, c, d, X[0], S11, MD5_T_F_00);  /* 1 */
    MD5_ROUND1(d, a, b, c, X[1], S12, MD5_T_F_01);  /* 2 */
    MD5_ROUND1(c, d, a, b, X[2], S13, MD5_T_F_02);  /* 3 */
    MD5_ROUND1(b, c, d, a, X[3], S14, MD5_T_F_03);  /* 4 */
    MD5_ROUND1(a, b, c, d, X[4], S11, MD5_T_F_04);  /* 5 */
    MD5_ROUND1(d, a, b, c, X[5], S12, MD5_T_F_05);  /* 6 */
    MD5_ROUND1(c, d, a, b, X[6], S13, MD5_T_F_06);  /* 7 */
    MD5_ROUND1(b, c, d, a, X[7], S14, MD5_T_F_07);  /* 8 */
    MD5_ROUND1(a, b, c, d, X[8], S11, MD5_T_F_08);  /* 9 */
    MD5_ROUND1(d, a, b, c, X[9], S12, MD5_T_F_09);  /* 10 */
    MD5_ROUND1(c, d, a, b, X[10], S13, MD5_T_F_10); /* 11 */
    MD5_ROUND1(b, c, d, a, X[11], S14, MD5_T_F_11); /* 12 */
    MD5_ROUND1(a, b, c, d, X[12], S11, MD5_T_F_12); /* 13 */
    MD5_ROUND1(d, a, b, c, X[13], S12, MD5_T_F_13); /* 14 */
    MD5_ROUND1(c, d, a, b, X[14], S13, MD5_T_F_14); /* 15 */
    MD5_ROUND1(b, c, d, a, X[15], S14, MD5_T_F_15); /* 16 */
    
    MD5_ROUND2(a, b, c, d, X[1], S21, MD5_T_G_00);  /* 17 */
    MD5_ROUND2(d, a, b, c, X[6], S22, MD5_T_G_01);  /* 18 */
    MD5_ROUND2(c, d, a, b, X[11], S23, MD5_T_G_02); /* 19 */
    MD5_ROUND2(b, c, d, a, X[0], S24, MD5_T_G_03);  /* 20 */
    MD5_ROUND2(a, b, c, d, X[5], S21, MD5_T_G_04);  /* 21 */
    MD5_ROUND2(d, a, b, c, X[10], S22, MD5_T_G_05); /* 22 */
    MD5_ROUND2(c, d, a, b, X[15], S23, MD5_T_G_06); /* 23 */
    MD5_ROUND2(b, c, d, a, X[4], S24, MD5_T_G_07);  /* 24 */
    MD5_ROUND2(a, b, c, d, X[9], S21, MD5_T_G_08);  /* 25 */
    MD5_ROUND2(d, a, b, c, X[14], S22, MD5_T_G_09); /* 26 */
    MD5_ROUND2(c, d, a, b, X[3], S23, MD5_T_G_10);  /* 27 */
    MD5_ROUND2(b, c, d, a, X[8], S24, MD5_T_G_11);  /* 28 */
    MD5_ROUND2(a, b, c, d, X[13], S21, MD5_T_G_12); /* 29 */
    MD5_ROUND2(d, a, b, c, X[2], S22, MD5_T_G_13);  /* 30 */
    MD5_ROUND2(c, d, a, b, X[7], S23, MD5_T_G_14);  /* 31 */
    MD5_ROUND2(b, c, d, a, X[12], S24, MD5_T_G_15); /* 32 */
    
    MD5_ROUND3(a, b, c, d, X[5], S31, MD5_T_H_00);  /* 33 */
    MD5_ROUND3(d, a, b, c, X[8], S32, MD5_T_H_01);  /* 34 */
    MD5_ROUND3(c, d, a, b, X[11], S33, MD5_T_H_02); /* 35 */
    MD5_ROUND3(b, c, d, a, X[14], S34, MD5_T_H_03); /* 36 */
    MD5_ROUND3(a, b, c, d, X[1], S31, MD5_T_H_04);  /* 37 */
    MD5_ROUND3(d, a, b, c, X[4], S32, MD5_T_H_05);  /* 38 */
    MD5_ROUND3(c, d, a, b, X[7], S33, MD5_T_H_06);  /* 39 */
    MD5_ROUND3(b, c, d, a, X[10], S34, MD5_T_H_07); /* 40 */
    MD5_ROUND3(a, b, c, d, X[13], S31, MD5_T_H_08); /* 41 */
    MD5_ROUND3(d, a, b, c, X[0], S32, MD5_T_H_09);  /* 42 */
    MD5_ROUND3(c, d, a, b, X[3], S33, MD5_T_H_10);  /* 43 */
    MD5_ROUND3(b, c, d, a, X[6], S34, MD5_T_H_11);  /* 44 */
    MD5_ROUND3(a, b, c, d, X[9], S31, MD5_T_H_12);  /* 45 */
    MD5_ROUND3(d, a, b, c, X[12], S32, MD5_T_H_13); /* 46 */
    MD5_ROUND3(c, d, a, b, X[15], S33, MD5_T_H_14); /* 47 */
    MD5_ROUND3(b, c, d, a, X[2], S34, MD5_T_H_15);  /* 48 */
    
    MD5_ROUND4(a, b, c, d, X[0], S41, MD5_T_I_00);  /* 49 */
    MD5_ROUND4(d, a, b, c, X[7], S42, MD5_T_I_01);  /* 50 */
    MD5_ROUND4(c, d, a, b, X[14], S43, MD5_T_I_02); /* 51 */
    MD5_ROUND4(b, c, d, a, X[5], S44, MD5_T_I_03);  /* 52 */
    MD5_ROUND4(a, b, c, d, X[12], S41, MD5_T_I_04); /* 53 */
    MD5_ROUND4(d, a, b, c, X[3], S42, MD5_T_I_05);  /* 54 */
    MD5_ROUND4(c, d, a, b, X[10], S43, MD5_T_I_06); /* 55 */
    MD5_ROUND4(b, c, d, a, X[1], S44, MD5_T_I_07);  /* 56 */
    MD5_ROUND4(a, b, c, d, X[8], S41, MD5_T_I_08);  /* 57 */
    MD5_ROUND4(d, a, b, c, X[15], S42, MD5_T_I_09); /* 58 */
    MD5_ROUND4(c, d, a, b, X[6], S43, MD5_T_I_10);  /* 59 */
    MD5_ROUND4(b, c, d, a, X[13], S44, MD5_T_I_11); /* 60 */
    MD5_ROUND4(a, b, c, d, X[4], S41, MD5_T_I_12);  /* 61 */
    MD5_ROUND4(d, a, b, c, X[11], S42, MD5_T_I_13); /* 62 */
    MD5_ROUND4(c, d, a, b, X[2], S43, MD5_T_I_14);  /* 63 */
    MD5_ROUND4(b, c, d, a, X[9], S44, MD5_T_I_15);  /* 64 */
    buff[0] += a;
    buff[1] += b;
    buff[2] += c;
    buff[3] += d;
}

static void finalize_hash(md5_buffer buff, unsigned char *output)
{
    unsigned int i,ii;
    for (i = 0, ii = 0; ii < 16; i++, ii += 4)
    {
        output[ii]     = (unsigned char) (buff[i] & 0xff);
        output[ii + 1] = (unsigned char)((buff[i] >> 8) & 0xff);
        output[ii + 2] = (unsigned char)((buff[i] >> 16) & 0xff);
        output[ii + 3] = (unsigned char)((buff[i] >> 24) & 0xff);
    }
}

static void u_char_erase(unsigned char* src, int len)
{
    for (int i = 0; i < len; i++)
    {
        src[i] = '\0';
    }
}

static void u_char_copy_erase(unsigned char *dest,unsigned char *src, int len)
{
    for (int i = 0; i < (int) len; i++)
    {
        dest[i] =(unsigned char)  src[i];
        src[i] = '\0';
    }
}

static void u_int32_erase(uint32_t* src, int len)
{
    for (int i = 0; i < len; i++)
    {
        src[i]=((uint32_t)0);
    }
}
