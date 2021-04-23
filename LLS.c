#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "LLS.h"


//Linked List function to add new nodes to the linked list
void push(struct Node** head, char* command, int pid){
    
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->commandName = (char*)malloc(strlen(command));
    
    strcpy(newNode->commandName, command);
    
    if(*head == histHead){
        indexCounter++;
    }
    
    
    newNode->indexNum = indexCounter;
    newNode->processId = pid;

    newNode->next = NULL;

    if(*(head) == NULL){
        *head = newNode;
        return;
    }
    
    struct Node* temp = *(head);
    while(temp->next !=NULL){
        temp = temp->next;
    }

    temp->next = newNode;
}

//Linked list function to delete a Node from the linked list 
void deleteNode(struct Node** head, int pid){
    
    
    struct Node *temp = *head, *prev;

    if(temp != NULL && temp->processId == pid){
        *head = temp->next;
        free(temp);
        return; 
    }

    while(temp != NULL && temp->processId != pid){
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL){
        return;
    }

    prev->next = temp->next;

    free(temp);
}

//Linked List function to print all the commands stored in the linked list
void printHistory(struct Node* head, int n){

    printf("\n");

    if(head == NULL){
        printf("No command to show\n\n");
        return;
    }
    
    struct Node* temp = head;
    int counter = 0;
    while(temp != NULL){
        counter++;
        temp = temp->next;
    }
    if(counter <= n){
        while(head != NULL){
            int x = head->indexNum;
            int len = 0;        
            while(x > 0){
                len++;
                x = x / 10;
            }
            printf("\033[0;35m%4d. \033[1;33m%s\n", head->indexNum, head->commandName);
            head = head->next;
        }
    }
    else{
        
        int c = counter - n;
        
        while(c > 0){
            c--;
            head = head->next;
        }

        while(head != NULL && n > 0){
            n--;
            printf("\033[0;35m%4d%4d. \033[1;33m%s\n", head->indexNum, head->commandName);
            head = head->next;
        }
    }
    reset();
}

//Linked List fucntion to print the process ids of all the processes spawned from the shell
void printPidAll(struct Node* head){
    printf("\n");
    char* c;
    while(head != NULL){
        c = (char*)malloc(sizeof(char) * strlen(head->commandName));
        strcpy(c, "");
        for(int i = 0; head->commandName[i] != ' '; i++){
            strncat(c, &head->commandName[i], 1);
        }
        purple();
        printf("Process name: ");
        yellow();
        printf("%-30s", c); 
        purple();
        printf("id: ");
        yellow();
        printf("[%-5d]\n", head->processId);
        head = head->next;
        c = NULL;
    }
}

//Linked List function to print the process ids of all the currently active processes spawned from the shell
int isPresentPid(struct Node* head, int p){
    int isPresent = 0;
    while(head != NULL){
        if (p == head->processId){
            isPresent = 1;
        }
        head = head->next;
    }
    return isPresent;
}

//Linked List function to get the command corresponding to a given index number
char* getCommand(struct Node* head, int index){
    while(head != NULL){
        if(head->indexNum == index){
            return head->commandName;
        }
        head = head->next;
    }
    return "Invalid index";
}


