#include "boggle.h"

char letter_scores[26] = {
  1, 2, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10
};

int score(Worddata *worddata,int scheme)
{
  int myscore;

  switch(scheme)
  {
    case 0:
      printf("Standard Boggle scheme.\n");
      myscore = strlen(worddata->word);
      if (strstr(worddata->word,"q"))
        myscore++;
      printf("Score == %d\n",worddata->score);
      return myscore;
    case 1:
      printf("Basic Zynga scheme.\n");
      myscore = score_zynga_no_bonus(worddata);
      printf("Score == %d\n",worddata->score);
      return myscore;
    default:
      printf("Scheme not found.\n");
  }
}


int score_zynga_no_bonus(Worddata *worddata)
{
  char *letter = worddata->word;
  int score = 0;
  while (*letter != '\0')
    score += letter_scores[*(letter++)-'a'];
  return(score);
}

