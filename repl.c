#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXLINE 2000

#define BUFSIZE 10
typedef struct tnode *Treeptr;
typedef struct tnode {
  char *type;
  char *value;
  Treeptr left;
  Treeptr right;
} Treenode;

Treenode* new_treenode();

Treenode* new_treenode() {
	Treenode* this = malloc(sizeof(Treenode));
	this->type = malloc(BUFSIZE);
	this->value = malloc(BUFSIZE);
	this->left = NULL;
	this->right = NULL;
	return this;
}

void del_treenode(Treenode* this) {
	free(this->type);
	free(this->value);
	// if (this->left != NULL) {
	// del_treenode(this->left);
	free(this);
}
void parse(char *buffer);
void get_input();
Treenode assign(char **buffer, int *ip);
Treenode expression(char **buffer, int *ip);
Treenode phrase(char **buffer, int *ip);
Treenode leaf(char **buffer, int ip);
char** tokenize(char *);

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

char **tokens = tokenize(trimmed_buffer);
i=0;
int *ip;
ip = &i;

Treenode the_tree;
the_tree = assign(tokens, ip);


}

char **tokenize(char* buffer){
	static char *tokens[20];
	int i, last_op, token_i, j;
	token_i = 0;
	for (i=0, last_op = 0; i < strlen(buffer); i++){
		if(buffer[i] == '='){
			char new_string[10];
			for (j=last_op; j<i; j++){
				new_string[j-last_op] = buffer[j];
				//	tokens[token_i][j-last_op] = buffer[j];
			}
			new_string[j-last_op] = '\0';
			tokens[token_i] = new_string;
			token_i++;
			char op_string[5];
			op_string[0] = buffer[i];
			op_string[1] = '\0';
			tokens[token_i] = op_string;
			last_op = i;
			token_i++;
		}
	}
	char new_string[10];
	for (i = last_op+1; i < strlen(buffer); i++) {
		new_string[i-last_op-1] = buffer[i];
	}
	new_string[i] = '\0';
	tokens[token_i] = new_string;
	/* int p, r; */
	/* for(p=0; p<len(tokens); p++){ */
	/* 	for(r=0; r */
	/* } */
	return tokens;
}



Treenode assign(char **buffer, int *ip){
// Assign -> variable = Expression
  Treenode var_leaf;
  var_leaf = leaf(buffer, *ip);
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
  //printf("%s\n", var_leaf.value);
  return var_leaf;
   }

/* Treenode expression(char *buffer, int *i){ */
/*   //Expression -> phrase | phrase + expression | phrase - expression */
/* } */

/* Treenode phrase(char *buffer, int *ip){ */
/*   // Phrase -> leaf | leaf * phrase | leaf / phrase */
/* } */

Treenode leaf(char **buffer, int ip){
  // leaf -> variable | number
  Treenode leafnode;
   leafnode.value = buffer[ip];
   char first_value_char = leafnode.value[0];
   switch (first_value_char){
		   case 'a' ... 'z':
    			leafnode.type = "var";
		   	break;
		   case '0' ... '9':
			leafnode.type = "number";
		   	break;
		   }
    leafnode.left = NULL;
    leafnode.right = NULL;
    return leafnode;
}

