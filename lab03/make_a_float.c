// make_a_float ... read in bit strings to build a float value
// COMP1521 Lab03 exercises
// Written by John Shepherd, August 2017
// Completed by ...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>

typedef unsigned int Word;

struct _float {
   // define bit_fields for sign, exp and frac
   // obviously they need to be larger than 1-bit each
   // and may need to be defined in a different order
   unsigned int frac:23, exp:8,sign:1;
};
typedef struct _float Float32;

union _bits32 {
   float   fval;  // interpret the bits as a float
   Word    xval;  // interpret as a single 32-bit word
   Float32 bits;  // manipulate individual bits
};
typedef union _bits32 Union32;

void    checkArgs(int, char **);
Union32 getBits(char *, char *, char *);
char   *showBits(Word, char *);

int main(int argc, char **argv)
{
   union _bits32 u;
   char out[50];

   // here's a hint ...
   u.bits.sign = u.bits.exp = u.bits.frac = 0;

   // check command-line args (all strings of 0/1
   // kills program if args are bad
   checkArgs(argc, argv);

   // convert command-line args into components of
   // a Float32 inside a Union32, and return the union
   u = getBits(argv[1], argv[2], argv[3]);

   printf("bits : %s\n", showBits(u.xval,out));
   printf("float: %0.10f\n", u.fval);
   return 0;
}

// convert three bit-strings (already checked)
// into the components of a struct _float
//TODO
Union32 getBits(char *sign, char *exp, char *frac)
{
   Union32 new;
   int i=0;
   // this line is just to keep gcc happy
   // delete it when you have implemented the function

   new.bits.sign = new.bits.exp = new.bits.frac = 0;

   // convert char *sign into a single bit in new.bits

   if(sign[0]=='1')
   {
      new.bits.sign=1;
   }
   else if(sign[0]=='0')
   {
      new.bits.sign=0;
   }
   else
   {
      //printf("\n'%c' is not either 1 or 0! when i is %d\n",exp[i],i);
      fprintf(stderr,"WARNING:UNRECOGNIZED CHARACTER READ!\n");
   }
   // convert char *exp into an 8-bit value in new.bits
   
   for(i=0;i<8;i++)
   {
      if(exp[i]=='1')
      {
         new.bits.exp+=pow(2,(7-i));
      }
      else if(exp[i]=='0')
      {
         continue;
      }
      else
      {
         //printf("\n'%c' is not either 1 or 0! when i is %d\n",exp[i],i);
         fprintf(stderr,"WARNING:UNRECOGNIZED CHARACTER READ!\n");
      }
   }
   // convert char *frac into a 23-bit value in new.bits
   
   for( i=0;i<23;i++)
   {
      if(frac[i]=='1')
      {
         new.bits.frac+=pow(2,(22-i));
      }
      else if(frac[i]=='0')
      {
         continue;
      }  
      else
      {
         //printf("\n'%c' is not either 1 or 0! when i is %d\n",exp[i],i);
         fprintf(stderr,"WARNING:UNRECOGNIZED CHARACTER READ!\n");
      }
   }
   //printf("       %s%s%s\n",sign,exp,frac);
   return new;
}

// convert a 32-bit bit-stringin val into a sequence
// of '0' and '1' characters in an array buf
// assume that buf has size > 32
// return a pointer to buf
char *showBits(Word val, char *buf)
{
   //printf("the val is %u\n",val);
   // this line is just to keep gcc happy
   // delete it when you have implemented the function
   buf[0] = '\0';
   Word mask=1u<<31;
   int i=0;
   int mask_counter=0;
   
   for(i=0;i<34;i++)
   {
      if(i==1||i==10)
      {
         
         buf[i]=' ';
         //printf("The %d is %c\n",i,buf[i]);
         continue;
      }
      //printf("%u\n",(mask&val));
      if((mask & val)!=0)
      {
         buf[i]='1';
         //printf("The %d is %c\n",i,buf[i]);
      }
      else
      {
         buf[i]='0';
         //printf("The %d is %c\n",i,buf[i]);
      }
      mask=mask>>1;
      mask_counter++;
      //printf("mask is now %u\n",mask);
      //printf("i is now %d ",i);
   }
   //printf("mask has been added %utimes\n",mask_counter);
   assert(i==34);
   buf[i]='\0';
   //printf("%s\n\n\n\n",buf);
   return buf;
  
}

// checks command-line args
// need at least 3, and all must be strings of 0/1
// never returns if it finds a problem
void checkArgs(int argc, char **argv)
{
   int justBits(char *, int);

   if (argc < 3) {
      printf("Usage: %s Sign Exp Frac\n", argv[0]);
      exit(1);
   }
   if (!justBits(argv[1],1)) {
      printf("%s: invalid Sign\n", argv[0]);
      exit(1);
   }
   if (!justBits(argv[2],8)) {
      printf("%s: invalid Exp: %s\n", argv[0], argv[2]);
      exit(1);
   }
   if (!justBits(argv[3],23)) {
      printf("%s: invalid Frac: %s\n", argv[0], argv[3]);
      exit(1);
   }
   return;
}

// check whether a string is all 0/1 and of a given length
int justBits(char *str, int len)
{
   if (strlen(str) != len) return 0;

   while (*str != '\0') {
      if (*str != '0' && *str != '1') return 0;
      str++;
   }
   return 1;
}
