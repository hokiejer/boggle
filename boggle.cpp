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

void compute_lettercounts(char words[66000][17], int wordcount, char lettercounts[66000][26])
{
  int i,j;
  char *word;
  int wordlength;

  for(i=0;i<wordcount;i++)
  {
    // Initialize lettercounts for this word to zero
    for(j=0;j<26;j++)
      lettercounts[i][j] = '0';

    word = words[i];
    wordlength = strlen(word);
    printf("word=%s, ",word);
    printf("wordlength=%d, ",wordlength);
    printf("lettercounts=");
    for(j=0;j<wordlength;j++)
      lettercounts[i][word[j]-'a']++;
    for(j=0;j<26;j++)
      printf("%c",lettercounts[i][j]);
    printf("\n");
  }
}

int main() 
{
  char words[66000][17]; // Word storage
  char lettercounts[66000][26]; // Letter counts per word
  int wordcount = 0; // Number of words

  // Tile information
  char tiles[16];

  // Letter counts
  int letter_counts[26];

  // Temporary variables
  int i;

  // Read in the word list
  wordcount = readwords(words);
  printf("word count = %d\n", wordcount);

  // Count each letter in each word in our word list
  compute_lettercounts(words,wordcount,lettercounts);

  // Read in the tiles
  printf("Input the 16 tiles, left to right, top to bottom\n");
  for(i=0;i<16;i++) {
    scanf("%c",&(tiles[i]));
  }

  // Generate the letter counts
  for(i=0;i<26;i++) {
    letter_counts[i] = 0;
  }
  for(i=0;i<16;i++) {
    letter_counts[tiles[i]-'a']++;
  }









  return(0);
}
