/* Including other header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>         /* defines options flags */
#include <sys/types.h>     /* defines types used by sys/stat.h */
#include <sys/stat.h>      /* defines S_IREAD & S_IWRITE  */
#include <unistd.h>
#include <errno.h>
#include <limits.h>

/* System bounds */
#define	MAXCMDS	10		// Should be 50 but for testing I'll do 10
#define	MAXARGS	300
#define MAXPATH 200
#define MAX_ALIAS 100


/* Define statements for built-ins */
#define	BYE			1
#define	CD			2
#define	CDHOME		3
#define	PRINTALIAS	4
#define	SETALIAS	5
#define UNALIAS		6
#define	PRINTENV	7
#define	SETENV		8
#define	UNSETENV	9

/* */
#define mkstr(a) (char *)makestring(a)
#define copystring(a,b) strcpy((a=(char *)malloc(strlen(b)+1)),b)
#define Allocate(t) (t *)malloc(sizeof(t))


/* Define statements for errors */

/* Typedefs */
typedef	struct	comargs	{
	char	*args[MAXARGS];
}	ARGTAB;

typedef	struct	com	{
	char	*comname;
	int		builtin;
	int		iofd[2];			// I/O file descriptor pipes
	int		nargs;
	ARGTAB	*atptr;
} COMMAND;

/* Alias structure */
struct aliasNode {
	char *name;\
	char *cmd;
	int valid;	
};

/* Command description */
extern	COMMAND	comtab[];	// defined in initShell()
extern	ARGTAB arguments[];	// defined in initShell()
extern	struct aliasNode aliastab[];	// defined in initAlias()
extern	int	background;
extern	int	errorfd;
extern	int	npipes;
extern	int	append;

/* Variables/flags */
int goodbye;			// defined in initShell()
int curr_cmd;			// Keep tracks of what command is being executed
int currarg;			// Pointing to the current argument (Used in Yacc)
int last_cmd;			// Keep track of last commad executed
int inf_alias;			// Infinite alias expansion (Set -1 initially)
int in_rd;				// Flag to set input redirectory
int o_rd;				// Flag to set input redirectory
COMMAND* p;				// A pointer to keep the code clean
char* SHELL_PWD;		// Store the current path of the program


/* Function prototypes */

/* in file parser.y */
extern void redFlexinput(char *); 


/* in file main.c */
extern void expand();
extern int expansion_check(char*);
void cmd2str(int, char*);

/* in file alias.c */ 
extern void initAlias();
extern void createAlias(char *aName, char *aCmd);
extern int searchAlias(char *aName);
extern void listAliases();
extern void remAlias(int a_id);
extern void removeAlias(char *aName);
extern void printAnAlias(char *al);

/* in file builtin.c */ 
extern void do_it(int);

/* in file execute.c */ 
extern void executeIt(int);
extern void initPipes();
extern void closePipes();
extern void setPipeline();

extern void setIredirection();
extern void setOredirection();
extern void simple_exec(int);
extern void execPipe(int i);
extern void firstPipe();
extern void lastPipe();

/* Special Case Function */
extern char **environ;
