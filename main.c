#include <bits/types/siginfo_t.h>
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CHARS 4096
#define MAX_ARGS 128

char buffer[MAX_CHARS];
char prompt[128];

void getInput(char* buf)
{
  if(fgets(buf, MAX_CHARS, stdin) != NULL)
  {
    buf[strcspn(buf, "\n")] = '\0';    
    return; 
  }
  }

void drawPrompt(char* username, char* hostname)
{
  snprintf(prompt,128,"[%s@%s@]-->",hostname,username);
  printf("%s",prompt);
}

int parseinput(char* buf, char *hostname, char *username)
{
  int argc = 0;
  char **args = (char**)calloc(MAX_ARGS, sizeof(*args));
  char *token = strtok(buf, " ");
  for(int i = 0;token != NULL;i++)
  {
    args[i] = token;
    token = strtok(NULL, " ");
    argc++;
  }
  args[argc] = NULL;
  int status;
  pid_t child = fork();
  if(child < 0)
  {
    return -1;
  }
  else if(child == 0)
  {
    execvp(args[0], args);
    return -1;
  }
  while(!waitpid(child,&status,WEXITSTATUS(status)));
  drawPrompt(username, hostname);
  return 0;
}

__sighandler_t handlesigint()
{
  printf("\npress CTRL + D to leave instead of ctrl c \n");
  return 0;
}

int main()
{
  char hostname[HOST_NAME_MAX + 1];
  char *username = getenv("USER");
  if(gethostname(hostname, HOST_NAME_MAX) == -1)
  {
    strncpy(hostname,"fallback-da-silva", strlen(hostname));
  }
  drawPrompt(hostname, username);
  do
  {
    getInput(buffer);
    parseinput(buffer,hostname,username);
  }while(1);
  return EXIT_SUCCESS;
}
