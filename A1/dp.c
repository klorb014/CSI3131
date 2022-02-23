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

/*--------------------------------------------------------------------

File: dp.c

Description: Main will parse the command line arguments into three arrays
         of strings one for each command to execv().
--------------------------------------------------------------------*/

int main(int argc, char *argv[])
{

   int i,j;         /*indexes into arrays */
   char *cmd1[10];  /*array for arguments of first command */
   char *cmd2[10];  /*array for arguments of second command */
   char *cmd3[10];  /*array for arguments of third command */
   if(argc == 1)
   {
     printf("Usage: dp <cmd1 arg...> : <cmd2 arg...> : <cmd3 arg....>\n");
     exit(1);
   }

   /* get the first command */
   for(i=1,j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found first command */
      cmd1[j]=argv[i];
   }
   cmd1[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only one command found\n");
      exit(1);
   }
   else i++;

   /* get the second command */
   for(j=0 ; i<argc ; i++,j++)
   {
      if(!strcmp(argv[i],":")) break; /* found second command */
      cmd2[j]=argv[i];
   }
   cmd2[j]=NULL;
   if(i == argc) /* missing : */
   {
      printf("Bad command syntax - only two commands found\n");
      exit(1);
   }
   else i++;

   /* get the third command */
   for(j=0 ; i<argc ; i++,j++) cmd3[j]=argv[i];
   cmd3[j]=NULL;
   if(j==0) /* no command after last : */
   {
      printf("Bad command syntax - missing third command\n");
      exit(1);
   }

   exit(doublePipe(cmd1,cmd2,cmd3));
}

/*--------------------------------------------------------------------------
  ----------------- You have to implement this function --------------------
  --------------------------------------------------------------------------
Function: doublePipe()

Description:  Starts three processes, one for each of cmd1, cmd2, and cmd3.
          The parent process will receive the output from cmd1 and
          copy the output to the other two processes.
-------------------------------------------------------------------------*/

int doublePipe(char **cmd1, char **cmd2, char **cmd3)
{

   int     pipe1fd[2], pipe2fd[2];
   pid_t   pid;

   // Create two pipes
   pipe(pipe1fd);
   pipe(pipe2fd);

   pid = fork();    
	if (pid < 0)  exit(-1); 
   if(pid == 0)
   {  
   // Set write end pipe #1 to  child stdout
   dup2(pipe1fd[1], 1);
   close(pipe1fd[1]);
   close(pipe1fd[0]);

   // Close both ends of pipe #2 
   close(pipe2fd[1]);
   close(pipe2fd[0]);

   // Execute cmd1
   execvp(cmd1[0], cmd1);
   exit(1);
   }

   pid = fork();    
	if (pid < 0)  exit(-1); 
   if(pid == 0)
   {
   // Set write end pipe #2 to  child stdout
   dup2(pipe2fd[1], 1);
   close(pipe2fd[1]);
   close(pipe2fd[0]);

   // Close both ends of pipe #1 
   close(pipe1fd[1]);
   close(pipe1fd[0]);

   // Execute cmd1
   execvp(cmd1[0], cmd1);
   exit(1);
   }else{
      pid = fork();
      if (pid < 0)  exit(-1); 
      if(pid == 0){
         // Set read end pipe #1 to  child stdin
         dup2(pipe1fd[0], 0);
         close(pipe1fd[0]);
         close(pipe1fd[1]);

         // Close both ends of pipe #2
         close(pipe2fd[0]);
         close(pipe2fd[1]);

         // Execute cmd2
         execvp(cmd2[0], cmd2);     
      }
      pid = fork();
      if (pid < 0)  exit(-1); 
      if(pid == 0){
         // Set read end pipe #2 to  child stdin
         dup2(pipe2fd[0], 0);
         close(pipe2fd[0]);
         close(pipe2fd[1]);

         // Close both ends of pipe #1
         close(pipe1fd[0]);
         close(pipe1fd[1]);

         // Execute cmd3
         execvp(cmd3[0], cmd3);
      }
   }
   
   // Close pipe #1 and pipe #2
   close(pipe1fd[0]);
   close(pipe1fd[1]);
   close(pipe2fd[0]);
   close(pipe2fd[1]);

   wait(0);
   wait(0);
   wait(0);
   wait(0);

	return 0;
}