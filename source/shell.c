#include "shell.h"

void initShell( void );
void printPrompt( void );
void initScannerParser( void );
int getCommand( void );
int expandAlias( void );
void processCommand( void );
void recoverFromErrors( void );

int	main( int argc, const char* argv[] ) {
	
	// set up initializations
	initShell();
	
	// shell loop
	while( !goodbye ) {
		printPrompt();
		initScannerParser();
		if( !getCommand() ) {
			processCommand();
		}
		else {
			// error recovery from getCommand()
			recoverFromErrors();
		}
	}
	
	return 0;
}

void initShell() {
	int goodbye = 0;
}

void printPrompt() {
	//printf("ccsh[%s]:",getenv("PWD"));
	char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("ccsh[%s]:", cwd);
}

void initScannerParser() {
	COMMAND comtab[MAXCMDS];
	int background = 0;
	int errorfd = STDERR_FILENO;
	int npipes = 0;
	int append = 0;
}

int getCommand() {
	do {
		if( yyparse() )
			return understandErrors();
	} while( expandAlias() );
}

int expandAlias() {
	
}

void processCommand() {
	
}

void recoverFromErrors() {
	
}