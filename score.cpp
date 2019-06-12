#include "boggle.h"

char letter_scores[26] = {
  1, 2, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10
};

void score(Worddata worddata)
{
}


int score_zynga_no_bonus(char *word)
{
  char *letter = word;
  int score = 0;
  while (*letter != '\0')
    score += letter_scores[*(letter++)-'a'];
  return(score);
}

