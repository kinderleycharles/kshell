#include "shell.h"


// This function handles all the commands that are not builtin
void executeIt(int i) {
	if ( fork () == 0 )  {       /*  == 0 means in child  */
		printf("i: %d\trd: %d\t w: %d\n", i, comtab[i].iofd[0], comtab[i].iofd[1]);
		dup2(comtab[i].iofd[0], STDIN_FILENO);
		dup2(comtab[i].iofd[1], STDOUT_FILENO);
		execvp(comtab[i].atptr->args[0],comtab[i].atptr->args);
		exit (0);
	}
	else  {
		wait (0);
	}
}

void simple_exec(int index) {
	unsigned int status;
	if ( fork () == 0 )  {       /*  == 0 means in child  */
		execvp(comtab[index].atptr->args[0],comtab[index].atptr->args);
		exit (status);
	}
	else  {                      /* != 0 means in parent */
		wait (&status);
	}
}

void firstPipe() {
	unsigned int status;
	if( fork()==0 ) {
		dup2(comtab[0].iofd[0], STDIN_FILENO);
		dup2(STDOUT_FILENO, comtab[0].iofd[1]);
		execvp(comtab[0].atptr->args[0],comtab[0].atptr->args);
		exit (status);
	}
	else {
		wait(&status);
	}
}

void lastPipe() {
	unsigned int status;
	if( fork()==0 ) {
		dup2(comtab[curr_cmd].iofd[0], STDIN_FILENO);
		dup2(STDOUT_FILENO, comtab[curr_cmd].iofd[1]);
		execvp(comtab[curr_cmd].atptr->args[0],comtab[curr_cmd].atptr->args);
		exit (status);
	}
	else {
		wait(&status);
	}
}

void execPipe(int i) {
	if( fork()==0 ) {
		// Case A: First Pipe
		if(i == 0) {
			dup2(comtab[i].iofd[1], STDOUT_FILENO);
			execvp(comtab[i].atptr->args[0],comtab[curr_cmd].atptr->args);
			closePipes();
		}
		// Case B: Last Command
		else if (curr_cmd) {
			dup2(comtab[i-1].iofd[1], STDIN_FILENO);
			execvp(comtab[i].atptr->args[0],comtab[curr_cmd].atptr->args);
			closePipes();
		}
		// Case C: Middle of Pipe
		else {
			dup2(comtab[i-1].iofd[0], STDIN_FILENO);
			dup2(comtab[i].iofd[1], STDOUT_FILENO);
			closePipes();
		}
	} // End-if
}

// Piping commands function
// These functions will later be moved to piping.c file

void setPipeline() {
	// Make copy of the last pipe
	comtab[last_cmd].iofd[1] = comtab[curr_cmd].iofd[0];
}

void initPipes() {
	for(int i = 0; i != MAXCMDS; ++i) {
		pipe(comtab[i].iofd);
	}
}

void closePipes() {
	for(int i = 0; i != MAXCMDS; ++i) {
		close(comtab[i].iofd[0]);
		close(comtab[i].iofd[1]);
	}
}

// This function copy the content of one file descriptor to the
// next line with line with line assumed to be MAX at 300 char 
void writetoFile(int orig, int dest) {
	char line[300];
	int len = strlen(line);
	FILE* in = fdopen(orig, "r");
	while( fgets(line, sizeof(line), in) != NULL ) {
   		if (write(dest, line, len) != len)
		    printf("Failed to write to children");
   	}
   	fclose(in);			// Close input file
}

void setIredirection(char* filename) {
	printf("filename: %s\n", filename);
	int in;
	if ((in = open(filename,O_RDONLY|O_RDWR)) == -1) {
		printf("File could not open\n");
		return;
	}

	printf("in: %d\n", in);

	// Init first command pipes
	// pipe(comtab[0].iofd);
	dup2(in,comtab[0].iofd[0]);
	in_rd = 1;				// Set input redir flag true	
}

void setOredirection(char* filename) {

	printf("filename: %s\n", filename);
	// Create file descriptor for output file
	// Assuming this gets executed
	int out;
	if ( (out = open(filename,O_WRONLY|O_CREAT)) == -1 ) {
		printf("File could not open\n");
		return;
	}

	// Init first command pipes
	// pipe(comtab[curr_cmd].iofd);
	dup2(out,comtab[curr_cmd].iofd[1]);
	o_rd = 1;				// Set output redir flag true
}