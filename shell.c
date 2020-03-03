/*
Name: Kirtik Singh
ID: 1810110109
Teammate name(s): Prakhar Bhasin
*/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>

#define MAXLINE 80

char history[10][80]; // 2D Array to store 10 elements of max_length 80
int count = 0;


void displayHistory(){ // Function that prints the history

    printf("History:- \n");    
    int i, j = 0;
    int Current = count;
    
    for (i = 0; i<10; i++){
        printf("%d ", Current);

        // Now printing each line of command character by character
        while(history[i][j] != '\n' && history[i][j] != '\0'){  
            printf("%c", history[i][j]);
            j++;
        }
        printf("\n");
        j=0;
        Current--;
        if(Current==0)
            break;
    }
    printf("\n");
}



void parse(char *line, char **args){
    while (*line != '\0'){       // if it is not the end of line
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0';     // replacing white spaces with 0
        *args++ = line;          // saving the argument position 
        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n') 
            line++;             // skip the argument until
    }
    *args = '\0';                 // end of argument list
}


void execute(char **args){   // used for finally executing the process
    pid_t pid;
    int status;
     
    if((pid = fork()) < 0){     // fork a child process
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if(pid == 0){          // for the child process
        if(execvp(*args, args) < 0){     // execute the command 
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else{                                  // for the parent   
        while(wait(&status) != pid);      // wait for completion 
    }
}


int main(void){
    char line[MAXLINE];  // Defining each line that user enters in the shell
    char *args[MAXLINE/2 + 1]; // Tokenising the line array
    int should_run = 1; // flag variable to check when the user should exit out of the terminal
    int i = 0;

    while(should_run){
        printf("osh>");  // prompt for the shell
        fflush(stdout);
        gets(line);
        parse(line, args);

        if(strcmp(args[0], "quit") == 0) // checks if user wants to exit the shell
            exit(0);

        else if(strcmp(args[0],"history")==0){  // Prints the history if user enters "History"
            if(count>0)
                displayHistory();
            else
                printf("\nNo Commands in the history\n");
        }

        else if (args[0][0]-'!' ==0){  // Used for checking if user wants to check for custom commands starting with "!"
            int x = args[0][1]-'0'; 
            int z = args[0][2]-'0'; 
            
            if(x>count){ //second letter check
                printf("\nNo Such Command in the history\n");
                strcpy(line,"Wrong command");
            } 

            else if (z!=-48){ //third letter check
            printf("\nNo Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");
            strcpy(line,"Wrong command");
            }

            else{
                if(x==-15)  //Checking for '!!',ascii value of '!' is 33.
                    strcpy(line,history[0]);  // this will be your 10 th(last) command
                else if(x==0){  //Checking for '!0'
                    printf("Enter proper command");
                    strcpy(line,"Wrong command");
                }                
                else if(x>=1){  //Checking for '!n', n >=1
                    strcpy(line,history[x]);
                }
                
            }
            execute(args);  // FINAL EXECUTION HAPPENS HERE FOR THE FINAL TOKENS
	    }
        else{
            execute(args);  // FINAL EXECUTION HAPPENS HERE FOR THE FINAL TOKENS
        }

        for (i = 9;i>0; i--)  //Moving the history elements one step higher
       	    strcpy(history[i], history[i-1]);
    
        strcpy(history[0],line);  //Updating the history array with input buffer
        count++;
	    if(count>10)
	        count=10;
    }
    return 0;
}