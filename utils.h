#ifndef PROCESS
#define PROCESS

//Defining some params
#define maxLength 1000

//Variable that stores the name of the shell
char shellName[maxLength];

char* trim(char* command);
char** tokenize(char* str);
char* addChars(char* str, char c);
char* replace(char*, char c);
char** parse(char* str, char* delim);
int check(char* s, char* delim);
int isNumber(char* str);
int numWords(char* str);
void getShellName();
void printHelp();
void red ();
void yellow();
void cyan();
void purple();
void bold();
void reset ();


#endif