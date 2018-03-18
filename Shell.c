#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#define BUFSIZE 1000
#define BUFTOKSIZE 100
void init_shell()
{
	printf("\n*********************************************************\n");
	printf("\t\t\tWelcome\t\t\t");
	printf("\n*********************************************************\n");
}
// Function to take input
int readToBuffer(char *buffer)
{
    
    fgets(buffer,BUFSIZE, stdin);
    if(strlen(buffer)==0)
    	return 0;
    else
    	return 1;       
}
int getTokens(char *buffer,char **tokens)
{
	char *token;
	int num_tokens = 0;
	token = strtok(buffer," \t\n\r\a");
	while (token != NULL)
    {
        //printf("%s\n", token);
        tokens[num_tokens] = token;
        num_tokens++;
        token = strtok(NULL," \t\n\r\a");
    }
    tokens[num_tokens] = NULL;
    for(int i=0;i<num_tokens;i++)
    {
    	printf("%s\n",tokens[i]);	
    }
    
    if(num_tokens==0)
    	return 0;
    else
    	return 1;
    
}
int executeCommand(char **tokens)
{
	pid_t pid;
	int status;
	int exec_val;
	pid = fork();
	if(pid==0)
	{
		exec_val = execvp(tokens[0],tokens);
		if(exec_val==-1)
			return 0;
		else
			return 1;
	}
	else if(pid<0)
	{
		return 0;
	}
	else
	{
		while (wait(&status) != pid)       
            ;
	}
	return 1;

}
int main()
{
	char *buffer[BUFSIZE];
	char *tokens[BUFTOKSIZE];
	int read_val,split_val,exec_val;
	init_shell();
	read_val = readToBuffer(buffer);
	if(read_val)
	{
		printf("%s","\nSucessful Read\n");
	}
	else
	{
		printf("%s","\nUnsuccessful Read\n");
	}
	split_val=getTokens(buffer,tokens);
	if(split_val)
	{
		printf("%s","\nSucessful Tokenizing\n");
	}
	else
	{
		printf("%s","\nUnsuccessful Tokenizing\n");
	}
	exec_val = executeCommand(tokens);
	if(exec_val)
	{
		printf("%s","\nSucessful Execution\n");
	}
	else
	{
		printf("%s","\nUnsucessful Execution\n");
	}
	
}
