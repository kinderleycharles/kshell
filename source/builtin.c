#include "shell.h"
#include "string.h"

// Change the current directory to the home directory
void goHome() {
	SHELL_PWD = getenv("HOME");
	if( chdir(SHELL_PWD) ) { printf("%s\n", strerror(errno)); }
}

// Change the current directory to the user directory
void gotoDir(int c) {
	char *tmp = strcat(strcat(SHELL_PWD,"/"),comtab[c].atptr->args[1]);
	if( chdir(tmp) ) { printf("%s\n", strerror(errno)); }
}

void printallEnv() {
	int i = 0;
	while(environ[i]) {
	  printf("%s\n", environ[i++]);
	}
}
// This function execute builtin functions
void do_it(int bi) {
	
	switch(comtab[bi].builtin) {
		case BYE: goodbye = 1;	// Set global goodbye variable to true
			break;

		case CDHOME: 
				goHome();
			break;

		case CD: 	// Go to specified path
				gotoDir(bi);
			break;

		case SETALIAS: // Adding alias to the alias table
				printf("adding alias to table\n");
				createAlias(comtab[curr_cmd].atptr->args[0], comtab[curr_cmd].atptr->args[1]);
				printf("Done adding alias to table\n");
			break;

		case UNALIAS:
				removeAlias(comtab[curr_cmd].atptr->args[0]);
			break;

		case PRINTALIAS:	// Print either a specific alias or all aliases
				if( comtab[curr_cmd].nargs == 0) {
					printf("calling\n");
					listAliases();		// Print all aliases
				}
				else {
					printAnAlias(comtab[curr_cmd].atptr->args[0]);	// Print specific alias
				}		
			break;

		case SETENV: 	// Set environment
				if( putenv( strcat(strcat(comtab[curr_cmd].atptr->args[0],"="),comtab[curr_cmd].atptr->args[1]) ) != 0) {
					printf("%s\n", strerror(errno));
				}
			break;
		case UNSETENV: 	// Removing environment variables
			if( unsetenv(comtab[bi].atptr->args[0]) == -1) {
				printf("%s\n", strerror(errno));
			}
			break;
		case PRINTENV: 	// Print environment variables
			printallEnv();
			break;
	}
}