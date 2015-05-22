%{
#include <stdio.h>
#include <string.h>
#include "shell.h"

void yyerror(const char *str)
{
        printf(stderr,"error: %s\n",str);
}

%}

%union{
    int i;
    char* str;
}

%token <str> WORD
%token <str> STRING

%token NEWLINE TAB CDIR SENV PENV USETENV ALIAS UALIAS QUIT IREDIR OREDIR GREATER PIPE BSLASH NPERCENT EQUAL SQUOTE


%%
commands: /* empty */
        | commands command
        ;

command: error_parsing
        |
        builtin_cmd
        |
        simple_cmd
        |
        complex_cmd
        |
        piping
        |
        next_line
        ;

error_parsing:
        piping NEWLINE { printf("Piping Syntax error\n"); return 1; }
        ;


builtin_cmd: 
        exit_shell
        |
        change_dir
        |
        set_var
        |
        unset_var
        |
        printenv_var
        |
        add_alias
        |
        rm_alias
        |
        print_alias
        ;

simple_cmd:
        WORD
                {   printf("SC: \n", $1);
                    // Allocate argument if does not already exist
                    if( (p = &comtab[curr_cmd])->atptr == NULL ) {
                        p->atptr = Allocate(ARGTAB);
                    }
                    copystring( (p->atptr->args[0]), $1);
                    p->comname = p->atptr->args[0];
                }
        | WORD arguments
                {   printf("SC ARGS: %s: ARGS\n",$1);
                    // Allocate argument if does not already exist
                    copystring( (p->atptr->args[0]), $1);
                    p->comname = p->atptr->args[0];
                }

complex_cmd:
        input_redirection
        |
        output_redirection
        ;


piping: piping
        | simple_cmd PIPE 
                {   printf("piping\n");
                    // testingPrintComTab();
                    last_cmd = curr_cmd;        // Keep track of last command executed   
                    ++curr_cmd;                 // Increment currcommand to the next one
                                                // Create file descriptor
                                                // Set output to file descriptor
                    setPipeline();            // Linked the pipe
                    printf("curr_cmd = %d\n", curr_cmd);
                }
        | builtin_cmd PIPE 
                {   printf("piping\n");
                    // testingPrintComTab();
                    last_cmd = curr_cmd;        // Keep track of last command executed   
                    ++curr_cmd;                 // Increment currcommand to the next one
                                                // Create file descriptor
                                                // Set output to file descriptor
                    setPipeline();            // Linked the pipe
                    printf("curr_cmd = %d\n", curr_cmd);
                }


input_redirection:
        IREDIR WORD
                {
                    printf("Input redirection\n");
                    setIredirection($2);
                }

output_redirection:
        OREDIR WORD
                {
                    printf("output redirection\n");
                    setOredirection($2);
                }

arguments:
        WORD
                {   currarg = 1;        // Reserve the first arg for comname
                    printf("WORD: %s\t curr_cmd: %d\n", $1, curr_cmd);
                    if( (p = &comtab[curr_cmd])->atptr == NULL ) {
                        p->atptr = Allocate(ARGTAB);
                    }
                    copystring( (p->atptr->args[currarg++]), $1);
                    ++p->nargs;
                }
        | STRING
                {   currarg = 1;        // Reserve the first arg for comname
                    printf("STRING WORD: %s\n", $1);
                    if( (p = &comtab[curr_cmd])->atptr == NULL ) {
                        (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB);
                    }
                    copystring( (p->atptr->args[currarg++]), $1);
                    ++p->nargs;
                }
        | arguments WORD
                {
                    printf("WORD: %s\n", $2);
                    copystring( (p->atptr->args[currarg++]), $2);
                    ++p->nargs;
                }
        | arguments STRING
                {
                    printf("STRING WORD: %s\n", $2);
                    copystring( (p->atptr->args[currarg++]), $2);
                    ++p->nargs;
                }

next_line:
        NEWLINE
        {
            return 0;
        }

exit_shell:
        QUIT
                {
                    comtab[curr_cmd].builtin = BYE; 
                }

change_dir:
        CDIR      
                {   // Allocate argument if does not already exist
                    if( (p = &comtab[curr_cmd])->atptr == NULL ) {
                        p->atptr = Allocate(ARGTAB);
                    }
                    ++comtab[curr_cmd].nargs;       // First slot of arg is reserver to args name
                    printf("Change dir: NO ARGS\n");
                    comtab[curr_cmd].builtin = CDHOME; 
                }
        | CDIR arguments 
                {   ++comtab[curr_cmd].nargs;       // First slot of arg is reserver to args name
                    printf("Change dir: ARGS\n");
                    comtab[curr_cmd].builtin = CD;
                }

set_var:
        SENV WORD WORD 
                {
                    printf("Changing directory\n");
                    (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB);
                    copystring( p->atptr->args[0], $2);
                    copystring( p->atptr->args[1], $3);
                    comtab[curr_cmd].builtin = SETENV;
                    comtab[curr_cmd].nargs += 1;
                }
                | SENV WORD STRING 
                {
                    printf("Changing directory\n");
                    (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB);
                    copystring( p->atptr->args[0], $2);
                    copystring( p->atptr->args[1], $3);
                    comtab[curr_cmd].builtin = SETENV;
                    comtab[curr_cmd].nargs += 1;
                }

unset_var:
        USETENV WORD
                {
                    printf("Changing directory\n");
                    (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB);
                    copystring( p->atptr->args[0], $2);
                    comtab[curr_cmd].builtin = UNSETENV;
                    comtab[curr_cmd].nargs += 1;
                }

printenv_var:
        PENV
        {
            (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB);
            comtab[curr_cmd].builtin = PRINTENV;
        }

add_alias:
        ALIAS WORD WORD
                {   // First argument is the name of the alias
                    printf("Adding alias: %s\n", $2);
                    comtab[curr_cmd].builtin = SETALIAS;
                    (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB); 
                    copystring( p->atptr->args[0], $2);
                    copystring( p->atptr->args[1], $3);
                    comtab[curr_cmd].comname = p->atptr->args[0];
                    comtab[curr_cmd].nargs += 2;
                }
        | ALIAS WORD STRING
                {   // First argument is the name of the alias
                    printf("Adding alias: %s\n", $2);
                    comtab[curr_cmd].builtin = SETALIAS;
                    (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB); 
                    copystring( p->atptr->args[0], $2);
                    copystring( p->atptr->args[1], $3);
                    comtab[curr_cmd].comname = p->atptr->args[0];
                    comtab[curr_cmd].nargs += 2;
                }

rm_alias:
        UALIAS WORD
                {   // First argument is the name of the alias
                    printf("Removing alias: %s\n", $2);
                    comtab[curr_cmd].builtin = UNALIAS; 
                    (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB); 
                    copystring( p->atptr->args[0], $2);
                    ++comtab[curr_cmd].nargs;
                }

print_alias:
        ALIAS
                {   // First argument denote which alias to print
                    printf("Printing all aliases\n");
                    printf("curr_cmd: %d\n", curr_cmd);
                    comtab[curr_cmd].builtin = PRINTALIAS;
                    comtab[curr_cmd].nargs = 0;
                }
        | ALIAS WORD
                {   // First argument denote which alias to print
                    printf("Printing WORD alias: %s\n", $2);
                    comtab[curr_cmd].builtin = PRINTALIAS;
                    (p = &comtab[curr_cmd])->atptr = Allocate(ARGTAB); 
                    copystring( p->atptr->args[0], $2);
                    comtab[curr_cmd].comname = p->atptr->args[0];
                    ++comtab[curr_cmd].nargs; 
                }
%%

