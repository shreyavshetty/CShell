#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#define WRITE_SIZE 1000
#define MAX_BUF_SIZE 255
#define TOKENS_MAX 64
#define NUM_ALIAS 71
#define MAX 50
char s[20];
// A node of ternary search tree
struct Node
{
    char data;//to store the character of a word    
    unsigned isEndOfString: 1;// True if this character is last character of one of the words 
    struct Node *left, *eq, *right;//Left pointer, Right pointer and Equality pointer
};
//function to create a new ternary search tree node
struct Node* newNode(char data);
int found;
int launch();
int initialize_alias();
int redirect(char **tokens);
void fileIO(char * args[], char* inputFile, char* outputFile, int option);
int check_pipe(char **tokens);
int redirect1(char **args);
int pipe_and_redirect(char* args[],int num_cmds);
void pipe_Handler(char *args[],int num_cmds);
char * read_line();
char ** get_tokens(char *);
int run(char **,char *, FILE *);
int check_red(char **tokens);
int execute(char **, char *,FILE *);
int cd(char **);
int echo(char **);
int help(char **);
int history(char **);
int creat_alias(char **);
char* get_alias(char *);
int texteditor();
void Create();
int Append(char *, int);
int Delete();
void Display();
void initialize_environment();
int set_environment(char **);
int rm_environment(char **);
int get_environment(char **);
int print_environment(char **);
int execute_editor();
int autocomplete();
int searchTST(struct Node *root, char *word);
void traverseTST(struct Node* root);
void insert(struct Node** root, char *word);
void traverseTSTUtil(struct Node* root, char* buffer, int depth);
struct  alias
{
	char command[50];
	char alias[1000][50];
	char options[1000][50][20];
	int ctr;
}arr[80];

struct hist
{
	int id;
	char command[100];
}hist;
#define atoa(x)
int ed_ctr;
