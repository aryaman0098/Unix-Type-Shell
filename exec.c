//Loading all the necessary header files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include <sys/wait.h>
#include "LLS.h"
#include "utils.h"
#include "exec.h"


//Function that handles signals (like ctrl - C or child process termaination)
void sigHandler(int sig){
    //Handling child process termination
    if(sig == SIGCHLD){
        int status;
        pid_t p = waitpid(-1, &status, WNOHANG);
        if(isPresentPid(currProc, p) == 1){
            if(p != -1){
                char* msg = (char*)calloc(maxLength, sizeof(char));
                char* dir = (char*)calloc(maxLength, sizeof(char));
                char* es = (char*)calloc(100, sizeof(char));   
                char* pid = (char*)calloc(20, sizeof(char));

                sprintf(pid, "%d", p);
                strcpy(msg, "\n\033[0;35mProcess with process id: \033[1;33m[");
                strcat(msg, pid);
                strcat(msg, "]\033[0;35m exited with status: ");
                        
                int exitStatus = WEXITSTATUS(status);
                sprintf(es, "%d", exitStatus);
                strcat(msg, es);
                strcat(msg, "\n");
                strcat(msg, "\033[0m");

                deleteNode(&currProc, p);

                strcat(msg, "\n\033[1;33m\033[1m");
                strcat(msg, shellName);
                strcat(msg, "\033[0;36m\033[1m");
                strcat(msg, getcwd(dir, maxLength));
                strcat(msg, "-> ");
                strcat(msg, "\033[0m");
                write(STDOUT_FILENO, msg, strlen(msg));
                        
                free(msg);
                free(dir);
                free(es);
                free(pid);
            }
        }
    //Handling ctrl - C
        }else if(sig == SIGINT){
            char* msg = (char*)calloc(maxLength, sizeof(char));
            char* dir = (char*)calloc(maxLength, sizeof(char));
            strcat(msg, "\n\033[1;33m\033[1m");
            strcat(msg, shellName);
            strcat(msg, "\033[0;36m\033[1m");
            strcat(msg, getcwd(dir, maxLength));
            strcat(msg, "-> ");
            strcat(msg, "\033[0m");
            write(STDOUT_FILENO, msg, strlen(msg));
            free(msg);
            free(dir);
        }    
    return;
}


//Function that executes the input command
void execCommand(char** args, int background){
    
    int size = 0;
    for(int i = 0; args[i] != NULL; i++){
        size += strlen(args[i]);
    }
    
    char command[size * 2];
    strcpy(command, "");
    for(int i = 0; args[i] != NULL ; i++){
        strcat(command, args[i]);
        strcat(command, " ");
    }

    if(background == 0){ // Foreground command
        int p = fork();
        
        // Saving the command in history
        push(&histHead, command, p);
        
        if(p == -1){
            red();
            printf("\n[!!] An error occurred while executing the command\n");
            reset();
            return;
        }
        if(p == 0){
            printf("\n");
            execvp(args[0], args);
            red();
            printf("[!!] An error occurred while executing ");
            yellow();
            printf("%s\n", command);
            reset();
            _exit(0);
        }else{
            wait(NULL);
            return;
        }
    }
    else if(background == 1){  // Background command
        int p = fork();
        
        //Saving the command in history and list of active processes
        push(&histHead, command, p);
        push(&currProc, command, p);

        if(p == -1){
            red();
            printf("[!!] An error occurred while executing the command\n");
            reset();
            return;
        }
        if(p == 0){
            setpgid(0, 0);
            printf("\n");
            execvp(args[0], args);
            red();
            printf("[!!] An error occurred while executing ");
            yellow();
            printf("%s\n", command);
            reset();
            _exit(0);
        }else{
            purple();
            printf("\nProcess ");
            yellow();
            printf("%s", args[0]);
            purple();
            printf(" started with pid : ");
            yellow();
            printf("[%d]\n", p);
            reset();
            return;    
        }
    }
}


//Function that handles I/O redirection commands and pipes
void execRedirectionAndPipe(char* command){
    
    char** arr = parse(command, "|"); //Checking for pipes
    int c = 0;
    
    while(arr[c] != NULL){
        c++;
    }

    //If pipes are not present
    if(c == 1){
        int flag1, flag2;
        char* s1 = strndup(arr[0], strlen(arr[0]));
        char* s2 = strndup(arr[0], strlen(arr[0]));
        char* s3 = strndup(arr[0], strlen(arr[0]));
        flag1 = check(addChars(s1, '\>'), ">");
        flag2 = check(addChars(s2, '\<'), "<");
        
        //If only output redirection is present
        if(flag1 == 1 && flag2 == 0){
            char** arr_ = parse(arr[0], ">");
            int counter = 0;
            while(arr_[counter] != NULL){
                counter++;
            }
            char** args = tokenize(arr_[0]);
            int p = fork();
            push(&histHead, s3, p); // Saving in history
            if(p == 0){
                FILE* f = fopen(arr_[counter - 1], "w"); //Clearing the file contents
                fclose(f);
                int file = open(arr_[counter - 1], O_WRONLY | O_CREAT, 0777); //Openning the file
                if(file == -1){
                    red();
                    printf("\n[!!]An error occured in openning file ");
                    yellow();
                    printf("%s\n", arr_[counter - 1]);
                    reset();
                    close(file);
                    exit(0);
                    return;
                }else{
                    dup2(file, STDOUT_FILENO); //Changing the standard output
                    close(file);
                    printf("\n");
                    execvp(args[0], args);
                    red();
                    printf("[!!] An error occurred while executing ");
                    yellow();
                    printf("%s\n", command);
                    reset();
                    _exit(0);
                }
            }else{
                wait(NULL);
                if(arr){
                    free(arr);
                    arr = NULL;
                }
                if(arr_){
                    free(arr_);
                    arr_ = NULL;
                }
                return;
            }
        }
        //If only input redirection is present
        else if(flag1 == 0 && flag2== 1){
            char** arr_ = parse(arr[0], "<");
            int counter = 0;
            while(arr_[counter] != NULL){
                counter++;
            }
            char** args = tokenize(arr_[0]);
            int p = fork();
            push(&histHead, s3, p); //Saving in history
            if(p == 0){
                int file = open(arr_[counter - 1], O_RDONLY); //Opening the file
                if(file == -1){
                    red();
                    printf("\n[!!]An error occured in openning file ");
                    yellow();
                    printf("%s\n", arr_[counter - 1]);
                    reset();
                    close(file);
                    exit(0);
                    return;
                }else{
                    dup2(file, STDIN_FILENO); //Changing the standard input
                    close(file);
                    
                    printf("\n");
                    execvp(args[0], args);
                    red();
                    printf("[!!] An error occurred while executing ");
                    yellow();
                    printf("%s\n", command);
                    reset();
                    _exit(0);  
                }
            }else{
                wait(NULL);
                if(arr){
                    free(arr);
                    arr = NULL;
                }
                if(arr_){
                    free(arr_);
                    arr_ = NULL;
                }
                return;
            }
        }
        //If both input and output redirections are present
        else if(flag1 == 1 && flag2 == 1){
            char** arr_ = parse(arr[0], ">");
            char** arr__ = parse(arr[0], "<");
            int counter1 = 0;
            while(arr_[counter1] != NULL){
                counter1++;
            }
            int counter2 = 0;
            while(arr__[counter2] != NULL){
                counter2++;
            }
            char** args = tokenize(arr__[0]);

            int p = fork();

            push(&histHead, s3, p); //Saving in history

            if(p == 0){
                
                //Openning the file and clearing the contents of output file
                int file1 = open(arr__[counter2 - 1], O_RDONLY);
                FILE* f = fopen(arr_[counter1 - 1], "w");
                fclose(f);
                int file2 = open(arr_[counter1 - 1], O_WRONLY | O_CREAT, 0777);
                
                if(file1 == -1){
                    red();
                    printf("\n[!!]An error occured in openning file ");
                    yellow();
                    printf("%s\n", arr__[counter2 - 1]);
                    reset();
                    close(file1);
                    _exit(0);
                    return;
                }

                if(file2 == -1){
                    red();
                    printf("\n[!!]An error occured in openning file ");
                    yellow();
                    printf("%s\n", arr_[counter1 - 1]);
                    reset();
                    close(file2);
                    _exit(0);
                    return;
                }

                //Changing the standard input output
                dup2(file1, STDIN_FILENO);
                close(file1);
                dup2(file2, STDOUT_FILENO);
                close(file2);
                
                printf("\n");
                execvp(args[0], args);
                red();
                printf("[!!] An error occurred while executing ");
                yellow();
                printf("%s\n", command);
                reset();
                _exit(0);

            }else{
                wait(NULL);
                if(arr){
                    free(arr);
                    arr = NULL;
                }
                if(arr_){
                    free(arr_);
                    arr_ = NULL;
                }
                if(arr__){
                    free(arr__);
                    arr__ = NULL;
                }
            }
        }        
    }
    //If pipes are present in the input command
    else{
        int fd[c][2];
        //Openning pipes
        for(int i = 0; i < c; i++){
            if(pipe(fd[i]) == -1){
                printf("[!!]Error occurred in redirection\n");
                return;
            }
        }

        for(int i = 0; arr[i] != NULL; i++){
            char* c_ = strndup(arr[i], strlen(arr[i]));
            char* c__ = strndup(arr[i], strlen(arr[i]));
            char** args = tokenize(arr[i]);

            int p = fork();
            push(&histHead, c_, p); //saving in history
            if(p == 0){
                if(i == 0){
                    //If input redirection is present in the first command of the pipe
                    if(check(addChars(c_, '\<'), "<") == 1){
                        char** arr1 = parse(c__, "<");
                        int counter = 0;
                        while(arr1[counter] != NULL){
                            counter++;
                        }
                        int file = open(arr1[counter - 1], O_RDONLY);
                        char** args = tokenize(arr1[0]);

                        if(file == -1){
                            red();
                            printf("\n[!!]An error occured in openning file ");
                            yellow();
                            printf("%s\n", arr1[counter - 1]);
                            reset();
                            close(file);
                            _exit(0);
                            return;
                        }

                        dup2(file,  STDIN_FILENO);
                        dup2(fd[i+1][1], STDOUT_FILENO);

                        close(fd[i][1]);
                        close(file);

                        if(arr1){
                            free(arr1);
                            arr1 = NULL;
                        }

                        printf("\n");
                        execvp(args[0], args);
                        red();
                        printf("[!!] An error occurred while executing ");
                        yellow();
                        printf("%s\n", command);
                        reset();
                        _exit(0);
                    }else{
                        //Changing standard I/O
                        dup2(fd[i + 1][1], STDOUT_FILENO);
                        
                        close(fd[i][0]);
                        close(fd[i][1]);
                        close(fd[i + 1][0]);
                        
                        printf("\n");
                        execvp(args[0], args);
                        red();
                        printf("[!!] An error occurred while executing ");
                        yellow();
                        printf("%s\n", command);
                        reset();
                        _exit(0);
                            }
                }
                //The last command in the pipe
                else if(arr[i + 1] == NULL){
                    //If output redirection is present in the last command
                    if(check(addChars(c_, '\>'), ">") == 1){
                        char** arr1 = parse(c__, ">");
                        int counter = 0;
                        while(arr1[counter] != NULL){
                            counter++;
                        }
                        FILE* f = fopen(arr1[counter - 1], "w");
                        fclose(f);
                        int file = open(trim(arr1[counter - 1]), O_WRONLY | O_CREAT, 0777);

                        if(file == -1){
                            red();
                            printf("\n[!!]An error occured in openning file ");
                            yellow();
                            printf("%s\n", arr1[counter - 1]);
                            reset();
                            close(file);
                            _exit(0);
                            return;                  
                        }

                        char** args = tokenize(arr1[0]); 
                        //Changing standard I/O
                        dup2(file, STDOUT_FILENO);
                        dup2(fd[i][0], STDIN_FILENO);
                        
                        close(fd[i][1]);
                        close(fd[i][0]);
                        
                        if(arr1){
                            free(arr1);
                            arr1 = NULL;
                        }

                        printf("\n");
                        execvp(args[0], args);
                        
                        red();
                        printf("[!!] An error occurred while executing ");
                        yellow();
                        printf("%s\n", command);
                        reset();
                        _exit(0);
                    }else{
                        //Changing standard I/O
                        dup2(fd[i][0], STDIN_FILENO);
                    
                        close(fd[i][1]);
                        close(fd[i][0]);
                        
                        printf("\n");
                        execvp(args[0], args);
                        red();
                        printf("[!!] An error occurred while executing ");
                        yellow();
                        printf("%s\n", command);
                        reset();
                        _exit(0); 
                    }
                }
                //All the commands in between
                else{
                    //Changing standard I/O
                    dup2(fd[i][0], STDIN_FILENO);
                    dup2(fd[i + 1][1], STDOUT_FILENO);
                    
                    close(fd[i][1]);
                    close(fd[i + 1][0]);
                    
                    execvp(args[0], args);
                    red();
                    printf("[!!] An error occurred while executing ");
                    yellow();
                    printf("%s\n", command);
                    reset();;
                    _exit(0);
                }
            }else{
                close(fd[i][0]);
                close(fd[i][1]);               
            }
        }
        wait(NULL);
        return;
    }
}


//Function to kill any background process, when the shell exits
void killBackgroundProcesses(struct Node** head){
    
    struct Node* temp = *head;
    
    if (temp == NULL){
        return;
    }
    printf("\n");
    while (temp != NULL){
        red();
        printf("\nKilling process ");
        yellow();
        printf("%s", temp->commandName);
        red();
        printf("having pid: ");
        yellow();
        printf("[%d]\n", temp->processId);
        reset();
        kill(temp->processId, SIGKILL);
        deleteNode(&head, temp->processId);
        temp = temp->next;
    }

    free(temp);
    return;
}


//Function to execute the Nth command from history
void execCommandN(struct Node* head, int n){
    char* command = getCommand(head, n);
    if(strcmp(command, "Invalid index") == 0){
        printf("[!!]Invalid index\n");
    }
    //Checking for the various typse of command that can be present
    else{
        char* command_ = strndup(command, strlen(command));
        if(check(addChars(command_, '\|'), "|") == 1 || check(addChars(command_, '\>'), ">") == 1 || check(addChars(command_, '\<'), "<") == 1){
            execRedirectionAndPipe(command);
        }else if(strlen(command) > 3 && command[0] == 'c' && command[1] == 'd' && (command[2] == ' ' || command[2] == '\t')){
            push(&histHead, command, getpid());
            changeDir(trim(command + 3));
        }else{
            if(command[strlen(command) - 1] != '&'){
                char** arr = tokenize(command);
                execCommand(arr, 0);
            }else{
                command[strlen(command) - 1] = '\0'; 
                char** arr = tokenize(command);
                execCommand(arr, 1);
            }
        }
    }
    return;
}


