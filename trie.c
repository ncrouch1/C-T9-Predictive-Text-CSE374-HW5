/*
  CSE 374 AU 22
  Noah Crouch
  11/28/22
  I APPOLOGIZE FOR HOW SPAGHETTI CODE THIS LOOKS, I was tryna get that 36/36 and the resulting code is how
  I managed to get around alot of these unsafe mem writes and reads

*/


/* trie implements a trie, made of trieNodes. This includes
   code to build, search, and delete a trie
   CSE374, HW5, 22wi 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"


/* You'll want to prototype any helper functions you need here */
/* As well as the helper functions you prototype above. */


/*
 * Function: Insert Node
 * Behavior: Helper function for Build Tree
 * Params: trie root, string pattern
 * Returns: Nothing
 */
void insert_node(trieNode* root, char* pattern) {
  // Make a copy of the root
  trieNode* branch = root;
  // initiate a for loop from 0 to the max string length
  for (int i = 0; i < MAXLEN; i++) {
    // get the digit for the character at pattern index
    int digit = get_digit(pattern[i]);
    // if character is "NULL" break, set 0 to NULL for simplicity
    if (digit == 0) {
      break;
    }
    // If the branch we're examining is NULL create a new node at the memory location
    if (branch->branches[digit] == NULL) {
      branch->branches[digit] = create_node();
    }
    // After making sure the branch we're examining exists, move to that branch
    branch = branch->branches[digit];
  }
  // After we found the branch check if the word is NULL
  if (branch->word != NULL) {
    // If it isn't start inserting '#' Nodes
    int digit = get_digit('#');
    while (branch->word != NULL) {
      if (branch->branches[digit] == NULL) {
	      branch->branches[digit] = create_node();
      }
      branch = branch->branches[digit];
    }
  }
  // Once we found a space for the word initiate the memory location
  // and store the word there
  branch->word = (char*) malloc(strlen(pattern)+1);
  strncpy(branch->word, pattern, strlen(pattern)+1);
}

/*
 * Function: Build Tree
 * Behavior: Builds Trie DS
 * Params: Dictionary
 * Returns: Trie root
 */
trieNode* build_tree(FILE* dict) {
  // Initiate the line container and root
  char line[MAXLEN];
  trieNode* root = create_node();
  // While there are still lines in the file insert the words from those lines
  while (fgets(line, MAXLEN, dict) != NULL) {
    insert_node(root, line);
  }
  // return root
  return root;
}
						            

/*
   * Function: Get Num
   * Behavior: Aids in retrieving word by validating digit
   * Param: Character from pattern
   * Return: Corresponding integer
 */

int get_num(char ch) {
  // Convert character to integer
  int digit = ch - '0';
  // If the letter is '#' return 10, corresponding digit for '#'
  if (ch == '#') {
    return 10;
  }
  // If the integer is a number between 0 and 9 return the digit
  if (digit >= 0 && digit <= 9) {
    return digit;
  }
  // If the other two passes fail, return 0
  return 0;
}

/*												               
 * Function: get word										             
 * Behavior: Gets the word if it exists in the trie						          
 * Params: Trie root, word pattern								       
 * Returns: Word if it is in the trie, Null if the trie is NULL or if the word is 		      
 * not in the trie										    
 */												    
char* get_word(trieNode *root, char *pattern) {
  // Copy the root into branch, reference for root
  trieNode* branch = root;
  // initiate a flag and length variable for NULL characters and pattern length.
  // I had to use len here bc I kept getting an uninitialized variable error
  int len = strlen(pattern);
  int nullflag = 0;
  // loop over the pattern string
  for (int i = 0; i < len; i++) {
    // get the digit from the helper method
    int digit = get_num(pattern[i]);
    // if the character was "NULL" break
    if (digit == 0) {
      break;
    }
    // If the branch we're trying to move to is NULL raise the flag
    if (branch->branches[digit] == NULL) {
      nullflag = 1;
    }
    // Progress to the next branch
    branch = branch->branches[digit];
    // If the nullflag is raised break.
    // Also had to use nullflag for same reason as len variable
    if (nullflag == 1) {
      break;
    }
  }
  // If the null flag is raise or the branch word is null
  // print a message according to the last character in the
  // pattern string.

  // nullflag had to be first since we technically could exist at a
  // null node. If NULL flag == 0 then the branch is non null so
  // attempting to access the word is mem safe
  if (nullflag == 1 || branch->word == NULL) {
    if (pattern[len-1] == '#') {
      printf("There are no more T9onyms\n");
    } else {
      printf("Not found in current dictionary\n");
    }
    // return a string casted null variable
    // This is mem readable safe for the interactive function
    return (char*) NULL;
  }
  // Else return the word at the branch
  return branch->word;
}   	    

/*												    
   * Function: Free_Tree									    
   * Behavior: Recursively free the trie							    
   * Params: Root node of trie									    
   * Returns: Nothing										    
   */												    
void free_tree(trieNode *root) {
  // Loop over all the branches for the current Node
  for (int i = 0; i < BRANCHES; i++) {
    // If the branch is not null recursively free it with
    // the free_tree function
    if (root -> branches[i] != NULL) {
      free_tree(root -> branches[i]);
    }
  }
  // if the word is not null free it
  if (root -> word) {
    free(root -> word);
  }
  // free the root and exit the current call iteration
  free(root);
}												    

/*												    
 * Function: Get Digit									    
 * Behavior: Returns the mapped digit to a given letter					    
 *  If given the next character it will return 0, if given the NULL char or end of		    
 *  file/line char it returns NULL. Else it returns the number mapped to the letter		    
 * Params: Char letter									    
   * Returns: Digit										    
   */												    
int get_digit(char letter) {
  // If the letter is '#' return 10
  if (letter == '#') {										    
    return 10;										    
  }
  // Convert char to int
  int let = letter - 'a';
  // If the char was uppercase account for that
  // using asci values this conditional block
  // tests to see if its within the range
  if (let < 0 && let >= -32) {
    let = letter - 'A';
  }
  // Make a table to map the letters to the keys
  int table[26] = {2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9}; 
  // If the digit is within the valid range return the table's entry for it
  if (let >= 0 && let < 26) {
    return table[let];
  }
  // Else return 0
  return 0;											    
}												    

/*												    
   * Function: Create Node									    
   * Behavior: Creates a new TrieNode								    
   * Params: Pointer to TrieNode								    
   * Returns: A fully initialized TrieNode							    
*/												    
trieNode* create_node() {
  // Allocate memory for the parent node
  trieNode* node = (trieNode*) malloc(sizeof(trieNode));
  // Initiate the word as NULL
  node->word = NULL;
  // Loop over all the branches an initiate them as NULL
  for (int i = 0; i < BRANCHES; i++) {
    node->branches[i] = NULL;
  }
  // Return the Node
  return node;
}			

												    





















