#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXLINE 2000

#define BUFSIZE 10
int buffer_index = 0;
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
Treenode *assign(char **buffer);
Treenode *expression(char **buffer);
Treenode *phrase(char **buffer);
Treenode *leaf(char **buffer);
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
	Treenode* the_tree;
	the_tree = assign(tokens);
	for (int i = 0; i < 200; i++) {
		free(tokens[i]);
	}
	free(tokens);
}

char **tokenize(char* buffer){
	char** tokens;

	tokens = (char**)malloc(200 * sizeof(char*));
	for (int i = 0; i < 200; i++) {
		tokens[i] = (char*)malloc(5);
	}


	int i, last_op, token_i, j;
	token_i = 0;
	for (i=0, last_op = 0; i < strlen(buffer); i++){
		if(buffer[i] == '=' || buffer[i] == '*'|| buffer[i]=='/' ||buffer[i]=='+' || buffer[i]=='-' || buffer[i] == '(' || buffer[i] == ')'){
			for (j=last_op; j<i; j++){
				tokens[token_i][j-last_op] = buffer[j];
			}
			tokens[token_i][j-last_op] = '\0';
			token_i++;
			tokens[token_i][0] = buffer[i];
			tokens[token_i][1] = '\0';
			last_op = i + 1;
			token_i++;
		}
	}
	//char new_string[10];
	for (i = last_op; i < strlen(buffer); i++) {
		tokens[token_i][i-last_op] = buffer[i];
	}
	tokens[token_i][i] = '\0';
	//tokens[token_i] = new_string;
	/* int p, r; */
	/* for(p=0; p<len(tokens); p++){ */
	/* 	for(r=0; r */
	/* } */
	return tokens;
}



Treenode *assign(char **buffer){
	// Assign -> variable = Expression
	Treenode *var_leaf;
	var_leaf = leaf(buffer);
	buffer_index++;
	Treenode* equalnode;
	if (!(strncmp(buffer[buffer_index], "=", 1))) {
		equalnode = new_treenode();
		equalnode->type = "op";
		equalnode->value = "=";
		equalnode->left = var_leaf;
	}
	else{
		return var_leaf;
	}
	buffer_index++;
	Treenode *rightnode;
	rightnode = expression(buffer);
	equalnode->left = var_leaf;
	equalnode->right = rightnode;
	return equalnode;
}

Treenode *expression(char **buffer){
	//Expression -> phrase | phrase + expression | phrase - expression
	Treenode *phrasenode;
	phrasenode = phrase(buffer);
	buffer_index++;
	Treenode *plusminusNode;
	if(!(strncmp(buffer[buffer_index], "+", 1)) || !strncmp(buffer[buffer_index], "-", 1)){
		plusminusNode = new_treenode();
		plusminusNode->type = "op";
		plusminusNode->value = buffer[buffer_index];
		plusminusNode->left = phrasenode;
	}
	else{
		buffer_index--;
		return phrasenode;
	}
	buffer_index++;
	Treenode *exprNode = expression(buffer);
	plusminusNode->right = exprNode;
	return plusminusNode;
}

/* Treenode phrase(char *buffer, int *ip){ */
/*   // Phrase -> leaf | leaf * phrase | leaf / phrase */
/* } */

Treenode *phrase(char **buffer) {
	Treenode *leafnode = leaf(buffer);
	buffer_index++;
	Treenode *multdivNode;
	if (!(strncmp(buffer[buffer_index], "*", 1)) || !strncmp(buffer[buffer_index], "/", 1)) {
		multdivNode = new_treenode();
		multdivNode->type = "op";
		multdivNode->value = buffer[buffer_index];
		multdivNode->left = leafnode;
	}
	else{
		buffer_index--;
		return leafnode;
	}
	buffer_index++;
	Treenode *phraseNode = phrase(buffer);
	multdivNode->right = phraseNode;



	return multdivNode;
}

Treenode *leaf(char **buffer){
	// leaf -> variable | number
	Treenode* leafnode = new_treenode();
	leafnode->value = buffer[buffer_index];
	char first_value_char = leafnode->value[0];
	switch (first_value_char){
		case 'a' ... 'z':
			leafnode->type = "var";
			break;
		case '0' ... '9':
			leafnode->type = "number";
			break;
	}
	leafnode->left = NULL;
	leafnode->right = NULL;
	return leafnode;
}

