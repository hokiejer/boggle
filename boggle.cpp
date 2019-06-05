#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readwords(char words[66000][17]) {
  FILE *fp;
  size_t read;
  size_t len;
  char *line = NULL;
  int wordcount = 0;

  // Read in the word list
  fp = fopen("./words.final", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {
    //printf("Retrieved line of length %zu:\n", read);
    //printf("%s", line);
    if (read < 16) {
      strcpy(words[wordcount++],line);
    }
  }
  free(line);
  fclose(fp);
  return(wordcount);
}

int main() {
  // Word storage
  char words[66000][17];
  int wordcount = 0;

  // Tile information
  char tiles[16];

  // Letter counts
  int letter_counts[26];

  // Temporary variables
  int i;

  // Read in the word list
  wordcount = readwords(words);
  printf("word count = %d\n", wordcount);

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
