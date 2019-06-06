#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readwords(char words[66000][17]) 
{
  FILE *fp;
  size_t read;
  size_t len;
  char *line = NULL;
  int index;
  int wordcount = 0;

  // Read in the word list
  fp = fopen("./words.final", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    if (read < 16) {
      index = (int) strlen(line);
      if (index >= 0)
        line[index-1] = '\0';
      strcpy(words[wordcount++],line);
    }
  }
  free(line);
  fclose(fp);
  return(wordcount);
}

void compute_lettercounts(char words[66000][17], int wordcount, char words_lettercounts[66000][26])
{
  int i,j;
  char *word;
  int wordlength;

  for(i=0;i<wordcount;i++)
  {
    // Initialize lettercounts for this word to zero
    for(j=0;j<26;j++)
      words_lettercounts[i][j] = 0;

    word = words[i];
    wordlength = strlen(word);
    //printf("word=%s, ",word);
    //printf("wordlength=%d, ",wordlength);
    //printf("words_lettercounts=");
    for(j=0;j<wordlength;j++)
      words_lettercounts[i][word[j]-'a']++;
    //for(j=0;j<26;j++)
      //printf("%c",words_lettercounts[i][j]);
    //printf("\n");
  }
}

void prune_wordlist_with_lettercounts(char words[66000][17], int *wordcount, char words_lettercounts[66000][26], char board_letter_counts[26])
{
  int index = 0;
  int i,j;
  int safe;

  for(i=0;i<(*wordcount);i++)
  {
    safe = 1;
    //printf("Considering word: %s\n",words[i]);
    for(j=0;j<26;j++)
    {
      //printf("%d",j);
      //printf("%d",words_lettercounts[i][j]);
      if (words_lettercounts[i][j] > board_letter_counts[j])
      {
        safe = 0;
        break;
      }
    }
    //printf("\n");
    if (safe)
    {
      //printf("ADDING TO LIST\n");
      strcpy(words[index++],words[i]);
    }
  }
  *wordcount = index-1;
}


void printboard(char tiles[16])
{
  int i,j;

  printf("Board: \n  ");
  for(i=0;i<4;i++)
  {
    for(j=0;j<4;j++)
    {
      printf("%c ",tiles[(4*i)+j]);
    }
    printf("\n  ");
  }
  printf("\n");
}


int main() 
{
  char words[66000][17]; // Word storage
  char words_lettercounts[66000][26]; // Letter counts per word
  int wordcount = 0; // Number of words

  // Tile information
  char tiles[16];

  // Letter counts
  char board_letter_counts[26];

  // Temporary variables
  int i;

  // Read in the word list
  wordcount = readwords(words);
  printf("Initial word count = %d\n", wordcount);

  // Count each letter in each word in our word list
  compute_lettercounts(words,wordcount,words_lettercounts);

  // Read in the tiles
  printf("Input the 16 tiles, left to right, top to bottom\n");
  for(i=0;i<16;i++) {
    scanf("%c",&(tiles[i]));
  }
  printboard(tiles);

  // Generate the letter counts for this board
  for(i=0;i<26;i++) {
    board_letter_counts[i] = 0;
  }
  for(i=0;i<16;i++) {
    board_letter_counts[tiles[i]-'a']++;
    if(tiles[i] == 'q')
      board_letter_counts['u'-'a']++;
  }
  printf("Board Letter Counts: ");
  for(i=0;i<26;i++)
    printf("%d",board_letter_counts[i]);
  printf("\n");

  // Prune the word list given the letter counts for this board
  prune_wordlist_with_lettercounts(words, &wordcount, words_lettercounts, board_letter_counts);
  printf("Revised word count after considering letter counts = %d\n", wordcount);

  for(i=0;i<wordcount;i++)
    printf("words[%d] == %s\n",i,words[i]);





  return(0);
}
