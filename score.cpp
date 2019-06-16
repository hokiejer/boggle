#include "boggle.h"

char letter_scores[26] = {
  1, 2, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10
};

char standard_boggle_length_bonus[12] = {0,0,1,1,2,3,0,0,0,0,0,0};
char zynga_length_bonus[12] = {0,0,0,0,0,3,6,10,15,20,100,100};


int score_zynga_no_bonus(Worddata *worddata,char path[16],char bonuses[16])
{
  char *letter;
  int score = 0;
  int i;
  int multiplier = 1;
  
  for (i=0;i<strlen(worddata->word);i++)
  {
    letter = &(worddata->word[i]);
    score += letter_scores[*(letter)-'a'];
    printf("bonuses[%d] == %c\n",path[i],bonuses[path[i]]);
    switch(bonuses[path[i]])
    {
      // Double letter score
      case 'b':
        score += letter_scores[*(letter)-'a'];
        break;
      // Triple letter score
      case 'g':
        score += (2 * letter_scores[*(letter)-'a']);
        break;
      // Double word score
      case 'r':
        multiplier *= 2;
        break;
      // Triple word score
      case 'y':
        multiplier *= 3;
        break;
      default:
        break;
    }
  }
  score *= multiplier;
  score += zynga_length_bonus[strlen(worddata->word)];
  return(score);
}


int score(Worddata *worddata,char path[16],char bonuses[16],int scheme)
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
      myscore = score_zynga_no_bonus(worddata,path,bonuses);
      return myscore;
    default:
      printf("Scheme not found.\n");
  }
}


