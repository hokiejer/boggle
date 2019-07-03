#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100000

struct Worddata {
  char word[16];
  int score;
  char path[16];
};

extern int score(Worddata *worddata, char bonuses[16], char path[16], int scheme);

