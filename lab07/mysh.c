// mysh.c ... a very simple shell
// Started by John Shepherd, October 2017
// Completed by <<YOU>>, October 2017

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

extern char *strdup(char *);
void trim(char *);
char **tokenise(char *, char *);
void freeTokens(char **);
int isExecutable(char *);
void execute(char **, char **, char **);

int main(int argc, char *argv[], char *envp[])
{
   pid_t pid;   // pid of child process
   int stat;    // return status of child
   char **path; // array of directory names

   // set up command PATH from environment variable
   int i;
   for (i = 0; envp[i] != NULL; i++) {
      if (strncmp(envp[i], "PATH", 4) == 0) break;
   }
   if (envp[i] == NULL)
      path = tokenise("/bin:/usr/bin",":");
   else
      // &envp[i][5] ignores "PATH="
      path = tokenise(&envp[i][5],":");

#ifdef DBUG
//   for (i = 0; path[i] != NULL;i++)
//      printf("dir[%d] = %s\n",i,path[i]);
#endif

   // main loop: print prompt, read line, execute command
   char line[BUFSIZ];
   printf("mysh$ ");
   while (fgets(line, BUFSIZ, stdin) != NULL) {
      trim(line); // remove leading/trailing space
      if (strcmp(line,"exit") == 0) break;
      if (strcmp(line,"") == 0) { printf("mysh$ "); continue; }

      // TODO: implement the tokenise/fork/execute/cleanup code
      //tokenise the command line
      //fork a copy of itself
      //the parent shell process then waits for the child to complete
      //the child shell process invokes the execute() function
      //the parent shell process then cleans up the tokens and prints another prompt

      char ** cmd = tokenise(line," ");
      pid = fork();
      if (pid < 0) {
          perror("fork failed");
          exit(EXIT_FAILURE);

      }else if (pid > 0) {
        //successfully created a fork, wait for the chjild to complete
          wait(&stat);
          freeTokens(cmd);
      } else {
          //printf("%s\n",line);
          //printf("%s\n",line);
          //printf("%s\n",commands);
          
          execute(cmd,path,envp);

      }

      printf("mysh$ ");
   }
   freeTokens(path);
   printf("\n");
   return(EXIT_SUCCESS);
}

// execute: run a program, given command-line args, path and envp
void execute(char **args, char **path, char **envp)
{
   // TODO: implement the find-the-executable and execve() it code
   //args = tokenise the command line
//if (args[0] starts with '/' or '.') {
   //check if the file called args[0] is executable
   //if so, use args[0] as the command
//else {
   //for each of the directories D in the path {
      //see if an executable file called "D/args[0]" exists
      //if it does, use that file name as the command
   //}
   char *cmd = NULL;
   
   if(args[0][0]=='/'||args[0][0]=='.')
   {
      if(isExecutable(args[0]))
      {
         printf("executable!\n");
         cmd = strdup(args[0]);
      }
   }
      else
      {
         int i = 0;
         
         while(path[i]!=NULL)
         {
            char temp[50];
            sprintf(temp, "%s/%s",path[i],args[0]);
            //printf("checking if%s/%s is executable",path[i],args[0]);
            if(isExecutable(temp))
            {
               
               cmd = strdup(temp);
               break;
            }
            i++;
         }
      }
    if(cmd ==  NULL)
    {
      printf("%s:comand not found.\n",args[0]);
    }
    else
    {
      printf("Running %s\n",cmd);
      execve(cmd, args,envp);
      //free(cmd);
    }

}

// isExecutable: check whether this process can execute a file
int isExecutable(char *cmd)
{
   struct stat s;
   // must be accessible
   if (stat(cmd, &s) < 0)
      return 0;
   // must be a regular file
   //if (!(s.st_mode & S_IFREG))
   if (!S_ISREG(s.st_mode))
      return 0;
   // if it's owner executable by us, ok
   if (s.st_uid == getuid() && s.st_mode & S_IXUSR)
      return 1;
   // if it's group executable by us, ok
   if (s.st_gid == getgid() && s.st_mode & S_IXGRP)
      return 1;
   // if it's other executable by us, ok
   if (s.st_mode & S_IXOTH)
      return 1;
   return 0;
}

// tokenise: split a string around a set of separators
// create an array of separate strings
// final array element contains NULL
char **tokenise(char *str, char *sep)
{
   // temp copy of string, because strtok() mangles it
   char *tmp;
   // count tokens
   tmp = strdup(str);
   int n = 0;
   strtok(tmp, sep); n++;
   while (strtok(NULL, sep) != NULL) n++;
   free(tmp);
   // allocate array for argv strings
   char **strings = malloc((n+1)*sizeof(char *));
   assert(strings != NULL);
   // now tokenise and fill array
   tmp = strdup(str);
   char *next; int i = 0;
   next = strtok(tmp, sep);
   strings[i++] = strdup(next);
   while ((next = strtok(NULL,sep)) != NULL)
      strings[i++] = strdup(next);
   strings[i] = NULL;
   free(tmp);
   return strings;
}

// freeTokens: free memory associated with array of tokens
void freeTokens(char **toks)
{
   for (int i = 0; toks[i] != NULL; i++)
      free(toks[i]);
   free(toks);
}

// trim: remove leading/trailing spaces from a string
void trim(char *str)
{
   int first, last;
   first = 0;
   while (isspace(str[first])) first++;
   last  = strlen(str)-1;
   while (isspace(str[last])) last--;
   int i, j = 0;
   for (i = first; i <= last; i++) str[j++] = str[i];
   str[j] = '\0';
}
