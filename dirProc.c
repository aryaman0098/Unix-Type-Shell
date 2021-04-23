//Loading all the necessary header files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "dirProc.h"

//Utility function to change the present working directory
void changeDir(char* str){
    char* path = (char*)calloc(strlen(str), sizeof(char));
    int count = 0;
    while(str[count] != NULL){
        if((int)str[count] == 92 && str[count + 1] == ' '){
            strncat(path, &str[count + 1], 1);
            count++;
        }else{
            strncat(path, &str[count], 1);
        }
        count++;
    }

    if(chdir(path) == -1){
        printf("[!!] An error occurred in changing the directory\n");
    }
    free(path);
    return;
}
