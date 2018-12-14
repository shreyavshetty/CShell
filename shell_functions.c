#include "shell.h"
#define WRITE_SIZE 1000
#define MAX_BUF_SIZE 255
#define TOKENS_MAX 64
#define NUM_ALIAS 71

int initialize_alias()
{
	strcpy(arr[0].command,"clear");
	strcpy(arr[1].command,"exit");
	strcpy(arr[2].command,"alias");
	strcpy(arr[3].command,"history");
	strcpy(arr[4].command,"ls");
	strcpy(arr[5].command,"pwd");
	strcpy(arr[6].command,"cd");
	strcpy(arr[7].command,"mkdir");
	strcpy(arr[8].command,"rmdir");
	strcpy(arr[9].command,"cp");
	strcpy(arr[10].command,"mv");
	strcpy(arr[11].command,"rm");
	strcpy(arr[12].command,"touch");
	strcpy(arr[13].command,"cat");
	strcpy(arr[14].command,"more");
	strcpy(arr[15].command,"less");
	strcpy(arr[16].command,"head");
	strcpy(arr[17].command,"tail");
	strcpy(arr[18].command,"wc");
	strcpy(arr[19].command,"du");
	strcpy(arr[20].command,"diff");
	strcpy(arr[21].command,"chmod");
	strcpy(arr[22].command,"chown");
	strcpy(arr[23].command,"chgrp");
	strcpy(arr[24].command,"umask");
	strcpy(arr[25].command,"grep");
	strcpy(arr[26].command,"sort");
	strcpy(arr[27].command,"uniq");
	strcpy(arr[28].command,"find");
	strcpy(arr[29].command,"xargs");
	strcpy(arr[30].command,"locate");
	strcpy(arr[31].command,"which");
	strcpy(arr[32].command,"zip");
	strcpy(arr[33].command,"unzip");
	strcpy(arr[34].command,"tar");
	strcpy(arr[35].command,"gzip");
	strcpy(arr[36].command,"gunzip");
	strcpy(arr[37].command,"bzip2");
	strcpy(arr[38].command,"bunzip2");
	strcpy(arr[39].command,"date");
	strcpy(arr[40].command,"cal");
	strcpy(arr[41].command,"uname");
	strcpy(arr[42].command,"time");
	strcpy(arr[43].command,"ps");
	strcpy(arr[44].command,"jobs");
	strcpy(arr[45].command,"kill");
	strcpy(arr[46].command,"killall");
	strcpy(arr[47].command,"bg");
	strcpy(arr[48].command,"fg");
	strcpy(arr[49].command,"whoami");
	strcpy(arr[50].command,"passwd");
	strcpy(arr[51].command,"groups");
	strcpy(arr[52].command,"sudo");
	strcpy(arr[53].command,"su");
	strcpy(arr[54].command,"w");
	strcpy(arr[55].command,"write");
	strcpy(arr[56].command,"wall");
	strcpy(arr[57].command,"hostname");
	strcpy(arr[58].command,"ssh");
	strcpy(arr[59].command,"sftp");
	strcpy(arr[60].command,"scp");
	strcpy(arr[61].command,"wget");
	strcpy(arr[62].command,"vi");
	strcpy(arr[63].command,"gcc");
	strcpy(arr[64].command,"echo");
	strcpy(arr[65].command,"make");
	strcpy(arr[66].command,"sleep");
	strcpy(arr[67].command,"editor");
	strcpy(arr[68].command,"rmenv");
	strcpy(arr[69].command,"setenv");
	strcpy(arr[70].command,"printenv");
	strcpy(arr[71].command,"autocomplete");
}
// this is where it starts
int launch()
{
	initialize_environment();
	char *line;
	char **tokens;
	int status;
	do
	{
		FILE *fp;
		int num_pipe_cmds;
		int red_cmd;
		int val;
		fp=fopen("hist.txt","a");
		printf("$ ");
		line = read_line();
		char line1[100];
		strcpy(line1,line);
		tokens = get_tokens(line);
		num_pipe_cmds = check_pipe(tokens);
		//printf("num pipes = %d\n", num_pipe_cmds);
		red_cmd = check_red(tokens);
		//printf("TOKENS: %s",*tokens);
		if((num_pipe_cmds > 1) && (red_cmd == 1))
		{
			pipe_and_redirect(tokens,num_pipe_cmds);
		}

		else if((num_pipe_cmds > 1) && (red_cmd !=1 ))
		{
			pipe_Handler(tokens,num_pipe_cmds);
		}

		else if((red_cmd == 1) && (num_pipe_cmds <= 1) )
		{
			//printf("%s %s\n",tokens[1],tokens[2]);
		    //printf("Redirection\n");
			val = redirect1(tokens);
			//printf("val = %d", val);
		}
		else
		{
			status=execute(tokens,line1,fp);

		}
		free(line);
		free(tokens);
		fclose(fp);
	}
	while(status);
	return 1;
}
int check_red(char **tokens)
{
	// We look for the special characters and separate the command itself
	// in a new array for the arguments
	int j =0;
    while (tokens[j] != NULL)
    {
			if ( (strcmp(tokens[j],">") == 0) || (strcmp(tokens[j],"<") == 0)){
			    return 1;
				break;
			}
		j++;
	}
	return 0;
}


/*
int tokenize(char* str)
{
	char str[100];
	//scanf ("%[^\n]%*c", str);
	//printf("%s\n\n",str);
	// Returns first token
	int token_ctr=TOKENS_MAX;
	int ctr=0;
	char **token_list = (char **)malloc(MAX_BUF_SIZE*sizeof(char*));
	char *buffer = (char*)malloc(token_ctr*sizeof(char));
	buffer=strtok(str,"<");
	while(buffer!=NULL)
	{
		token_list[ctr++]=buffer;
		buffer = strtok(NULL,"<");
	}
	token_list[ctr]=NULL;
	char *infile=malloc(token_ctr*sizeof(char));
	if(token_list[1]!=NULL)
		infile=strtok(token_list[1]," ");
	char **token_list1 = (char **)malloc(MAX_BUF_SIZE*sizeof(char*));
	char *buffer1 = (char*)malloc(token_ctr*sizeof(char));
	ctr=0;
	buffer1=strtok(token_list[0],"|");
	while(buffer1!=NULL)
	{
		//printf("%s\n",buffer1);
		token_list1[ctr++]=buffer1;
		buffer1 = strtok(NULL,"|");
	}
	token_list1[ctr]=NULL;
	int ctr1=0;
	char **token_list2 = (char **)malloc(MAX_BUF_SIZE*sizeof(char*));
	char *buffer2 = (char*)malloc(token_ctr*sizeof(char));
	buffer2=strtok(token_list1[ctr-1],">");
	while(buffer2!=NULL)
	{
		//printf("%s\n",buffer2);
		token_list2[ctr1++]=buffer2;
		buffer2 = strtok(NULL,">");
	}
	char *outfile=malloc(token_ctr*sizeof(char));
	if(token_list2[1]!=NULL)
	{	outfile=strtok(token_list2[1]," ");
		//printf("%s tok2: %s\n",token_list1[ctr-1],token_list2[1]);
		token_list1[ctr-1]=token_list2[0];
	}
	if(strlen(infile)!=0)
		printf("Infile : %s\n",infile);
	int j=0;
	while(token_list1[j]!=NULL)
	{
		printf("Command %d: %s\n",j+1,token_list1[j]);
		j++;
	}
	if(strlen(outfile)!=0)
		printf("Oufile : %s\n",outfile);
	return 0;

}
*/

int redirect(char **tokens)
{
	//printf("tokens: %s",*tokens);
	char *tokens_aux[256];
	int j =0;
	int i=0;
	int aux;
	while (tokens[j] != NULL)
	{
			if ((strcmp(tokens[j],">") == 0) || (strcmp(tokens[j],"<") == 0))
			{
				break;
			}
			tokens_aux[j] = tokens[j];
			j++;
	 }
	 //printf("tokens_aux: %s\n",*tokens_aux);
	while(tokens[i] != NULL)
		{
		if (strcmp(tokens[i],"<") == 0){
				aux = i+1;
				if (tokens[aux] == NULL){
					printf("Not enough input arguments\n");
					return -1;
				}
				//printf("inp =%s out=%s\n",tokens[i+1],tokens[i-1]);
				fileIO(tokens_aux,tokens[i+1],NULL,1);
				return 1;
			}
			// If '>' is detected, we have output redirection.
			// First we check if the structure given is the correct one,
			// and if that is the case we call the appropriate method
			else if (strcmp(tokens[i],">") == 0){
				if (tokens[i+1] == NULL){
					printf("Not enough input1 arguments\n");
					return -1;
				}
				fileIO(tokens_aux,NULL,tokens[i+1],0);
				return 1;
			}
			i++;
		}
}



int redirect1(char **args)
{
	int pid = fork();
    if (pid == 0)
	{
    	int fd0,fd1,i,in=0,out=0;
    	char input[64],output[64];

    	// finds where '<' or '>' occurs and make that args[i] = NULL , to ensure that command wont't read that

   		 for(i=0;args[i]!='\0';i++)
    	{
    		if(strcmp(args[i],">")==0)
        	{
        	    args[i]=NULL;
        	    strcpy(output,args[i+1]);
        	    out=2;
       		 }
        	else if(strcmp(args[i],"<")==0)
        	{
        	    args[i]=NULL;
       		     strcpy(input,args[i+1]);
        	    in=2;
      		}
    	}
    	//if '<' char was found in string inputted by user
    	if(in)
    	{
    	    // fdo is file-descriptor
    	    int fd0;
    	    printf("Input file: %s\n", input);
     	   if ((fd0 = open(input, O_RDONLY, 0)) < 0)
     	    {
      	      perror("Couldn't open input file");
           	  exit(0);
     	   }
           // dup2() copies content of fdo in input of preceeding file
        	dup2(fd0, 0); // STDIN_FILENO here can be replaced by 0

      		  close(fd0); // necessary
    	}

   		 //if '>' char was found in string inputted by user
   		if (out)
   		{
			printf("Output file: %s\n",output);
     	   int fd1 ;
     	   if ((fd1 = creat(output , 0644)) < 0)
     	   {
      	      perror("Couldn't open the output file");
         	   exit(0);
       		}

       		 dup2(fd1, STDOUT_FILENO); // 1 here can be replaced by STDOUT_FILENO
     		close(fd1);
   		}
   		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		FILE *fp=fopen("hist.txt","a");
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,args[0]);
		fclose(fp);
   		 execvp(*args, args);
   		 perror("execvp");
   		 _exit(1);
	}


    else if((pid) < 0)
    {
        printf("fork() failed!\n");
        exit(1);
    }

    else {                                  /* for the parent:      */
    	int status;
        while (!(wait(&status) == pid)) ; // good coding to avoid race_conditions(errors)
    }

}


int check_pipe(char **tokens)
{
	int l=0;
	int num_cmds = 0;
	// First we calculate the number of commands (they are separated by '|')
	while (tokens[l] != NULL)
	{
		if (strcmp(tokens[l],"|") == 0)
		{
			num_cmds++;
		}
		l++;
	}
	num_cmds++;
	return num_cmds;

}
void fileIO(char * args[], char* inputFile, char* outputFile, int option)
{

	int err = -1;
	pid_t pid;

	int fileDescriptor; // between 0 and 19, describing the output or input file

	if((pid=fork())==-1){
		printf("Child process could not be created\n");
		return;
	}
	if(pid==0)
	{
		// Option 0: output redirection
		if (option == 0){
			// We open (create) the file truncating it at 0, for write only
			fileDescriptor = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
			// We replace de standard output with the appropriate file
			dup2(fileDescriptor, STDOUT_FILENO);
			close(fileDescriptor);
		// Option 1: input and output redirection
		}else if (option == 1){
			// We open file for read only (it's STDIN)
			fileDescriptor = open(inputFile, O_RDONLY, 0600);
			// We replace de standard input with the appropriate file
			dup2(fileDescriptor, STDIN_FILENO);
			close(fileDescriptor);
			// Same as before for the output file
			//fileDescriptor = open(outputFile, O_CREAT | O_TRUNC | O_WRONLY, 0600);
			//dup2(fileDescriptor, STDOUT_FILENO);
			//close(fileDescriptor);
		}

		//setenv("parent",getcwd(currentDirectory, 1024),1);

		if (execvp(args[0],args)==err){
			printf("err");
			kill(getpid(),SIGTERM);
		}
	}
	waitpid(pid,NULL,0);
}


int pipe_and_redirect(char* args[],int num_cmds)
{
	//printf("\n%s\n",args[0]);
	int in=0,out=0;
	char input[64],output[64];
	for(int i = 0; i<num_cmds; i++)
	{

		 for(i=0;args[i]!='\0';i++)
    	{
    		if(strcmp(args[i],">")==0)
        	{
        	    args[i]=NULL;
        	    strcpy(output,args[i+1]);
        	    out=2;
       		 }
        	else if(strcmp(args[i],"<")==0)
        	{
        	    args[i]=NULL;
       		     strcpy(input,args[i+1]);
        	    in=2;
      		}
    	}


    }

    // File descriptors
	int filedes[2]; // pos. 0 output, pos. 1 input of the pipe
	int filedes2[2];
	char *command[256];
	pid_t pid;

	int err = -1;
	int end = 0;

	// Variables used for the different loops
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;



	// Main loop of this method. For each command between '|', the
	// pipes will be configured and standard input and/or output will
	// be replaced. Then it will be executed
	while (args[j] != NULL && end != 1)
	{
		k = 0;
		// We use an auxiliary array of pointers to store the command
		// that will be executed on each iteration
		while (strcmp(args[j],"|") != 0){
			command[k] = args[j];
			j++;
			if (args[j] == NULL){
				// 'end' variable used to keep the program from entering
				// again in the loop when no more arguments are found
				end = 1;
				k++;
				break;
			}

			k++;
			//printf("k: %s %d\n",*command,k);
		}
		// Last position of the command will be NULL to indicate that
		// it is its end when we pass it to the exec function
		command[k] = NULL;
		j++;

		// Depending on whether we are in an iteration or another, we
		// will set different descriptors for the pipes inputs and
		// output. This way, a pipe will be shared between each two
		// iterations, enabling us to connect the inputs and outputs of
		// the two different commands.
		if (i % 2 != 0){
			pipe(filedes); // for odd i
		}else{
			pipe(filedes2); // for even i
		}

		pid=fork();

		if(pid==-1)
		{
			if (i != num_cmds - 1)
			{
				if (i % 2 != 0)
				{
					close(filedes[1]); // for odd i
				}
				else
				{
					close(filedes2[1]); // for even i
				}
			}
			printf("Child process could not be created\n");
			return 1;
		}
		if(pid==0)
		{
			// If we are in the first command
			if (i == 0)
			{
				//printf("%d %s\n",i, args[i]);

				if(in)
		    	{
		    	    // fdo is file-descriptor
		    	    int fd0;
		    	    printf("Input file: %s\n", input);
		     	   if ((fd0 = open(input, O_RDONLY, 0)) < 0)
		     	    {
		      	      perror("Couldn't open input file");
		           	  exit(0);
		     	   }
		           // dup2() copies content of fdo in input of preceeding file
		        	dup2(fd0, 0); // STDIN_FILENO here can be replaced by 0

		      		  close(fd0); // necessary
		    	}


				dup2(filedes2[1], STDOUT_FILENO);
			}
			// If we are in the last command, depending on whether it
			// is placed in an odd or even position, we will replace
			// the standard input for one pipe or another. The standard
			// output will be untouched because we want to see the
			// output in the terminal
			else if (i == num_cmds - 1)
			{
				//printf("%d %s\n",i, args[i]);
				if (num_cmds % 2 != 0)
				{ // for odd number of commands

					if (out)
			   		{
						printf("Output file: %s\n",output);
			     	   int fd1 ;
			     	   if ((fd1 = creat(output , 0644)) < 0)
			     	   {
			      	      perror("Couldn't open the output file");
			         	   exit(0);
			       		}

			       		 dup2(fd1, STDOUT_FILENO); // 1 here can be replaced by STDOUT_FILENO
			     		close(fd1);
			   		}

					dup2(filedes[0],STDIN_FILENO);
				}
				else
				{ // for even number of commands
					if (out)
			   		{
						printf("Output file: %s\n",output);
			     	   int fd1 ;
			     	   if ((fd1 = creat(output , 0644)) < 0)
			     	   {
			      	      perror("Couldn't open the output file");
			         	   exit(0);
			       		}

			       		 dup2(fd1, STDOUT_FILENO); // 1 here can be replaced by STDOUT_FILENO
			     		close(fd1);
			   		}

					dup2(filedes2[0],STDIN_FILENO);
				}
			// If we are in a command that is in the middle, we will
			// have to use two pipes, one for input and another for
			// output. The position is also important in order to choose
			// which file descriptor corresponds to each input/output
			}
			else
			{ // for odd i
				if (i % 2 != 0){
					dup2(filedes2[0],STDIN_FILENO);
					dup2(filedes[1],STDOUT_FILENO);
				}else{ // for even i
					dup2(filedes[0],STDIN_FILENO);
					dup2(filedes2[1],STDOUT_FILENO);
				}
			}

			if (execvp(command[0],command)==err)
			{
				kill(getpid(),SIGTERM);
			}
		}

		// CLOSING DESCRIPTORS ON PARENT
		if (i == 0){
			close(filedes2[1]);
		}
		else if (i == num_cmds - 1){
			if (num_cmds % 2 != 0){
				close(filedes[0]);
			}else{
				close(filedes2[0]);
			}
		}else{
			if (i % 2 != 0){
				close(filedes2[0]);
				close(filedes[1]);
			}else{
				close(filedes[0]);
				close(filedes2[1]);
			}
		}

		waitpid(pid,NULL,0);
		int ctr=0;
		char line[100]="";
		while(command[ctr]!=NULL)
		{
			//printf("%s\n",command[ctr] );
			strcat(line, command[ctr]);
			strcat(line, " ");
			ctr++;
		}
		//printf("%s\n",line);
		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		FILE *fp=fopen("hist.txt","a");
		if(in && i==0)
		{
			strcat(line," < ");
			strcat(line,input);
			strcat(line," ");
		}
		if(out && i==num_cmds-1)
		{
			strcat(line," > ");
			strcat(line,output);
			strcat(line," ");
		}
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s\n",val,id,time1,line);
		fclose(fp);
		i++;
	}

}


void pipe_Handler(char *args[],int num_cmds)
{
	// File descriptors
	int filedes[2]; // pos. 0 output, pos. 1 input of the pipe
	int filedes2[2];
	char *command[256];
	//printf("Args: %s\n",*args);
	pid_t pid;

	int err = -1;
	int end = 0;

	// Variables used for the different loops
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;



	// Main loop of this method. For each command between '|', the
	// pipes will be configured and standard input and/or output will
	// be replaced. Then it will be executed
	while (args[j] != NULL && end != 1)
	{
		k = 0;
		// We use an auxiliary array of pointers to store the command
		// that will be executed on each iteration
		while (strcmp(args[j],"|") != 0){
			command[k] = args[j];
			j++;
			if (args[j] == NULL){
				// 'end' variable used to keep the program from entering
				// again in the loop when no more arguments are found
				end = 1;
				k++;
				break;
			}

			k++;
			//printf("k: %s %d\n",*command,k);
		}
		// Last position of the command will be NULL to indicate that
		// it is its end when we pass it to the exec function
		command[k] = NULL;
		j++;
		//printf("command %s %d\n",*command,k);

		// Depending on whether we are in an iteration or another, we
		// will set different descriptors for the pipes inputs and
		// output. This way, a pipe will be shared between each two
		// iterations, enabling us to connect the inputs and outputs of
		// the two different commands.
		if (i % 2 != 0){
			pipe(filedes); // for odd i
		}else{
			pipe(filedes2); // for even i
		}

		pid=fork();

		if(pid==-1){
			if (i != num_cmds - 1){
				if (i % 2 != 0){
					close(filedes[1]); // for odd i
				}else{
					close(filedes2[1]); // for even i
				}
			}
			printf("Child process could not be created\n");
			return;
		}
		if(pid==0){
			// If we are in the first command
			if (i == 0){
				//printf("%d %s\n",i, args[i]);
				dup2(filedes2[1], STDOUT_FILENO);
			}
			// If we are in the last command, depending on whether it
			// is placed in an odd or even position, we will replace
			// the standard input for one pipe or another. The standard
			// output will be untouched because we want to see the
			// output in the terminal
			else if (i == num_cmds - 1){
				//printf("%d %s\n",i, args[i]);
				if (num_cmds % 2 != 0){ // for odd number of commands
					dup2(filedes[0],STDIN_FILENO);
				}else{ // for even number of commands
					dup2(filedes2[0],STDIN_FILENO);
				}
			// If we are in a command that is in the middle, we will
			// have to use two pipes, one for input and another for
			// output. The position is also important in order to choose
			// which file descriptor corresponds to each input/output
			}
			else
			{ // for odd i
				if (i % 2 != 0){
					dup2(filedes2[0],STDIN_FILENO);
					dup2(filedes[1],STDOUT_FILENO);
				}else{ // for even i
					dup2(filedes[0],STDIN_FILENO);
					dup2(filedes2[1],STDOUT_FILENO);
				}
			}
			//int ctr=0;
			if (execvp(command[0],command)==err)
			{
				kill(getpid(),SIGTERM);
			}
		}

		// CLOSING DESCRIPTORS ON PARENT
		if (i == 0){
			close(filedes2[1]);
		}
		else if (i == num_cmds - 1){
			if (num_cmds % 2 != 0){
				close(filedes[0]);
			}else{
				close(filedes2[0]);
			}
		}else{
			if (i % 2 != 0){
				close(filedes2[0]);
				close(filedes[1]);
			}else{
				close(filedes[0]);
				close(filedes2[1]);
			}
		}

		waitpid(pid,NULL,0);
		int ctr=0;
		char line[100]="";
		while(command[ctr]!=NULL)
		{
			//printf("%s\n",command[ctr] );
			strcat(line, command[ctr]);
			strcat(line, " ");
			ctr++;
		}
		//printf("%s\n",line);
		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		FILE *fp=fopen("hist.txt","a");
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s\n",val,id,time1,line);
		fclose(fp);
		i++;
	}
}

char *read_line()
{
	char *buffer = (char*)malloc(MAX_BUF_SIZE*sizeof(char));
	size_t bufsize = MAX_BUF_SIZE;
	if(getline(&buffer,&bufsize,stdin)==-1)
		printf("Error: Unable to read line\n");
	return buffer;
}

char **get_tokens(char *line)
{
	int token_ctr=TOKENS_MAX;
	int ctr=0;
	char **token_list = (char **)malloc(MAX_BUF_SIZE*sizeof(char*));
	char *buffer = (char*)malloc(token_ctr*sizeof(char));
	buffer=strtok(line," \t\n");
	while(buffer!=NULL)
	{
		token_list[ctr++]=buffer;
		buffer = strtok(NULL," \t\n");
	}
	token_list[ctr]=NULL;
	return token_list;
}

int execute(char **args, char *line, FILE *fp)
{
	if (args[0] == NULL)
	{
    	// An empty command was entered.
    	return 1;
  	}
  	char str[10];
  	strcpy(str,get_alias(args[0]));
  	if(strcmp(str,"noal")!=0)
  	{
  		char str1[100];
  		strcpy(str1,str);
  		char **args1=get_tokens(str);
  		args[0]=args1[0];
  		//printf("%s\n",args[0]);
  		int i=0;
  		while(args[i]!=NULL)
  			i++;
  		int j=1;
  		while(args1[j]!=NULL)
  		{
  			if(strlen(args1[j])<=1)
  				break;
  			args[i]=args1[j];
  			i++;
  			j++;
  		}
  		int k=0;

  		while(args[k]!=NULL)
  		{
  			if(strlen(args[k])<=1)
  			{
  				args[k]=NULL;
  				break;
  			}
  			k++;
  		}
  		//printf("\n");
  		execute(args,str1,fp);
  		fprintf(fp,"\n");
  		return 1;
  	}
  	if(strcmp(args[0],"alias")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return creat_alias(args);
  	}
  	if(strcmp(args[0],"editor")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return texteditor(args);
  	}
	if(strcmp(args[0],"autocomplete")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return autocomplete();
  	}
  	if(strcmp(args[0],"help")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return help(args);
  	}
  	if(strcmp(args[0],"echo")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return get_environment(args);
  	}
  	if(strcmp(args[0],"setenv")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return set_environment(args);
  	}
  	if(strcmp(args[0],"printenv")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return print_environment(args);
  	}
  	  	if(strcmp(args[0],"rmenv")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return rm_environment(args);
  	}
  	else if(strcmp(args[0],"cd")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
 	 	return cd(args);
 	}
 	else if(strcmp(args[0],"history")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
 	 	return history(args);
 	}
  	else if(strcmp(args[0],"exit")==0)
  	{
  		int val=getpid();
  		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",val,id,time1,line);
  		return 0;
  	}
  	else
  		return run(args,line,fp);
}

int run(char **args, char *line, FILE *fp)
{
	pid_t child,parent;
	child=fork();
	if(child==-1)
	{
		printf("Error: Could not fork the process\n");
	}
	else if(child==0)
	{
		if(execvp(args[0],args)<0)
			printf("Error: No such command\n");
		exit(0);
	}
	else
	{
		wait(NULL);
		int id=getuid();
		time_t rawtime;
		struct tm * timeinfo;
		time(&rawtime);
		timeinfo=localtime(&rawtime);
		char *time1=asctime(timeinfo);
		int len=strlen(time1);
		time1[len-1]='\0';
		//printf("Current local time and date: %s\n",asctime(timeinfo));
		fprintf(fp,"PID: %d UID: %d Time: %s Command: %s",child,id,time1,line);
		if((strcmp(args[0],"head")==0)|| (strcmp(args[0],"tail")==0))
			printf("\n");
	}
	return 1;
}

int echo(char **args)
{
	int i=1;
	while(args[i]!=NULL)
	{
		int val=strlen(args[i]);
		char temp1[val-1];
		int j;
		for(j=0;j<val;j++)
			temp1[j]=args[i][j+1];
		if(getenv(temp1)!=NULL)
		{
			char *tmp_val = getenv(temp1);
			printf("%s\n",tmp_val);
		}
		else
			printf("Could not get environment variable\n");
		i++;
	}
	return 1;
}
//Built-in function implementations.
int cd(char **args)
{
  	if(args[1] == NULL)
   		 printf("Error: No argument given to change the directory\n");
  	else
  	{
   		if(chdir(args[1]) != 0)
   	   	printf("Error: Could not change the directory\n");
  	}
  	return 1;
}

int help(char **args)
{
	printf("To use this shell, simply type in a command as you would with a normal terminal\n\n");
	printf("Built-in commands:\ncd: Change the current directory for the process\nEg:- cd abc changes the working directory to abc\n\nhelp: Provide help to the user running the shell\n\nexit: Exit the shell\n\n");
	printf("\nalias: Creates an alias of a command\n\nhistory:can be used either to display the last 25 commands typed or along with a keyword to seach for in the history of commands\n");
	printf("\neditor: Opens a basic in-built text editor\n\nUse the man command followed for information on the other commands\n\n");
  	return 1;
}

int history(char **args)
{
	if(args[1]==NULL)
	{
		char line[100]="tail -25 hist.txt";
		char **tokens=get_tokens(line);
		int x=fork();
		if(x==0)
		{
			execvp(tokens[0],tokens);
			exit(0);
		}
		else
		{
			wait(NULL);
			printf("\n");
			return 1;
		}
	}
	else
	{
		int pipefd[2];
    	pid_t p1, p2;
    	if (pipe(pipefd) < 0)
    	{
       		return 1;
    	}
    	p1 = fork();
    	if (p1 < 0)
    	{
       		return 1;
    	}
    	if (p1 == 0)
     	{
        	close(pipefd[0]);
        	dup2(pipefd[1], STDOUT_FILENO);
        	close(pipefd[1]);
        	char line[100]="cat hist.txt";
			char **tokens=get_tokens(line);
        	if (execvp(tokens[0], tokens) < 0)
        	{
           		exit(0);
        	}
        	//sleep(2000);
        	exit(0);
    	}
    	else
    	{
       	 // Parent executing
       	 	p2 = fork();
	     	if (p2 < 0)
     	   	{
       	    	 return 1;
       		}
        	if (p2 == 0)
        	{
        		char str[100]="grep ";
        		int len=strlen(str);
        		int i=0;
        		while(args[i]!='\0')
        		{
        			str[len]=args[1][i];
        			len++;
        			i++;
        		}
        		str[len]='\0';
            	close(pipefd[1]);
            	dup2(pipefd[0], STDIN_FILENO);
            	close(pipefd[0]);
            	char **tokens=get_tokens(str);
            	if (execvp(tokens[0], tokens) < 0)
            	{
               		 exit(0);
            	}
            	exit(0);
        	}
        	else
        	{// parent executing, waiting for two children
        		wait(NULL);
        		sleep(1);
           		return 1;
        	}
    	}
	}
}
int creat_alias(char **args)
{
	if(args[1] == NULL || args[2]==NULL)
    {
    	printf("Error: No argument given for the command or it's alias\n");
		return 1;
	}
    int flag=0,arg=-1;
    for(int i=0;i<NUM_ALIAS;i++)
    {
    	//printf("%s\n",arr[i].command);
    	if(strcmp(arr[i].command,args[2])==0)
    	{
    		flag=1;
    		arg=i;
    		break;
    	}
    }
    if(flag==0)
    {
    	printf("Not a valid command : %s\n",args[2]);
    }
    else
    {
    	for(int i=0;i<NUM_ALIAS;i++)
		{
			for(int j=0;j<arr[i].ctr;j++)
			{
				if(strcmp(args[1],arr[i].alias[j])==0)
				{
					printf("Alias already exists\n");
					return 1;
				}
			}
		}
    	for(int i=0;i<NUM_ALIAS;i++)
    	{
    		if(strcmp(arr[i].command,args[1])==0)
    		{
    			printf("Cannot name the alias the same name as an existing command\n");
    			return 1;
    		}
    	}
    	int val=arr[arg].ctr;
    	strcpy(arr[arg].alias[val],args[1]);
    	int k=3;
    	while(args[k]!=NULL)
    	{
    		strcpy(arr[arg].options[val][k-3],args[k]);
    		k++;
    	}
    	int j=0;
    	arr[arg].ctr=val+1;
    }
    return 1;
}

char* get_alias(char *arg)
{
	char *command=malloc(sizeof(char)*100);
	for(int i=0;i<NUM_ALIAS;i++)
	{
		for(int j=0;j<arr[i].ctr;j++)
		{
			if(strcmp(arg,arr[i].alias[j])==0)
			{
				strcpy(command,arr[i].command);
				int k=0;
				while(arr[i].options[j][k][0]!='\0')
				{
					strcat(command," ");
					strcat(command,arr[i].options[j][k]);
					k++;
				}
				//printf("%s\n",command);
				return command;
			}
		}
	}
	return "noal";
}


int texteditor()
{
	int flag=0;
	do
	{
  		//clrscr();
  		printf("\n***** TEXT EDITOR *****");
  		printf("\n\nMENU:\n-----\n ");
  		printf("\n1.CREATE\n2.DISPLAY\n3.APPEND\n4.DELETE\n5.EXECUTE\n6.EXIT\n");
  		printf("\nEnter your choice: ");
  		int ec;
  		scanf("%d",&ec);
  		switch(ec)
  		{
   			case 1:
     			Create();
    			break;
   			case 2:
     			Display();
    			break;
  			case 3:
    			Append(NULL,1);
     			break;
   			case 4:
     			Delete();
    			break;
    		case 5:
    			execute_editor();
    			break;
   			case 6:
     			flag=1;
  			default:
    			continue;
  		}
	}while(flag==0);
 	return 1;
}

void Create()
{
	char c;
  	FILE *fp1;
 	fp1=fopen("temp.txt","w");
 	char fn[50];
 	printf("\nEnter then new filename: ");
  	scanf("%s",fn);
   	FILE* fp2=fopen(fn,"w");
   	fclose(fp2);
   	Append(fn,0);
}
void Display()
{
	char ch;
	char fn[50];
	printf("\nEnter the file name: ");
	scanf("%s",fn);
	FILE *fp1;
	fp1=fopen(fn,"r");
	if(fp1==NULL)
  	{
   		printf("\nFile not found!");
  	}
  	while(1)
  	{
   		 ch=fgetc(fp1);
    	if(ch==EOF)
        	break;
    	printf("%c",ch);
  	}
  	printf("\n");
}
int Delete()
{
	char fn[50];
  	printf("\nEnter the file name: ");
  	scanf("%s",fn);
	FILE *fp1;
  	fp1=fopen(fn,"r");
  	if(fp1==NULL)
  	{
   		printf("\nFile not found!");
   		return 1;
  	}
  	fclose(fp1);
  	if(remove(fn)==0)
  	{
  		printf("\n\nFile has been deleted successfully!");
   		return 1;
  	}
  	else
   		printf("\n\tError!\n");
}
int Append(char *fn,int ctr)
{
	FILE *f;
	char f1[50];
	if(ctr==1)
  	{
  		printf("\nEnter the file name: ");
  		scanf("%s",f1);
  		f= fopen(f1, "a+");
  	}
  	else
  	{
 		f = fopen(fn, "a+");
 	}
 	if(!f)
 	{
  		printf("File could not be created/opened\n");
  		return 0;
 	}
 	//FIND SIZE OF FILE
 	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
 	fseek(f, 0, SEEK_SET); //same as rewind(f);
	//READ ENTIRE FILE
 	char *string = malloc(fsize + 1);
 	int read_ret=fread(string, fsize, 1, f);
 	if(read_ret<0)
 	{
  		printf("File Empty or could not read\n");
  		return 0;
 	}
 	string[fsize] = '\0';
 	//PRINT FILENAME
 	printf("Either press Ctrl+D 3 times or Enter key followed by Ctrl+D to stop appending\n");
 	if(ctr==0)
 		printf("\n--------------------------\n\tFILE : %s\t\n--------------------------\n",fn);
 	else
 		printf("\n--------------------------\n\tFILE : %s\t\n--------------------------\n",f1);
 	//SPLIT STRING INTO LINES
  	int i=1;
  	if(strlen(string)!=0)
  	{
     	printf("%s\n",string);
 	}
 	//ALLOW USER TO WRITE UNTIL CtrlD IS PRESSED
 	char * new_string=malloc(sizeof(char)*WRITE_SIZE);
 	int write_ret;
	for (;;)
	{
		fseek(f,0,SEEK_END);
    	fflush( NULL );
     	//GET NEW STRING
     	if (fgets(new_string,WRITE_SIZE,stdin) == NULL)
     	{
     		break;
     	}
     	if(new_string[0]=='\n' || new_string[strlen(new_string)-1]=='\n')
     	{
     	}
    	 //WRITE NEW STRING
  		int len=strlen(new_string);
  		fputs(new_string,f);
 	}
 	fclose(f);
	return 1;
}
int execute_editor()
{
	char name[100];
	printf("Enter the C program to be compiled:\n");
	scanf("%s",name);
	char str[100];
	sprintf(str,"%d",ed_ctr);
	//printf("%s\n",str);
	ed_ctr++;
	char options[50]="ed";
	strcat(options,str);
	char com[100]="gcc ";
	strcat(com,name);
	strcat(com," -o ");
	strcat(com,options);
	//printf("%s\n",com);
	char **tokens=get_tokens(com);
	int x=fork();
    if(x==0)
    {
    	execvp(tokens[0],tokens);
    }
    else
    {
    	int status;
    	waitpid(x,&status,0);
    	if(status!=0)
    	{
    		//printf("Status: %d\n",status);
    		printf("\nCOMPILATION ERROR, CANNOT EXECUTE, RETURNING TO EDITOR MENU\n");
    		return 1;
    	}
    	else
    	{
    		int ch;
    		printf("Enter 1 to execute the program,anything to go back to editor menu\n");
    		scanf("%d",&ch);
    		if(ch==1)
    		{
    			printf("\nOutput of C program:\n");
    			char executable[20]="./";
    			strcat(executable,options);
    			char **tok=get_tokens(executable);
    			int y=fork();
    			if(y==0)
    			{
    				//printf("%s\n",executable);
    				execvp(tok[0],tok);
    			}
    			else
    			{
    				wait(NULL);
    				return 1;
    			}
    		}
    		else
    			return 1;
    	}
    	return 1;
    }
}
int set_environment(char **args)
{
	//printf("%s %s\n",args[1],args[2]);
    int res = setenv(args[1],args[2],1);
    if(res != 0)
      printf("Error: could not set variable %s to value %s\n",args[1],args[2]);
    return 1;
}


int rm_environment(char **args)
{

    int res = unsetenv(args[1]);
    if(res != 0)
      printf("Error: could not remove variable %s\n",args[1]);
    return 1;
}


int get_environment(char **args)
{

	int j=1;
	while(args[j]!=NULL)
	{

		char *res =  getenv(args[j]);
  		printf("%s = %s\n",args[j],res);
  		j++;
  	}
  	return 1;
}

int print_environment(char **args)
{
	extern char **environ;
	int j=0;
	while(environ[j]!=NULL)
	{
  		printf("%s \n",environ[j]);
  		j++;
  	}
  	return 1;
}
void initialize_environment()
{
	setenv("SHELL","/home/hduser/CShell",1);
}
// Function to insert words from dictionary into a Ternary Search Tree
void insert(struct Node** root, char *word)
{
    // Base Case: Tree is empty
    if (!(*root))
        *root = newNode(*word);//set the root to new word

    // If current character of word is smaller than root's character,
    // then insert this word in left subtree of root
    if ((*word) < (*root)->data)
        insert(&( (*root)->left ), word);

    // If current character of word is greate than root's character,
    // then insert this word in right subtree of root
    else if ((*word) > (*root)->data)
        insert(&( (*root)->right ), word);

    // If current character of word is same as root's character,
    else
    {
        //if it's not the last character of the word
        if (*(word+1))
            insert(&( (*root)->eq ), word+1);//then root becomes root->eq and the next character is sent to the insert function

        // the last character of the word
        else
            (*root)->isEndOfString = 1;//then set the endofstring to 1-->to indicate the string is complete
    }
}
struct Node* newNode(char data)
{
    struct Node* temp = (struct Node*) malloc(sizeof( struct Node ));//dynamically allocate memory
    //initial setting of new node
    temp->data = data;
    temp->isEndOfString = 0;
    temp->left = temp->eq = temp->right = NULL;
    return temp;
};
// A recursive function to traverse Ternary Search Tree
void traverseTSTUtil(struct Node* root, char* buffer, int depth)
{
    if (root)
    {
        // First traverse the left subtree
        traverseTSTUtil(root->left, buffer, depth);

        // Store the character of this node
        buffer[depth] = root->data;
        if (root->isEndOfString)
        {
            buffer[depth+1] = '\0';
	    			printf("%s",s);
            printf( "%s\n",buffer);
	    found = 1;


        }

        // Traverse the subtree using equal pointer (middle subtree)
        traverseTSTUtil(root->eq, buffer, depth + 1);

        // Finally Traverse the right subtree
        traverseTSTUtil(root->right, buffer, depth);
    }
}
// The main function to traverse a Ternary Search Tree.
// It mainly uses traverseTSTUtil()
void traverseTST(struct Node* root)
{
    char buffer[MAX];
    traverseTSTUtil(root, buffer, 0);
}
// Function to search a given word in TST
int searchTST(struct Node *root, char *word)
{
    //if root is empty then return 0
    if (!root)
        return 0;
 /*If current character of word is smaller than root's character,
 then search for this word in left subtree of root*/
    if (*word < (root)->data)
        return searchTST(root->left, word);
 /*If current character of word is greater than root's character,
 then search for this word in left subtree of root*/
    else if (*word > (root)->data)
        return searchTST(root->right, word);
 /*If current character of word is equal to root's character,
 then search for this word in the equality subtree of root*/
    else
    {
        if (*(word+1) == '\0')//if last character of the word
          {
	    traverseTST(root->right);//then traverse the right sub-tree
	    traverseTST(root->left);//then traverse the left sub-tree
	    traverseTST(root->eq);//then traverse the equality sub-tree

          }

        return searchTST(root->eq, word+1);/*call searchTST function till its the last charcter of the word*/
    }
}

int autocomplete()
{
    struct Node *root = NULL;//root set to NULL
    FILE* file = fopen("unix_commands.txt", "r"); //Open the txt file
    char line[256];
    while (fgets(line, sizeof(line), file))
    {//read each line from the text file
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
	int len=strlen(line);
	if(line[len-1]=='\n')
	{
		line[len-1]=0;
	}

         insert(&root,line);//start inserting each word from the text file
    }
    fclose(file);//close the file here
    while(1)
    {
	printf("*********************************************\n");
	printf("1.Enter text\n");
	printf("2.Exit\n");
	char c[10];
        found = 0;
	scanf("%s",c);
	if(strcmp(c,"1")==0)
		{
			printf("Enter:\n");
			scanf("%s",&s);
			searchTST(root, s);
			(found)? printf("\nFound\n"): printf("\nNot Found\n");
		}
	else if(strcmp(c,"2")==0)
		{
			printf("*********************************************\n");
			return(1);
		}
	else
		{
			printf("Invalid Option\n");
		}
    }

}
