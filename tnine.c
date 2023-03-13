/*
  CSE 374 AU 22
  Noah Crouch
  11/28/22
  BRO I'M SO SORRY FOR THE SPHAGETTI CODE, the resulting code is how I got my implementation
  to be mem safe as well as get 36/36, I legit debugged for like 12 hours
*/

/* tnine is a program that drives a trie / t9 program.  This code
   will build a trie, according to trienode.  It will also run
   an interactive session where the user can retrieve words using
   t9 sequences.
   CSE374, HW5, 22wi 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"



/*
  * Function: Verify Tree
  * Behavior: Determines if trie has words or not
  * Params: Trie
  * Returns: Int code
*/
int verify_tree(trieNode* wordTrie) {
  // If the root is not null enter this branch
  if (wordTrie != NULL) {
    // Loop over the branches, if at least one is
    // non null return 0
    for (int i = 0; i < BRANCHES; i++) {
      if (wordTrie -> branches[i] != NULL) {
        return 0;
      }
    }
  }
  // if all the branches are null and or the root node
  // is null return 1
  return 1;
}

/*
  * Function: Verify Input
  * Behavior: Verifies that input has only digits and pound signs
  * Params: String pattern
  * Returns: Int code
*/
int verify_input(char* pattern) {
  // loop over the string
  for (int i = 0; i < strlen(pattern); i++) {
    // convert pattern char into int
    int digit = pattern[i] - '0';
    // if the int is not within 0 to 9 and the character
    // is not the next word char return 1
    if ((digit < 0 || digit > 9) && pattern[i] != '#') {
      return 1;
    }
  }
  // if the input is valid return 0
  return 0;
}

// run_session run the on-going decoding interaction with the
// user.  It requires a previously build wordTrie to work.

/*
 * Function: Run Session
 * Behavior: Runs interactive Trie session
 * Params: Trie root
 * Returns: Nothing
 */
void run_session(trieNode *wordTrie) {
  // initiate the container strings here.
  // Had to init here or else mem safe error
  char* last_pattern = (char*) malloc(MAXLEN);
  char* pattern = (char*) malloc(MAXLEN);
  // init last pattern length
  // had to do here as well for mem safety
  int lastlen = 0;
  // set the last_pattern strings 1st character to null
  // to ensure that it is null terminated
  last_pattern[0] = '\0';
  // Print entering message
  printf("Enter \"exit\" to quit.\n");
  // While 'True' run this loop
  while (1) {
    // Print prompt message
    printf("Enter Key Sequence (or \"#\" for next word):\n> ");
    // Scan pattern into pattern container and ensure it is null terminated
    scanf("%s\\0", pattern);
    // init pattern length variable, for some reason it would not be mem
    // safe unless here
    int patlen = strlen(pattern);
    // if the pattern matches the exit message then null terminate the pattern
    // string at the front and break the loop
    if (strcmp(pattern, "exit") == 0) {
      pattern[0] = '\0';
      break;
    }
    // if the input fails the verification test Print a valid message
    // and null terminate the pattern container string, then continue
    if (verify_input(pattern) == 1) {
      printf("Input invalid please enter a valid pattern\n");
      pattern[0] = '\0';
      continue;
    }
    // If the pattern string contains the next word character enter this block
    if (pattern[0] == '#') {
      // If the last pattern container contains a pattern copy the current pattern
      // to the last pattern then move that to the current pattern container
      if (last_pattern != NULL) {
	char temp[patlen];
	strncpy(temp, pattern, patlen);
	strncpy(pattern, last_pattern, lastlen);
	strncat(pattern, temp, patlen);
	// else print an appropiate message and null terminate the pattern container
	// string, then continue
      } else {
	printf("No previous pattern to check\n");
	pattern[0] = '\0';
	continue;
      }
      // if the pattern does not contain only next word characters null terminate
      // the last pattern container string at the front
    } else {
      last_pattern[0] = '\0';
    }
    // ensure that the pattern length is the correct value. For some reason it did not
    // like being in the if conditional block 
    patlen = strlen(pattern);
    // get the word for the associated pattern
    char* word = get_word(wordTrie, pattern);
    // If the word is not null print the word
    if (word != NULL) {
      printf("%s", word);
    }
    // copy the pattern to the last pattern container
    strncpy(last_pattern, pattern, patlen);
    // set the last pattern length to the current pattern length
    lastlen = patlen;
    // null terminate the pattern container string
    pattern[0] = '\0';
  }
  // free the containers
  free(pattern);
  free(last_pattern);
}

// Given code
int main(int argc, char **argv) {
  FILE* dictionary = NULL;
  trieNode *wordTrie = NULL;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [DICTIONARY]\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    dictionary = fopen(argv[1], "r");
    if (!dictionary) {
      fprintf(stderr, "Error: Cannot open %s\n", argv[1]);
      return EXIT_FAILURE;
    }
  }
  
  // build the trie
  wordTrie = build_tree(dictionary);

  // run interactive session
  // Verify the trie first, if it passes run the session
  // if it fails print that there are no words
  if (verify_tree(wordTrie) == 0) {
    run_session(wordTrie);
  } else {
    printf("There are no words\n");
  }
  // clean up
  // free the trie and close the dictionary
  free_tree(wordTrie);
  fclose(dictionary);
  
  return(EXIT_SUCCESS);
}





