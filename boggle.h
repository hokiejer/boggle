#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 66000

struct Worddata {
  char word[32];
  int score;
  char path[16];
};

extern int score(Worddata *worddata, char bonuses[16], char path[16], int scheme);

