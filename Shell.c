#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define GetCurrentDir getcwd
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <signal.h>
FILE *logfile;
time_t current_time ;
void start();
void signalhandler();
void signalhandler()
{
    printf("Child process was terminated");
    struct tm *tm = localtime(&current_time);
    char s[100];
    strftime(s, sizeof(s), "%c", tm);
    fprintf(logfile,"%s: Child process was terminated\n",s);
}
int main()
{
	while(1)
	{
		current_time = time(NULL);
		logfile = fopen("logfile.log","a+");
		start(); //To print the username and location;
		char term[1000]; //Get the input from the user
		int background, i = 0; //Background use checker and counter
		gets(term); 
		struct tm *tm = localtime(&current_time);
		char s[100];
		strftime(s, sizeof(s), "%c", tm); //convert the local time to C time format
		fprintf(logfile,"%s: %s\n",s,term); //prints the user command
		char *terms; //Terms after separation
		char *arg[1000]; //Command separated
		if(!(strcmp(term,"exit")))
		{ //Exit command to terminate the program
			 struct tm *tm = localtime(&current_time);
			 char s[100];
			 strftime(s, sizeof(s), "%c", tm);
			 fprintf(logfile,"%s: Terminal was terminated by exit command\n",s);
			 fclose(logfile); 
			 break;
   		} 
		if(!(strcmp(term,"clear")))  // @cls for Windows reset for Linux
		{
			struct tm *tm = localtime(&current_time);
			char s[100];
			strftime(s, sizeof(s), "%c", tm);
			fprintf(logfile,"%s: Window was cleared\n",s);
			fclose(logfile);
			system("reset");
			continue;
		}
		terms = strtok(term," "); //For separating the input with delimiters space
		while( terms != NULL ) 
		{
    		arg[i]=terms;
			terms = strtok(NULL, " ");
			i++;
	   	}
		arg[i] = NULL;
   		if(!strcmp(arg[0],"cd"))
		{

				if(chdir(arg[1]) == 0)
				{
					fclose(logfile);
					continue;
				}
				else
				{
					printf("Changing directory failed");
					struct tm *tm = localtime(&current_time);
					char s[100];
					strftime(s, sizeof(s), "%c", tm);
					fprintf(logfile,"%s: Changing directory failed!\n",s);
					printf("\n");
					fclose(logfile);
					continue;

				}
		}
   		background = ((arg[i-1]!=NULL)?(strcmp(arg[i-1],"&"))?0:1:0); //To determine if it's background process or not
   		arg[i-1] = (background?NULL:arg[i-1]);
   		pid_t child = fork(); //Child with process id data type  
   		if(child < 0)
		{
	   		printf("Child creation failed!");
	   		struct tm *tm = localtime(&current_time);
			char s[100];
			strftime(s, sizeof(s), "%c", tm);
	   		fprintf(logfile,"%s: Child creation failed!\n",s);
	   		printf("\n");
	   		fclose(logfile);
	   		continue;
   		}
		else if(child == 0)
		{
	   		if(execvp(arg[0],arg) < 0)
			{
		   		printf("Couldn't run the current command");
		   		struct tm *tm = localtime(&current_time);
				char s[100];
				strftime(s, sizeof(s), "%c", tm);
		   		fprintf(logfile,"%s: Couldn't run the current command\n",s);
		   		exit(-1); //To exit failed forked process.
		   		printf("\n");
		   		continue;  
	   		}
	   		printf("\n");
	   		fclose(logfile);
			continue;
  		}else
		{
        	// waiting for child to terminate if background = 0
        	if(background ==0) wait(NULL);
        	signal(SIGCHLD , signalhandler);
   		}
   		printf("\n");
   		fclose(logfile);
   		continue;
	}
 	return 0;
}
void start()
{
	char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
	char *username;
	username = getlogin();
	printf("%s@ubuntu:%s$ ",username,buff);
}







