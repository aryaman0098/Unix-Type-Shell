#ifndef EXEC
#define EXEC

#include<signal.h>

void sigHandler(int sig);
void execCommand(char** args, int background);
void killBackgroundProcesses(struct Node** head);
void execRedirectionAndPipe(char* command);
void execCommandN(struct Node* head, int n);

#endif