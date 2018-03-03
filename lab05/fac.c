#include<stdio.h>

int fac(int n);
int main(void)
{
   int n =0;
   int solution=0;
   printf("Enter a number\n");
   scanf("%d",&n);
   
   printf("%d\n",fac(n));
   return 0;
}

int fac(int n)
{
   if(n==0)
   {
      return 1;
   }
   else
   {
      return n*fac(n-1);
   }
}
