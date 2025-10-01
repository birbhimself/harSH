#include <stdint.h>
#include <string.h>
#include <stdio.h>

int string_tok(char** dest, char* src, char delim)
{
  if(dest == NULL)
  {
    return -1;
  }
  uint32_t delim_index = 0;
  size_t dest_iter = 0;
  for(int i = 0; i < strlen(src);i++)
  {
    if(src[i] == delim)
    {
      char buf[strlen(src) + 1];
      int buf_iter = 0;
      for(int j = i; j > delim_index; j--, buf_iter++)
      {
        buf[j] = src[j];
        // printf("buf: %c <-> src: %c\n",buf[j],src[j]);
        printf("buf: %s <-> src: %s\n",buf,src);  
      }
      buf[strlen(buf) + 1] = '\0';
      dest[dest_iter] = buf;
      dest_iter++;
      delim_index = i;
    }
  }
}

int main(int ac, char **av)
{
  char *result[300] = {};
  char *str = "the quick brown fox jumps over the lazy dog";
  string_tok(result, str, ' ');
    for(int i = 0; i < 300; i++)
  {
    if(result[i] == NULL)
    {
      continue;
    }
    else
    {
      printf("arg nº %i: %s\n",i,result[i]);
    }
  }
}
