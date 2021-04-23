//Loading all the necessary header files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/utsname.h>
#include "utils.h"


//Utility function to remove the leading and preceeding spaces and tabs in a stirng
char* trim(char* command) { 
    
    int n = strlen(command);
    char *str = (char*)malloc(sizeof(char) * (n + 1));
    int count = 0, j, k; 
    while (isspace(command[count])) { 
        count++; 
    } 
    for (j = count, k = 0; command[j] != '\0'; j++, k++) { 
        str[k] = command[j]; 
    } 
    
    if (!isspace(str[strlen(str) - 1])){
            str[k] = '\0'; 
            return str;
    }
    else{
        for(int i = strlen(str) - 1; i >=0; i--){
            if (!isspace(str[i])){
                str[i+1] = '\0';
                return str;
            } 
        } 
    }
}


//Utility function to tokenize the given strings into respective tokens
char** tokenize(char* str){
    int n = strlen(str);
    char** toks = (char**)malloc(sizeof(char*) * (n + 1));;
    char* tok;
    int count = 0;
    while(tok = strtok_r(str, " \t\n", &str)){
        toks[count] = tok;
        count++;
    }
    toks[count] = NULL;
    return toks;
}


//Utility function to add specific characters to preserve a given section in the string
char* addChars(char* str, char c){
    int x = 170;
    for(int i = 0; str[i] != NULL; i++){       
        if((i + 1) < strlen(str) && str[i] == '\\' && str[i + 1] == c){
            str[i] = (char)x;
            str[i + 1] = (char)x;
            i++;
        }
    }
    return str;
}

//Utility function to replace the specific characters added in the string
char* replace(char* str, char c){
    for(int i = 0; str[i] != NULL; i++){
        if((i + 1) < strlen(str) && (int)str[i] == -86 && (int)str[i + 1] == -86){
            str[i] = (char)92;
            str[i + 1] = c;
            i++;
        }
    }
    return str;
}


//Utility function to split the string from a given deliminator
char** parse(char* str, char* delim){
    int n = strlen(str);
    char** toks = (char**)malloc(sizeof(char*) * (n + 1));   
    char* tok;
    int count = 0;
    while(tok = strtok_r(str, delim, &str)){
        toks[count] = trim(tok);
        count++;
    }
    toks[count] = NULL;
    return toks;
}

//Utility function to check whether a character is present in string or not
int check(char* s, char* delim){
    int n = strlen(s);
    char* tok = strtok_r(s, delim, &s);
    int n_;
    if(tok == NULL){
        n_ = 0;
    }else{
        n_ = strlen(tok);
    }

    if(n == n_){
        return 0;
    }else{
        return 1;
    }
}

//Utility function to check if the given string is a number
int isNumber(char* str){
    if(strlen(str) == 0){
        return -1;
    }
    for(int i = 0; str[i] != NULL; i++){
        if(48 <= (int)str[i] && (int)str[i] <= 57){
            continue;
        }else{
            return -1;
        }
    }
    return 1;
}

//Utility function to count the number of words in a string
int numWords(char* str){
    int number = 0;
    for(int i = 0; i < strlen(str); i++){
        
        if(str[i] == ' ' || str[i] == '\t' || str[i] == '\n'){
            continue;
        }
        else{
            
            number++;
            while(str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && (i < strlen(str))){
                i++;
            }
            i--;
        }
    }
    return number;
}


//Utility function to get the name of the shell
void getShellName(){
    char username[maxLength];
    getlogin_r(username, maxLength);

    char systemName[maxLength];
    struct utsname unameData;
    uname(&unameData);  
    strcpy(systemName, unameData.nodename); 

    strcat(shellName, "<");
    strcat(shellName, username);
    strcat(shellName, "@");
    strcat(shellName, systemName);
    strcat(shellName, ":~");
}


//Utility function that displays the instructions on how to use this shell
void printHelp(){
    printf("\n");
    printf("\t\t************************Brief Instructions************************\n\n");
    printf("   1. Run the command \'HISTN\' to view the last N commands executed on this shell\n");
    printf("   2. Run the command \'!HISTN\' to execute the Nth command executed on this shell\n");
    printf("   3. Run the commnad \'pid\' to get the process id of this shell\n");
    printf("   4. Run the commnad \'pid all\' to get the process ids of all the processes spawned from this shell\n");
    printf("   5. Run the commnad \'pid current\' to get the process ids of all the processes spawned from this shell and currently active\n");
    printf("   6. Run the commnad \'HELP\' to get the instructions for this shell\n\n");
    printf("\t\t*******************************************************************\n\n");
    return;
}

void red (){
  printf("\033[1;31m");
}

void yellow(){
    printf("\033[1;33m");
}

void cyan(){
    printf("\033[0;36m");
}

void purple(){
    printf("\033[0;35m");
}


void bold(){
    printf("\033[1m");
}

void reset (){
  printf("\033[0m");
}


