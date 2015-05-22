#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell.h"


// Create cmd, and alias tables
COMMAND comtab[MAXCMDS];
struct aliasNode aliastab[MAX_ALIAS];


// ------------------------------------------------------------

void testingPrintComTab() {
    int i, j;
    printf("COMMAND\tName\tr\tw\tBI\tNo.\n");
    for(i = 0; i != (curr_cmd + 1); ++i) {
        printf("%d\t%s\t%d\t%d\t%d\t%d\t\tARGS:\t", i, comtab[i].comname, comtab[i].iofd[0], comtab[i].iofd[1], comtab[i].builtin, comtab[i].nargs);
        for(j = 0; ((comtab[i].atptr != NULL) && (j != comtab[i].nargs)); ++j) { // Stop: # args of this command
            printf("%s\t",comtab[i].atptr->args[j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void resetComTab() {

    // Reset flags
    curr_cmd = 0;
    in_rd = 0;              // Input redir = true
    o_rd = 0;               // Output redir = false

    // Reset the command table
    for(int i = 0; i != MAXCMDS; ++i) {
        p = &comtab[i];     // Create a temporaty pointer to command args
        if( p->atptr != NULL ) {

            // Free the spaces that have been allocated for every single word
            for(int j = 0; j != comtab[i].nargs; ++j) {
                if( p->atptr->args[j] != NULL ) {
                    free( p->atptr->args[j] );
                    p->atptr->args[j] = NULL;
                } 
            }

            // Must free memory allocated by arguments array
        }
        
        // Reset all the reamaining attributes
        comtab[i].comname = NULL;
        comtab[i].builtin = -1;
        comtab[i].nargs = 0;
        comtab[i].iofd[0] = 0;
        comtab[i].iofd[1] = 1;
        comtab[i].atptr = NULL;
    }
}



int getCommand() {

    // init_lex_parser();      // Init the lex and the yacc
    resetComTab();          // Reset commands table
    initPipes();            // Initialize pipes
    printf("before yyparse: \n");
    int a = yyparse();
    printf("Get command yyparse: %d\n", a);
    if(a) {
        // Understand what the error is, print it to
        // the user, then return 1 to say so
        printf("Display errors\n");
        // testingPrintComTab();
        return 1;
    }
    else {



        // Parsing was a success
        // Return 0 to main function to indicate so
        printf("11111111111\n");
        testingPrintComTab();
        printf("22222222222\n");
        return 0;
    }
}

void initShell() {

    // Set all flags to false
    goodbye = 0;            // Set exit flag to false
    curr_cmd = 0;           // Set current command to 1st slot
    last_cmd = -1;          // Indicate no past command executed yet
    SHELL_PWD = getenv("PWD");   // Copy the current path
    // Initialize all the other stuffs
    initAlias();

    // Initialize commands table
    for(int i = 0; i != MAXCMDS; ++i) {
        comtab[i].comname = NULL;
        comtab[i].builtin = -1;
        comtab[i].nargs = 0;
        comtab[i].iofd[0] = 0;
        comtab[i].iofd[1] = 1;
        comtab[i].atptr = NULL;
    }
}

void cmd2str(int in, char* n_comd) {
    printf("B4 write, n_comd: %s\n", n_comd);
    for(int i = in; i <= curr_cmd; ++i) {

        // Add pipe character
        if(i != 0) { strcat(n_comd," | "); }
        
        // Case A: builtin
        if( comtab[i].builtin != -1) {

            switch(comtab[i].builtin) {
                case CD: strcat(n_comd,"cd ");
                    break;
                case CDHOME: strcat(n_comd,"cd ");
                    break;
                case PRINTALIAS: strcat(n_comd,"alias ");
                    break;
                case SETALIAS: strcat(n_comd,"alias ");
                    break;
                case UNALIAS: strcat(n_comd,"unalias ");
                    break;
                case SETENV: strcat(n_comd,"setenv ");
                    break;
                case UNSETENV: strcat(n_comd,"unsetenv ");
                    break;
                case PRINTENV: strcat(n_comd,"printenv ");
                    break;
            }

            int a = 1;
            while( comtab[i].atptr->args[a] != NULL ) {
                strcat(n_comd,comtab[i].atptr->args[a]);
                strcat(n_comd,"/");
                ++a;
            } 
        }
        else {
            int a = 0;
            while( comtab[i].atptr->args[a] != NULL ) {
                strcat(strcat(n_comd,comtab[i].atptr->args[a])," ");
                ++a;
            } 
        }
        
        printf("Each write, n_comd: %s\n", n_comd);
    }
    printf("After write, n_comd: %s\n", n_comd);
}

// Return 1 if expansion happen
// Else return 0
int expansion_check(char* n_comd) {
    // Case A: Check if first word is alias
    int al = searchAlias(comtab[0].comname);
    listAliases();
    printf("Exansion Check | al:%d\n",al);
    if( al != -1 ) {
        printf("alias found: %d\n", al);
        printf("%s\n",n_comd );

        // Get the first command
        int a = 0;
        while( comtab[0].atptr->args[a] != NULL ) {
            if(a == 0) { strcat(n_comd,aliastab[al].cmd); }
            else { strcat(n_comd,comtab[0].atptr->args[a]); }
            ++a;
        }
        
        // Get remaining command
        cmd2str(1, n_comd);

        printf("aqui n_comd= %s\n", n_comd);
        return 1;
    }


    printf("exp check return 0\n");
    return 0;
}

// ---------------------------------------------------------

void expand() {

    // Case A: Expand if needed
    // int loop = 0;
    // char* n_comd = Allocate(char);
    // do{
        
        // if( expansion_check(n_comd) == 1 ) {
        //     printf("n_comd: %s\n",n_comd);
        //     init_lex_parser();
        //     resetComTab();          // Reset commands table
        //     redFlexinput(n_comd);
        //     testingPrintComTab();
        //     free( n_comd );
        //     n_comd = Allocate(char);
        //     loop = 1;
        // }
    //     else { loop = 0; }
    // } while(loop);
    // printf("Here\t%s\n",n_comd);
    // free( n_comd );

    int loop;
    char* n_comd;
    do {

        loop = 0;
        n_comd = Allocate(char);
        int a = expansion_check(n_comd);
        if( a == 1) {
            printf("n_comd: %s\n",n_comd);
            init_lex_parser();
            resetComTab();          // Reset commands table
            printf("New cmd total: %d\n", curr_cmd);
            redFlexinput(n_comd);
            testingPrintComTab();
            free(n_comd);
            loop = 1;
        }

    } while(loop);

    printf("Exit expand()\n"); 
}

// ---------------------------------------------------------

void processCommand() {

    // Reset to standard input and standard output if no redirection
    if(!in_rd) { comtab[0].iofd[0] = 0; }
    if(!o_rd) { comtab[curr_cmd].iofd[1] = 1; }

    // Expand command if needed
    // char* n_comd = Allocate(char);
    // expansion_check(n_comd);
    // cmd2str(0,n_comd);
    // expand(); 


    // Begin the execution process
    for(int i = 0; i != (curr_cmd + 1); ++i) {
        printf("Executing cmd: %d\n", i);
        // Case A: Built in function
        if( comtab[i].builtin != -1 ) {
            do_it(i);           // in file builtin.c
        }
        // Case B: Other commands
        else {
            printf("BEAST\n");
            printf("Cmd total: %d\n", curr_cmd);
            executeIt(i);
            printf("seg\n");
        }
    }
    printf("seg\n");
}

// ---------------------------------------------------------

int main() {

    int choice = 0;
    initShell();
    printf("===================================================================\n\n"); 
    // Loop as long as user does not decide to quit
    do {
        printf("\n[SHELL MASTER]: ");
        switch(choice = getCommand()) {
        case 0: printf("curr_cmd: %d\n", curr_cmd);
                processCommand();
            break;

        case 1: // Recover from error
                // Clear input buffer
            break;
        }
        printf("where is the seg\n");
    }while(!goodbye);
    printf("Exiting Shell Gracefully\n");
	return 0;
}
