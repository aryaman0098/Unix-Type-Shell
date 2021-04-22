//Loading all the necessary header files
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/utsname.h>
#include<signal.h>
#include<ctype.h>
#include "LLS.h"
#include "utils.h"
#include "dirProc.h"
#include "exec.h"

//Driver function
int main(){

    printf("\n");
    getShellName(); //Getting the name of the shell

    //Defining the various signals
    signal(SIGCHLD, sigHandler);
    signal(SIGINT, sigHandler);


    char dir[maxLength]; //Defining variable to get the preset working directory
    //Some variables that help in getting the input
    size_t len = 0;
    char* command = NULL;
    
    //Starting the shells
    while(1){
        strcpy(dir, "");
        printf("%s%s> ", shellName, getcwd(dir, maxLength)); //Shell prompt
        
        //Getting the input command
        if(getline(&command, &len, stdin) == -1){
            printf("\n");
            return;
        }
        
        //Removing trailing '\n'
        if(strlen(command) > 0 && command[strlen(command) - 1]=='\n'){
            command[strlen(command) - 1] = '\0';
        }
        
        //Removing leading and trailing spaces
        command = trim(command);
        
        //Creating copy of the command
        char command_[strlen(command)];
        strcpy(command_, command);

        //Checking for redirection and calling the necessary function
        if(check(addChars(command_, '\|'), "|") == 1 || check(addChars(command_, '\>'), ">") == 1 || check(addChars(command_, '\<'), "<") == 1){
            execRedirectionAndPipe(command);
        }
        //Checking for change directory command  and calling the necessary function
        else if(strlen(command) > 3 && command[0] == 'c' && command[1] == 'd' && (command[2] == ' ' || command[2] == '\t')){
            push(&histHead, command, getpid());
            changeDir(trim(command + 3));
        }
        //Printing the last 'N' commands executed using HISTN command
        else if(command[0] != '!' && strstr(command, "HIST")){
            if(isNumber(command + 4) == 1){
                printHistory(histHead, atoi(command + 4));
            }else{
                printf("[!!]Error while executing command. To print the latest \'N\' commands, run the command \'HISTN\'\n");
            }
        }
        //Executing the Nth command using !HISTN command
        else if(strstr(command, "HIST")){
            if(isNumber(command+5) == 1 && numWords(command) == 1){
                execCommandN(histHead, atoi(command + 5));
            }else{
                printf("[!!]Error while executing command. To execute command number \'N\' , run the command \'!HISTN\'\n");
            }
        }
        //Command to print the pid of the current shell
        else if(strcmp(command, "pid") == 0){
            printf("\nCommand name : %-30s process id : %-5d\n", "./shell", getpid());
        }
        //Command to print the pid of all the spawned processes from the shell
        else if((strstr(command, "pid") != NULL) && (strstr(command, "all") != NULL) && (numWords(command) == 2)){
            if(histHead == NULL){
                printf("\n*No process spawned from this shell so far*\n");
            }
            else{
                printPidAll(histHead);
            }
        }
        //Command to print all the active processes spanwed from the shell
        else if((strstr(command, "pid") != NULL) && (strstr(command, "current") != NULL) && (numWords(command) == 2)){
            if(currProc == NULL){
                printf("\n*No process currently active*\n");
            }
            else{
                printPidAll(currProc);
            }
        }
        //Command to exit the shell
        else if(strcmp(command, "STOP") == 0){
            killBackgroundProcesses(&currProc); //Killing background processes, if any
            printf("\n*Exiting from this shell, bye*\n");
            free(command);
            break;
        }
        else if(strcmp(command, "HELP") == 0){
            printHelp();
        }
        //Execution of normal command (background and foreground)
        else{
            int flag = 0;
            for(int i = 0; i < strlen(command); i++){
                if(isalpha(command[i]) || isdigit(command[i])){
                    flag = 1;
                    break;
                }
            }
            
            if(flag == 1){
                
                char* command__ = strdup(command);
                if(command[strlen(command) - 1] != '&'){ //Checking if the process is background process or not
                    char** arr = tokenize(command);
                    execCommand(arr, 0);
                }else{
                    command[strlen(command) - 1] = '\0';
                    char** arr = tokenize(command);
                    execCommand(arr, 1);
                }
            }
        }
        printf("\n");
    }
}