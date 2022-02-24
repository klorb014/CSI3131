/*--------------------------------------------------------------------

File: dp.c
Student name(s): Kenneth Lorbetskie
Student id(s): 300013409
Group No.:  62

Description:  Double pipe program.  To pipe the output from the standard
          output to the standard input of two other processes.
          Usage:  dp  <cmd1> : <cmd2> : <cmd3>
          Output from process created with cmd1 is piped to
          processes created with cmd2 and cmd3

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
/* prototypes */
int doublePipe(char **,char **,char **);
int isPrime(int);

/*--------------------------------------------------------------------

File: dp.c

Description: Main will parse the command line arguments into three arrays
         of strings one for each command to execv().
--------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
   int val;
   char input[20];

   printf( "Enter a value :");
   fgets(input,20,stdin);
   input[strlen(input)-1] = '\0';
   val = atoi(input);

   if(val < 3)
   {
     printf("Input an even number greater than 2\n");
     exit(1);
   }

   if(val%2 == 1)
   {
     printf("Input an even number greater than 2\n");
     exit(1);
   }


   //exit(doublePipe(cmd1,cmd2,cmd3));

   printf( "\nYou entered: %d \n", val);

   int* array = calloc(val, sizeof(int) );

   int i;
   for ( i = 0; i < val; i++ ){
     array[i] = i+1;
   }

  int leftPtr = 0;
  int rightPtr = val - 1;

  int result[2];
  int left;
  int right;
  while (rightPtr > leftPtr){
    left = array[leftPtr];
    right = array[rightPtr];
    
    if (isPrime(left) == 1 && isPrime(right) == 1){
      printf("%d %d \n", left, right);
      if (left + right == val){
        break;
      } else if (left + right > val){
        rightPtr -= 1;
      }else{
        leftPtr += 1;
      }
    }else if (isPrime(left) == 1 && isPrime(right) != 1){
      rightPtr -= 1;
    }else if (isPrime(left) != 1 && isPrime(right) == 1){
      leftPtr += 1;
    }else{
      rightPtr -= 1;
      leftPtr += 1;
    }
  }

   free(array);
	  

   return 0;

}


/*--------------------------------------------------------------------------
  --------------------------------------------------------------------------
Function: doublePipe()

Description:  Starts three processes, one for each of cmd1, cmd2, and cmd3.
          The parent process will receive the output from cmd1 and
          copy the output to the other two processes.
-------------------------------------------------------------------------*/

int doublePipe(char **cmd1, char **cmd2, char **cmd3)
{

	return 0;
}

int isPrime(int num){
   if (num == 2 || num == 3)
        return 1;

    if (num <= 1 || num % 2 == 0 || num % 3 == 0)
        return 0;

    for (int i = 5; i * i <= num; i += 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;
    }
    return 1;
}
