#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <readline/history.h>
#include <readline/readline.h>

#define MAX_CHARS 4096
#define MAX_ARGS 128

#define clear() printf("\033[H\033[J")

char buffer[MAX_CHARS];
char prompt[128];

const char* builtins[10] =
{
"pwd",
"cd",
"exit",  
};

typedef struct
{
  int verbose;
}bool_flags;

bool_flags flags;

int getInput(char* buf)
{
  char* str;
  str = readline(prompt);
  if(str != NULL)
  {
    add_history(str);
    strncpy(buf, str, MAX_CHARS -1);
    buf[MAX_CHARS - 1] = '\0';
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

int printVerbose(char *str, bool_flags flags)
{
  if(flags.verbose == 1)
  {
    printf("\n%s\n",str);
    return 0;
  }
  else
  {
    return 1;
  }
}

int tokenize(char* buf, char *args[MAX_ARGS])
{
  char* token = strtok(buf, " ");
  int argc = 0;
  for(int i = 0 ;token != NULL;i++)
  {
    args[i] = token;
    printf("%s\n",args[i]);
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
    printVerbose("\nerror running command\n", flags);
    return -1;
  }
  else if(child == 0)
  {
    command_counter++;
    if(execvp(args[0], args) < 0)
    {
      printf("\ncould not execute command %s",args[0]);
    }
    exit(0);
  }
  else
  {
    wait(NULL);
    return 0;  
  }
  
  return 0;
}

__sighandler_t handlesigint()
{
  printf("\npress CTRL + D to leave instead of ctrl c \n"); //TODO: use this
  return 0;
}

char* isnbuiltin(char* cmd, char** builtins,size_t size)
{
  for(int i = 0; i < size; i++)
  {
    if(strcmp(builtins[i], cmd))
    {
      return cmd;
    }
  }
  return NULL;
}

int handleBultin(char *cmd, char** builtins,char** args)
{
  char* builtin = isnbuiltin(cmd, builtins, sizeof(builtins)/sizeof(builtins[0]));  
  if(!builtin || args[0] != cmd)
  {
    return 0;
  }
  else
  {
    if(strcmp(builtin, args[0]))
    {
      chdir(args[1]);
      return 1;
    }
  }
  return 0;
}

int main(int ac, char** av)
{
  if(ac > 1)
  {
    for(int i = 0; i < ac; i++)
    {
      if(strcmp(av[i],"-v") == 0)
      {
        flags.verbose = 1;
        printf("verbose mode\n");
      }
    }
  }
  char *args[MAX_ARGS * sizeof(char*)];
  int argc;
  char hostname[HOST_NAME_MAX + 1];
  char *username = getenv("USER");
  printVerbose("test\n", flags);
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
