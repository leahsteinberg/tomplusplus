#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXLINE 2000
#define MEM_SIZE 8*1024

#define BUFSIZE 10
#define NUM_SYMBOLS 2
int buffer_index = 0;
void *heap_b;
void *heap_e;
typedef struct tnode *Treeptr;


typedef struct tnode {
	char *type;
	char *value;
	Treeptr left;
	Treeptr right;
} Treenode;

typedef struct queueNode *Qptr;

typedef struct queueNode
{
	char *type;
	char *value;
	Qptr next;
	int *level;
} QNode;

typedef struct symbol *sym;

typedef struct symbol
{
	char *type;
	char *value;
	int size;
	int used;
	void *pointer;
} Symbols;

sym symbol_table;

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
char * evaluate(char *left, char *nodeVal, char *right);
char *traverse(Treenode *node);

void setup_mem();

int main(){

	setup_mem();
	get_input();
}

void setup_mem(){
	heap_b = (void *)malloc(MEM_SIZE);
	heap_e = (heap_b + MEM_SIZE);
	symbol_table = (sym) malloc(NUM_SYMBOLS*sizeof(Symbols));
	/* for(int i = 0; i< NUM_SYMBOLS; i++){ */
	/* 	symbol_table[i].type = NULL; */
	/* 	symbol_table[i].value = "free"; */
	/* 	symbol_table[i].size = MEM_SIZE/2; */
	/* 	symbol_table[i].used = 0; */
	/* 	symbol_table[i].pointer = heap_b+(i*(MEM_SIZE/NUM_SYMBOLS)); */

	/* } */
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
	// eokenize
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
	char *treechar = traverse(the_tree);
	printf("heres tree char: %s\n", treechar);
	free(treechar);
	for (int i = 0; i < 200; i++) {
		free(tokens[i]);
	}
	free(tokens);
	buffer_index = 0;
}

char **tokenize(char* buffer){
	char** tokens;

	tokens = (char**)malloc(200 * sizeof(char*));
	for (int k = 0; k < 200; k++) {
		tokens[k] = (char*)malloc(5);
	}


	int i, last_op, token_i, j;
	token_i = 0;
	/* for (i=0, last_op = 0; i < strlen(buffer); i++){ */
	/* 	if(buffer[i] == '=' || buffer[i] == '*'|| buffer[i]=='/' ||buffer[i]=='+' || buffer[i]=='-' || buffer[i] == '(' || buffer[i] == ')'){ */
	/* 		if (i == 0 || (isalpha(buffer[i]) || isdigit(buffer[i]))) { */
	/* 		for (j=last_op; j<i; j++){ */
	/* 			tokens[token_i][j-last_op] = buffer[j]; */
	/* 		} */
	/* 		tokens[token_i][j-last_op] = '\0'; */
	/* 		token_i++; */
	/* 		} */
	/* 		tokens[token_i][0] = buffer[i]; */
	/* 		tokens[token_i][1] = '\0'; */
	/* 		last_op = i + 1; */
	/* 		token_i++; */
	/* 	} */
	/* } */

	for (i=0, j = 0; i < strlen(buffer); i++) {
		if(isalpha(buffer[i]) || isdigit(buffer[i])){
			while (isalpha(buffer[i]) || isdigit(buffer[i])) {
				tokens[token_i][j++] = buffer[i++];
			}
			--i;
		}
		else{
			tokens[token_i][j++] = buffer[i];
		}
		tokens[token_i][j] = '\0';
		j=0;
		token_i++;
	}


	//char new_string[10];
	/* for (i = last_op; i < strlen(buffer); i++) { */
	/* 	tokens[token_i][i-last_op] = buffer[i]; */
	/* } */
	/* tokens[token_i][i] = '\0'; */
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
	if(!(strncmp(buffer[buffer_index], "+", 1))|| !strncmp(buffer[buffer_index], "-", 1)){
	while(!(strncmp(buffer[buffer_index], "+", 1)) || !strncmp(buffer[buffer_index], "-", 1)){
		plusminusNode = new_treenode();
		plusminusNode->type = "op";
		plusminusNode->value = buffer[buffer_index];
		plusminusNode->left = phrasenode;
		buffer_index++;
		Treenode *secondLeafNode = phrase(buffer);
		plusminusNode->right = secondLeafNode;
		phrasenode = plusminusNode;
		buffer_index++;
	}
	}
	else{
		buffer_index--;
		return phrasenode;
	}
	buffer_index--;
	/* Treenode *exprNode = expression(buffer); */
	/* plusminusNode->right = exprNode; */
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
	while (!(strncmp(buffer[buffer_index], "*", 1)) || !strncmp(buffer[buffer_index], "/", 1)) {
		multdivNode = new_treenode();
		multdivNode->value = buffer[buffer_index];
		multdivNode->type = "op";
		buffer_index++;
		Treenode *secondLeaf = leaf(buffer);
		multdivNode->left = leafnode;
		multdivNode->right = secondLeaf;
		buffer_index++;
		leafnode = multdivNode;
		/* multiNode->left = phreaseNode; */
	}
	}
	else{
		buffer_index--;
		return leafnode;
	}
	buffer_index--;
	/* multdivNode->right = phraseNode; */
	return multdivNode;
}

Treenode *leaf(char **buffer){

	// leaf -> variable | number
	if(buffer[buffer_index][0] == '('){
		buffer_index++;
		Treenode *exprNode = expression(buffer);
		buffer_index++;
		if(buffer[buffer_index][0] == ')'){
			//buffer_index++; // i think??
			return exprNode;
		}
		else{
			printf("%s", "didnt close parentheses!!!!\n");
		}
	}
	Treenode* leafnode = new_treenode();
	leafnode->value = buffer[buffer_index];
	switch (leafnode->value[0]){
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



char* traverse(Treenode *node){
	char *left, *right;
	if(node->left == 0x0 && node->right == 0x0){
		return node->value;
	}
	left = traverse(node->left);
	right = traverse(node->right);
	char *value = evaluate(left, node->value, right);
	return value;

}
	/* return node->value; */
	/* } */
	/* 	left = traverse(node->left); */
	/* 	right = traverse(node->right); */
	/* 	return evaluate(left, node->value, right); */
	///printf("%s\n", node->value);


char *evaluate(char *left, char *nodeVal, char *right){
	int returnVal;
	char *returnChar = malloc(10);
	switch(*nodeVal){
		case '=':
			returnVal = atoi(right);
			break;
		case '+':
			returnVal = atoi(left) + atoi(right);
			break;
		case '-':
			returnVal = atoi(left) - atoi(right);
			break;
		case '*':
			returnVal = atoi(left) * atoi(right);
			break;
		case '/':
			returnVal = atoi(left) / atoi(right);
			break;
	}
	sprintf(returnChar, "%d", returnVal);
	return returnChar;
}


/* void prettyPrint(Treenode *root) { */
	


/* } */

/* QNode *make_q_Node(){ */
/* 	QNode *this = malloc(sizeof(QNode)); */
/* 	this->type = malloc(BUFSIZE); */
/* 	this->value = malloc(BUFSIZE); */
/* 	this->next = NULL; */
/* 	this->level = malloc(sizeof(int)); */
/* 	return this; */
/* } */



/* void enqueue(Treenode *node){ */
/* 	Qptr newQueue = make_q_Node(); */
/* 	newQueue->type = node->type; */
/* 	newQueue->value = node->value; */


/* } */


