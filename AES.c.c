// Online C compiler to run C program online
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char s_box[16] =
 {
    0x9, 0x4, 0xA, 0xB,
    0xD, 0x1, 0x8, 0x5,
    0x6, 0x2, 0x0, 0x3,
    0xC, 0xE, 0xF, 0x7

 };

unsigned char Inverse_s_box[16] =
 {
    0x8, 0x5, 0x9, 0xB,
    0x1, 0x7, 0x8, 0xF,
    0x6, 0x0, 0x2, 0x3,
    0xC, 0x4, 0xD, 0xE

 };


unsigned char mul4[] =
 {
   0x0, 0x4, 0x8, 0xC,
   0x3, 0x7, 0xB, 0xF,
   0x6, 0x2, 0xE, 0xA,
   0x5, 0x1, 0xD, 0x9,
 };

 unsigned char mul9[] =
 {
   0x0, 0x9, 0x1, 0x8,
   0x2, 0xB, 0x3, 0xA,
   0x4, 0xD, 0x5, 0xC,
   0x6, 0xF, 0x7, 0xE,

 };


 unsigned char mul2[] =
 {
   0x0, 0x2, 0x4, 0x6,
   0x8, 0xA, 0xC, 0xE,
   0x3, 0x1, 0x7, 0x5,
   0xB, 0x9, 0xF, 0xD,

 };
unsigned short InverseSubShiftRow(unsigned short input)
{
   unsigned short output;

   unsigned char x = ((input & 0xff00)>>8);
   unsigned char y = (input & 0x00FF);

   unsigned char x_LeftByte =(x & 0x0f);
   unsigned char y_LeftByte = (x & 0xf0) >> 4;


   unsigned char x_RightByte =(y & 0x0f);
   unsigned char y_RightByte = (y & 0xf0) >> 4;

   unsigned char tmp[4];

   tmp[0] = Inverse_s_box[(int)y_LeftByte];
   tmp[1] = Inverse_s_box[(int)x_LeftByte];
   tmp[2] = Inverse_s_box[(int)y_RightByte];
   tmp[3] = Inverse_s_box[(int)x_RightByte];


   output = tmp[0]<<12 | tmp[3]<<8 | tmp [2]<<4 | tmp[1];
   return output;
}

unsigned short SubShiftRow(unsigned short input)
{
   unsigned short output;

   unsigned char x = ((input & 0xff00)>>8);
   unsigned char y = (input & 0x00FF);

   unsigned char x_LeftByte =(x & 0x0f);
   unsigned char y_LeftByte = (x & 0xf0) >> 4;


   unsigned char x_RightByte =(y & 0x0f);
   unsigned char y_RightByte = (y & 0xf0) >> 4;

   unsigned char tmp[4];

   tmp[0] = s_box[(int)y_LeftByte];
   tmp[1] = s_box[(int)x_LeftByte];
   tmp[2] = s_box[(int)y_RightByte];
   tmp[3] = s_box[(int)x_RightByte];


   output = tmp[0]<<12 | tmp[3]<<8 | tmp [2]<<4 | tmp[1];
   return output;
}

unsigned short mixCols(unsigned short input)
{
    unsigned short output;

   unsigned char x = ((input & 0xff00)>>8);
   unsigned char y = (input & 0x00FF);

   unsigned char y_LeftByte =(x & 0x0f);
   unsigned char x_LeftByte = (x & 0xf0) >> 4;


   unsigned char y_RightByte =(y & 0x0f);
   unsigned char x_RightByte = (y & 0xf0) >> 4;

   unsigned char tmp[4];

   tmp[0] = mul4[y_LeftByte]  ^ x_LeftByte ;
   tmp[1] = mul4[x_LeftByte]  ^ y_LeftByte ;
   tmp[2] = mul4[y_RightByte] ^ x_RightByte ;
   tmp[3] = mul4[x_RightByte] ^ y_RightByte ;


   output = tmp[0]<<12 | tmp[1]<<8 | tmp [2]<<4 | tmp[3];
   return output;
}

unsigned short inverseMixCols(unsigned short input)
{

    unsigned short output;

   unsigned char x = ((input & 0xff00)>>8);
   unsigned char y = (input & 0x00FF);

   unsigned char y_LeftByte =(x & 0x0f);
   unsigned char x_LeftByte = (x & 0xf0) >> 4;


   unsigned char y_RightByte =(y & 0x0f);
   unsigned char x_RightByte = (y & 0xf0) >> 4;

   unsigned char tmp[4];

   tmp[0] = mul2[y_LeftByte] ^ mul9[x_LeftByte] ;
   tmp[1] = mul2[x_LeftByte] ^ mul9[y_LeftByte] ;
   tmp[2] = mul2[y_RightByte] ^ mul9[x_RightByte] ;
   tmp[3] = mul2[x_RightByte] ^ mul9[y_RightByte] ;


   output = tmp[0]<<12 | tmp[1]<<8 | tmp [2]<<4 | tmp[3];
   return output;
}
unsigned char RotateNibble(unsigned char x)
{
   unsigned char templeft;
   unsigned char tempright;

   templeft = (x & 0xF0)>>4;
   tempright = (x & 0x0F)<< 4;
   return (tempright|templeft);
}
unsigned char SubNibble(unsigned char x)
{

   unsigned char templeft = (x & 0xF0)>>4;
   unsigned char tempright = (x & 0x0F);


   unsigned char tmp[2];

   tmp[0] = s_box[(int)templeft];
   tmp[1] = s_box[(int)tempright];


   return (tmp[0]<<4 | tmp[1]);

}

 /// @brief
 /// @param array of 16 bits keys with only one input
 /// @param counter
 /// @return returns key generated
 void key_generator(unsigned short* subKey, unsigned short Firstkey)
 {
   unsigned char w[6] = {0};
   unsigned char rightKey =(Firstkey & 0x00ff);
   unsigned char leftKey = (Firstkey & 0xff00)>>8;


   for(int i=0; i< sizeof(w)/sizeof(char) +1; i++)
   {
      if (i ==0)
      {
         w[0]= leftKey;

      }
      else if (i ==1)
      {
         w[1]= rightKey;

      }
      else if (i == 2 )
      {
         w[i] = w[i-2] ^ 0b10000000 ^ SubNibble(RotateNibble(w[i-1]));
      }
      else if (i == 4 )
      {
         w[i] = w[i-2] ^ 0b00110000 ^ SubNibble(RotateNibble(w[i-1]));
      }
      else
      {
         w[i] = w[i-1] ^ w[i-2];

      }

   }

   int k =0;
   for(int i=0; i<sizeof(subKey)/sizeof(short) +1; i++)
   {

      subKey[i]= (w[k]<< 8 | w[k+1]);
      k= k+2;

   }


 }
unsigned short subKey[3] = {0};

unsigned short AESEncryption(unsigned short plainText, unsigned short Firstkey)
{
     key_generator(subKey, Firstkey);

   //Initial round
   unsigned short cipher;
   cipher = plainText ^ subKey[0];

   //1st Round
   //Nibble Substitution (S-boxes) & Shift row
   cipher= SubShiftRow(cipher);

   //Mix Cols
   cipher=mixCols(cipher);

   //AddKey1
   cipher = cipher ^ subKey[1];

   //Final Round
   //Nibble Substitution (S-boxes) & Shift row
   cipher= SubShiftRow(cipher);

   //AddKey2
   cipher = cipher ^ subKey[2];

   return cipher;

}
unsigned short AESDecryption(unsigned short CipherText, unsigned short Firstkey)
{
   key_generator(subKey, Firstkey);
   unsigned short plainText;

   //AddKey2
   plainText = CipherText ^ subKey[2];


   //Nibble Substitution (S-boxes) & Shift row
   plainText= InverseSubShiftRow(plainText);


   //AddKey1
   plainText = plainText ^ subKey[1];

   //Mix Cols
   plainText=inverseMixCols(plainText);

   //Nibble Substitution (S-boxes) & Shift row
   plainText= InverseSubShiftRow(plainText);

   plainText = plainText ^ subKey[0];


   return plainText;

}

int main(int argc, const char *argv[])
{
    if (argc != 4)
    {
        printf("Needs 3 parameters. Expects:\n\t%s  ENC|DEC  key  data\n", argv[0]);
        exit(1);
    }

     unsigned short key = strtol(argv[2], NULL, 16);
     unsigned short data = strtol(argv[3], NULL, 16);

    if (strcmp(argv[1], "ENC") == 0)
        printf("%x",AESEncryption(data, key));

    if (strcmp(argv[1], "DEC") == 0)
        printf("%x",AESDecryption(data, key));

    else
    {
        //printf("\nInvalid Input.\n");
         exit(1);
    }

    return 0;
}
