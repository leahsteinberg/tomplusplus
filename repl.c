#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXLINE 2000

typedef struct tnode *Treeptr;
typedef struct tnode {
  char *type;
  char value;
  Treeptr left;
  Treeptr right;
} Treenode;

void parse(char *buffer);
void get_input();
Treenode assign(char *buffer, int *ip);
Treenode expression(char *buffer, int *ip);
Treenode phrase(char *buffer, int *ip);
Treenode leaf(char *buffer, int *ip);


int main(){

get_input();
}


void get_input(){
  char buffer[MAXLINE];
  while(1) {
    printf("%s", ">>> ");
   fgets(buffer, MAXLINE, stdin);
   if (!(strncmp(buffer, "quit()", 6)))
     break;
   parse(buffer);
  }


}

void parse(char *buffer) {
 // get rid of whitespace
// tokenize
int i, j;
char trimmed_buffer[MAXLINE];
for (i=0, j=0; i < strlen(buffer); i++) {
  if (!(isspace(buffer[i])))
      trimmed_buffer[j++] = buffer[i];
}
trimmed_buffer[j] = '\0';

i=0;
int *ip;
ip = &i;

Treenode the_tree;
the_tree = assign(trimmed_buffer, ip);


}

Treenode assign(char *buffer, int *ip){
// Assign -> variable = Expression
  Treenode var_leaf;
  var_leaf = leaf(buffer, ip);
  (*ip)++;
  //if (buffer[i] == '='){
    //Treenode equalNode;
    //equalNode.type = 'op';
      //equal
    //make node for = sign (how are these all connected?) <- roo
 // i++
  //(right child) node = expression(buffer, i)

 // }
 // else{
 // }
  printf("%c\n", var_leaf.value);
  return var_leaf;
   }

/* Treenode expression(char *buffer, int *i){ */
/*   //Expression -> phrase | phrase + expression | phrase - expression */
/* } */

/* Treenode phrase(char *buffer, int *ip){ */
/*   // Phrase -> leaf | leaf * phrase | leaf / phrase */
/* } */

Treenode leaf(char *buffer, int *ip){
  // leaf -> variable | number
  Treenode leafnode;
  if(isalpha(buffer[*ip])){
    leafnode.type = "var";
      }
  else if(isdigit(buffer[*ip])){
      leafnode.type = "number";
    }
    leafnode.value = buffer[*ip];
    leafnode.left = NULL;
    leafnode.right = NULL;
    return leafnode;
}

