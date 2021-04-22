#ifndef LLS
#define LLS

//Counter for storing number of elements in history
int indexCounter;

//Structure of linked list to store the history
struct Node{
    char *commandName;  //Stores the command name
    int indexNum;       //Stores the index number corresponding to each command
    int processId;      //Stores the process id of all the commands
    struct Node* next;  //Stores the address of next node
};


struct Node* histHead;
struct Node* currProc;


void push(struct Node** head, char* command, int pid);

void deleteNode(struct Node** head, int pid);

void printHistory(struct Node* head, int n);

void printPidAll(struct Node* head);

int isPresentPid(struct Node* head, int p);

char* getCommand(struct Node* head, int index);

#endif


