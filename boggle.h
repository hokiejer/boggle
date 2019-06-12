#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 66000

struct Worddata {
  char word[32];
  int score;
  char path[16];
};

extern int score(Worddata *worddata);
extern int score_zynga_no_bonus(char *word);
