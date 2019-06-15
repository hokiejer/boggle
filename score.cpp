#include "boggle.h"

char letter_scores[26] = {
  1, 2, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10
};

char standard_boggle_length_bonus[12] = {0,0,1,1,2,3,0,0,0,0,0,0};
char zynga_length_bonus[12] = {0,0,0,0,0,3,6,10,15,20,100,100};

int score(Worddata *worddata,int scheme)
{
  int myscore;
  int length = strlen(worddata->word);

  switch(scheme)
  {
    case 0:
      myscore = standard_boggle_length_bonus[length];
      if (strstr(worddata->word,"q"))
        myscore++;
      return myscore;
    case 1:
      myscore = score_zynga_no_bonus(worddata);
      myscore += zynga_length_bonus[length];
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

