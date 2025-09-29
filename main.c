#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <readline/history.h>
#include <readline/readline.h>

#define MAX_CHARS 4096
#define MAX_ARGS 128

#define clear() printf("\033[H\033[J")

char buffer[MAX_CHARS];
char prompt[128];

int getInput(char* buf)
{
  char* str;
  // if(fgets(buf, MAX_CHARS, stdin) != NULL)
  // {
  //   buf[strcspn(buf, "\n")] = '\0';    
  //   return; 
  // }
  str = readline(prompt);
  if(str != NULL)
  {
    add_history(str);
    strncpy(buf, str, strlen(buf));
    return 0;
  }
  else
  {
    return -1;
  }
}

void createPrompt(char* username, char* hostname)
{
  snprintf(prompt,128,"[%s@%s@]-->",hostname,username);
}

// int parseinput(char* buf, char *hostname, char *username)
// {
//   int command_counter = 0;
//   int argc = 0;
//   char **args = (char**)calloc(MAX_ARGS, sizeof(*args));
//   char *token = strtok(buf, " ");
//   for(int i = 0;token != NULL;i++)
//   {
//     args[i] = token;
//     token = strtok(NULL, " ");
//     argc++;
//   }
//   args[argc] = NULL;
//   int status;
//   pid_t child = fork();
//   if(child < 0)
//   {
//     return -1;
//   }
//   else if(child == 0)
//   {
//     command_counter++;
//     execvp(args[0], args);
//     return -1;
//   }
//   while(!waitpid(child,&status,WEXITSTATUS(status)));
//   drawPrompt(username, hostname);
//   return 0;
// }

int tokenize(char* buf, char **args)
{
  int argc = 0;
  args = (char**)calloc(MAX_ARGS, sizeof(*args));
  if(args == NULL)
  {
    printf("FATAL ERROR: Malloc failed");
    return 1;
  }
  char *token = strtok(buf, " ");
  for(int i = 0;token != NULL;i++)
  {
    args[i] = token;
    token = strtok(NULL, " ");
    argc++;
  }
  args[argc] = NULL;
  return 0;
}

int runCommand(char **args, int argc)
{
  int status;
  int command_counter = 0;
  pid_t child = fork();
  if(child < 0)
  {
    return -1;
  }
  else if(child == 0)
  {
    command_counter++;
    execvp(args[0], args);
  }
  return 0;
}

__sighandler_t handlesigint()
{
  printf("\npress CTRL + D to leave instead of ctrl c \n");
  return 0;
}

int main()
{
  char **args;
  int argc;
  char hostname[HOST_NAME_MAX + 1];
  char *username = getenv("USER");
  if(gethostname(hostname, HOST_NAME_MAX) == -1)
  {
    strncpy(hostname,"fallback-da-silva", strlen(hostname));
  }
  createPrompt(hostname, username);
  do
  {
    getInput(buffer);
    tokenize(buffer, args);
    runCommand(args, argc);
  }while(1);
  return EXIT_SUCCESS;
}
