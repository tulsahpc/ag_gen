#include <stdlib.h>
#include <string.h>
#include "util.h"

int main()
{
  char *text = "Hello, world!";
  int textLen = strlen(text);
  DEBUG_PRINT("text length: %d\n", textLen);

  char *textCopy = malloc(sizeof(char)*textLen);
  sstrcpy(textCopy, text, textLen);

  printf("%s\n", text);
  printf("%s\n", textCopy);
}
