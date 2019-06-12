#include "boggle.h"

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


void logindent(int depth)
{
  int i;
  for(i=0;i<depth;i++) printf("  ");
}


void copypath(char target[], char source[])
{
  int i;
  for(i=0;i<16;i++) 
    target[i] = source[i];
}


void trim_qu_in_wordlist(
  Worddata worddata[MAX], 
  int wordcount) 
{
  int index = 0;
  int i;
  char *location;

  for(i=0;i<wordcount;i++)
  {
    if (location = strstr(worddata[i].word,"qu"))
    {
      do {
        location++;
        *location = *(location+1);
      } while (*location != '\0');
    }
  }
}


void restore_qu_in_wordlist(
  Worddata worddata[MAX], 
  int wordcount) 
{
  int index = 0;
  int i;
  char *location, *curptr;
  int length;

  for(i=0;i<wordcount;i++)
  {
    if (location = strstr(worddata[i].word,"q"))
    {
      length = (int) strlen(worddata[i].word);
      curptr = &(worddata[i].word[length])+1;
      do {
        *curptr = *(curptr-1);
        curptr--;
      } while (curptr != location);
      *(location+1) = 'u';
    }
  }
}


void printwords(Worddata worddata[MAX], int wordcount, int spacer)
{
  int i;

  for(i=0;i<wordcount;i++) {
    printf("%s  (%d)\n",worddata[i].word,worddata[i].score);
    if (spacer > 0)
    {
      if (i % spacer == 0)
        printf("\n");
    }
  }
  printf("\n");
}


int readwords(Worddata worddata[MAX]) 
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
      strcpy(worddata[wordcount++].word,line);
    }
  }
  free(line);
  fclose(fp);
  return(wordcount);
}

void compute_lettercounts(Worddata worddata[MAX], int wordcount, char words_lettercounts[MAX][26])
{
  int i,j;
  char *word;
  int wordlength;

  for(i=0;i<wordcount;i++)
  {
    // Initialize lettercounts for this word to zero
    for(j=0;j<26;j++)
      words_lettercounts[i][j] = 0;

    word = worddata[i].word;
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

void prune_wordlist_with_lettercounts(Worddata worddata[MAX], int *wordcount, char words_lettercounts[MAX][26], char board_letter_counts[26])
{
  int index = 0;
  int i,j;
  int safe;

  for(i=0;i<(*wordcount);i++)
  {
    safe = 1;
    //printf("Considering word: %s\n",worddata[i].word);
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
      strcpy(worddata[index++].word,worddata[i].word);
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


int wordsearch(Worddata *worddata,char path[16],int index,char tiles[16],int scheme)
{
  char *word = worddata->word;
  int neighborindex = 0;
  char nexttry;
  int i,j;
  int repeat;
  int found;
  int bestscore = 0;
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

    // If no tile was reused
    if(repeat == 0)
    {
      path[index] = nexttry;
      found = wordsearch(worddata,path,index+1,tiles,scheme);
      if (found > bestscore)
        bestscore = found;
      // We do not break here in case a better option is present
    }
  }
  return(bestscore);
}


int find_word_in_board(Worddata *worddata, char *tiles, int scheme)
{
  char path[16];
  int i, j;
  int found = 0;

  printf("Looking for word in board\n");
  for(i=0;i<16;i++)
  {
    if (tiles[i] == worddata->word[0])
    {
      for(j=0;j<16;j++)
        path[j] = -1;
      path[0] = i;
      found = wordsearch(worddata,path,1,tiles,scheme);
      if (found) 
      {
        j = 0;
        while (path[j] >= 0)
          printf("%c ",tiles[path[j++]]);
        printf("\n");

        worddata->score = score(worddata,scheme);
        break;
      }
    }
  }
  if(found) {
    printf ("FOUND!\n\n");
    return(1);
  } else {
    printf ("NOT FOUND!\n\n");
    return(0);
  }
}


void prune_wordlist_to_found_words(
  Worddata worddata[MAX], 
  int *wordcount, 
  char tiles[16],
  int scheme)
{
  int index = 0;
  int i;

  for(i=0;i<(*wordcount);i++)
  {
    printboard(tiles);
    printf("worddata[%d].word == %s\n",i,worddata[i].word);
    if(find_word_in_board(&(worddata[i]),tiles,scheme))
    {
      printf("WORD FOUND IN BOARD: %s (%d)\n",worddata[i].word,worddata[i].score);
      strcpy(worddata[index].word,worddata[i].word);
      worddata[index++].score = worddata[i].score;
    }
  }
  *wordcount = index-1;
}


static int compare_scores(const void *a, const void *b)
{
  int score_a = ((Worddata *) a)->score;
  int score_b = ((Worddata *) b)->score;
  if (score_a > score_b)
    return(1);
  else 
  {
    if (score_a == score_b)
      return(0);
    else
      return(-1);
  }
}

void sort(Worddata worddata[MAX], int wordcount)
{
  qsort(worddata,wordcount,sizeof(Worddata),compare_scores); 
}


int main() 
{
  Worddata worddata[MAX];              // Word storage
  char words_lettercounts[MAX][26]; // Letter counts per word
  int wordcount = 0;                // Number of words
  char tiles[16];                   // Tile information
  char board_letter_counts[26];     // Letter counts
  int i;                            // Temporary variable
  int scheme = 1;                   // Default to normal Boggle

  // Read in the word list
  wordcount = readwords(worddata);
  printf("Initial word count = %d\n", wordcount);

  // Change all "qu" to "q"
  trim_qu_in_wordlist(worddata, wordcount);

  // Count each letter in each word in our word list
  compute_lettercounts(worddata,wordcount,words_lettercounts);

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
    worddata, 
    &wordcount, 
    words_lettercounts, 
    board_letter_counts
  );
  printf("Revised word count after considering letter counts = %d\n", wordcount);

  // Find the words
  prune_wordlist_to_found_words(worddata,&wordcount,tiles,scheme);
  printf("Total words found = %d\n", wordcount);
  printwords(worddata, wordcount,0);

  // Sort the words by score
  sort(worddata,wordcount);

  printf("\nPost Sort 1:\n");
  printwords(worddata, wordcount,0);

  // Change all "q" back to "qu"
  restore_qu_in_wordlist(worddata, wordcount);

  printf("\nPost U restoration:\n");
  printwords(worddata, wordcount,0);

  // Print the word list
  printf("\nWord list:\n");
  printwords(worddata, wordcount,3);

  return(0);
}
