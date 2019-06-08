#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 66000

// For each index in "tiles", these are the adjacent indices
char neighbors[16][9] = {
  {1,4,5,-1,-1,-1,-1,-1,-1},
  {0,2,4,5,6,-1,-1,-1,-1},
  {1,3,5,6,7,-1,-1,-1,-1},
  {2,6,7,-1,-1,-1,-1,-1,-1},
  {0,1,5,8,9,-1,-1,-1,-1},
  {0,1,2,4,6,8,9,10,-1},
  {1,2,3,5,7,9,10,11,-1},
  {2,3,6,10,11,-1,-1,-1,-1},
  {4,5,9,12,13,-1,-1,-1,-1},
  {4,5,6,8,10,12,13,14,-1},
  {5,6,7,9,11,13,14,15,-1},
  {6,7,10,14,15,-1,-1,-1,-1},
  {8,9,13,-1,-1,-1,-1,-1,-1},
  {8,9,10,12,14,-1,-1,-1,-1},
  {9,10,11,13,15,-1,-1,-1,-1},
  {10,11,14,-1,-1,-1,-1,-1,-1}
};

char letter_scores[26] = {
  1, 2, 4, 2, 1, 4, 3, 3, 1, 10, 5, 2, 4, 2, 1, 4, 10, 1, 1, 1, 2, 5, 4, 8, 3, 10
};

void logindent(int depth)
{
  int i;
  for(i=0;i<depth;i++) printf("  ");
}


void trim_qu_in_wordlist(
  char words[MAX][17], 
  int wordcount) 
{
  int index = 0;
  int i;
  char *location;

  for(i=0;i<wordcount;i++)
  {
    if (location = strstr(words[i],"qu"))
    {
      do {
        location++;
        *location = *(location+1);
      } while (*location != '\0');
    }
  }
}


void restore_qu_in_wordlist(
  char words[MAX][17], 
  int wordcount) 
{
  int index = 0;
  int i;
  char *location, *curptr;
  int length;

  for(i=0;i<wordcount;i++)
  {
    if (location = strstr(words[i],"q"))
    {
      length = (int) strlen(words[i]);
      curptr = &(words[i][length])+1;
      do {
        *curptr = *(curptr-1);
        curptr--;
      } while (curptr != location);
      *(location+1) = 'u';
    }
  }
}


void printwords(char words[MAX][17], int wordcount, int spacer)
{
  int i;

  for(i=0;i<wordcount;i++) {
    printf("%s\n",words[i]);
    if (spacer > 0)
    {
      if (i % spacer == 0)
        printf("\n");
    }
  }
  printf("\n");
}


int readwords(char words[MAX][17]) 
{
  FILE *fp;
  size_t read;
  size_t len;
  char *line = NULL;
  int index;
  int wordcount = 0;

  // Read in the word list
  fp = fopen("./wordlist.final", "r");
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

void compute_lettercounts(char words[MAX][17], int wordcount, char words_lettercounts[MAX][26])
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

void prune_wordlist_with_lettercounts(char words[MAX][17], int *wordcount, char words_lettercounts[MAX][26], char board_letter_counts[26])
{
  int index = 0;
  int i,j;
  int safe;

  for(i=0;i<(*wordcount);i++)
  {
    safe = 1;
    printf("Considering word: %s\n",words[i]);
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
    printf("\n");
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


int wordsearch(char *word,char path[16],int index,char tiles[16])
{
  int neighborindex = 0;
  char nexttry;
  int i,j;
  int repeat;
  int found;
  char priortile;
  char current_tile_index = path[index-1];
  char current_tile = tiles[current_tile_index];

  for(i=0;i<index;i++) printf("  ");
  printf("===tiles[%d], letter == %c, index == %d\n",current_tile_index,current_tile,index);

  // Check the current letter
  if (tiles[current_tile_index] != word[index-1])
  {
    logindent(index);
    printf("LETTER MISMATCH!  tile == %c  word letter == %c\n",current_tile,word[index-1]);
    return(0);
  } else {
    logindent(index);
    printf("LETTER MATCH.  STILL GOING!\n");
  }

  // Check if this tile was previously seen
  for(priortile=0;priortile<(index-1);priortile++)
  {
    logindent(index);
    printf("Checking past tiles: path[%d] == %d\n",priortile,path[priortile]);
    if (path[index] == path[priortile])
    {
      logindent(index);
      printf("This tile was previously seen!\n");
      return(0);
    }
  }
  logindent(index);
  printf("This tile was not previously seen!\n");

  // If we're out of letters, then we've found the word!
  if (index == strlen(word))
  {
    return(1);
  } else {
    logindent(index);
    printf("Not done with word.  Looking for the next letter.\n");
  }

  while ((nexttry = neighbors[current_tile_index][neighborindex++]) >= 0)
  {
    logindent(index);
    printf("Considering neighbor %d (%c)\n",nexttry,tiles[nexttry]);

    // Look for reuse of tiles
    repeat = 0;
    for(j=0;j<index;j++)
    {
      if(path[j] == nexttry)
      {
        logindent(index);
        printf("No dice.  Tile %d has already been used.\n",nexttry);
        repeat = 1;
        break;
      }
    }
    if(repeat == 0)
    {

      path[index] = nexttry;
      found = wordsearch(word,path,index+1,tiles);
      if (found)
        return(1);
    }
  }
  return(0);
}


int find_word_in_board(char *word, char *tiles)
{
  char path[16];
  int index;
  int i, j;
  int found = 0;

  printf("Looking for word in board\n");
  for(i=0;i<16;i++)
  {
    if (tiles[i] == word[0])
    {
      for(j=0;j<16;j++)
        path[j] = -1;
      path[0] = i;
      index = 1;
      found = wordsearch(word,path,index,tiles);
      if (found) 
      {
        j = 0;
        while (path[j] > 0)
          printf("%c ",tiles[path[j++]]);
        printf("\n");
        break;
      }
    }
  }
  if(found) {
    return(1);
    printf ("FOUND!\n\n");
  } else {
    printf ("NOT FOUND!\n\n");
    return(0);
  }
}


void prune_wordlist_to_found_words(
  char words[MAX][17], 
  int *wordcount, 
  char tiles[16])
{
  int index = 0;
  int i;

  for(i=0;i<(*wordcount);i++)
  {
    printboard(tiles);
    printf("words[%d] == %s\n",i,words[i]);
    if(find_word_in_board(words[i],tiles))
    {
      printf("WORD FOUND IN BOARD: %s\n",words[i]);
      strcpy(words[index++],words[i]);
    }
  }
  *wordcount = index-1;
}


static int compare_standard_boggle(const void *a, const void *b)
{
  int len_a = strlen((char *) a);
  int len_b = strlen((char *) b);
  if (len_a > len_b)
    return(1);
  else 
  {
    if (len_a == len_b)
      return(0);
    else
      return(-1);
  }
}

int score_zynga_no_bonus(char *word)
{
  char *letter = word;
  int score = 0;
  while (*letter != '\0')
    score += letter_scores[*(letter++)-'a'];
  return(score);
}


static int compare_zynga_no_bonus(const void *a, const void *b)
{
  int len_a = score_zynga_no_bonus((char *) a);
  int len_b = score_zynga_no_bonus((char *) b);
  if (len_a > len_b)
    return(1);
  else 
  {
    if (len_a == len_b)
      return(0);
    else
      return(-1);
  }
}


void sort(char words[MAX][17], int wordcount, int scheme)
{
  switch(scheme) {
    case 0:
      printf("Standard Boggle scheme.\n");
      qsort(words,wordcount,17,compare_standard_boggle); 
      break;
    case 1:
      printf("Basic Zynga scheme.\n");
      qsort(words,wordcount,17,compare_zynga_no_bonus); 
      break;
    default:
      printf("Scheme not found.\n");
  }

}


int main() 
{
  char words[MAX][17];              // Word storage
  char words_lettercounts[MAX][26]; // Letter counts per word
  int wordcount = 0;                // Number of words
  char tiles[16];                   // Tile information
  char board_letter_counts[26];     // Letter counts
  int i;                            // Temporary variable
  int scheme = 1;                   // Default to normal Boggle

  // Read in the word list
  wordcount = readwords(words);
  printf("Initial word count = %d\n", wordcount);

  // Change all "qu" to "q"
  trim_qu_in_wordlist(words, wordcount);

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
  }

  // Prune the word list given the letter counts for this board
  prune_wordlist_with_lettercounts(
    words, 
    &wordcount, 
    words_lettercounts, 
    board_letter_counts
  );
  printf("Revised word count after considering letter counts = %d\n", wordcount);

  // Find the words
  prune_wordlist_to_found_words(words,&wordcount,tiles);
  printf("Total words found = %d\n", wordcount);
  printwords(words, wordcount,0);

  // Sort the words by score
  if (scheme != 0)
    sort(words,wordcount,scheme);

  printf("\nPost Sort 1:\n");
  printwords(words, wordcount,0);

  // Change all "q" back to "qu"
  restore_qu_in_wordlist(words, wordcount);

  printf("\nPost U restoration:\n");
  printwords(words, wordcount,0);

  if (scheme == 0)
    sort(words,wordcount,scheme);

  // Print the word list
  printf("\nWord list:\n");
  printwords(words, wordcount,3);

  return(0);
}
