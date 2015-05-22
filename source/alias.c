// The following contains all the functions necessary
// to add, and delete aliases
#include "shell.h"
#include "stdio.h"
#include "string.h"


// --------------------------------------------------------------------

// Create linked list of free alias spaces
void initAlias() {
    aliastab[MAX_ALIAS];
    for(unsigned int i = 0; i != MAX_ALIAS; ++i) {
        aliastab[i].name = Allocate(char);
        aliastab[i].cmd = Allocate(char);
        aliastab[i].valid = 0;
    }
}

// --------------------------------------------------------------------

// Add alias into list of aliases
void createAlias(char *aName, char *aCmd) {
    for(int i = 0; i != MAX_ALIAS; ++i) {
        if( aliastab[i].valid == 0 ) {
            strcpy(aliastab[i].name, aName);
            strcpy(aliastab[i].cmd, aCmd);
            aliastab[i].valid = 1;
            return;
        }
    }
}

// --------------------------------------------------------------------

// Return alias id if alias exist else return -1
int searchAlias(char *aName) {

    if(aName == NULL) { return -1; }

    for(int i = 0; i != MAX_ALIAS; ++i) {
        if( (aliastab[i].valid == 1) && (strcmp (aliastab[i].name,aName) == 0) ) {
            return i;  // Set alias to dirty
        }
    }
    return -1;  // Indicate alias does not exist
}

// --------------------------------------------------------------------

// Print the information of a given alias
void printAnAlias(char *al) {
    for(int i = 0; i != MAX_ALIAS; ++i) {
        if( (aliastab[i].valid == 1) && (strcmp (aliastab[i].name,al) == 0) ) {
            printf("alias %s= %s\n",aliastab[i].name, aliastab[i].cmd);
            return;
        }
    }
}

// --------------------------------------------------------------------

// Print all aliases created
void listAliases() {
    for(int i = 0; i != MAX_ALIAS; ++i) {
        if(aliastab[i].valid == 1) {
            printf("alias %s= %s\n",aliastab[i].name, aliastab[i].cmd);
        }
    }
}

// --------------------------------------------------------------------

void remAlias(int a_id) {
    aliastab[a_id].valid = 0;  // Set alias to dirty
}

// --------------------------------------------------------------------

// Removing an alias from the alias list
void removeAlias(char *aName) {
    for(int i = 0; i != MAX_ALIAS; ++i) {
        if( (aliastab[i].valid == 1) && (strcmp (aliastab[i].name,aName) == 0) ) {
            aliastab[i].valid = 0;  // Set alias to dirty
        }
    }
}