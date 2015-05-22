%{
#include <stdio.h>
#include "shell.h"
#include "parser.tab.h"
%}

%%
" "							/*ignore white space*/
\n							return NEWLINE;
\t							return TAB;

cd							return CDIR;
setenv						return SENV;
printenv					return PENV;
unsetenv					return USETENV;
alias						return ALIAS;
unalias						return UALIAS;
bye							return QUIT;

[^ \=\"\<\>\|\&\\\t\n]+	    {
								yylval.str = strdup(yytext);
								return WORD;
							}

\".*\"						{
								yylval.str = calloc(strlen(yytext)-1, sizeof(char));
								 // you copy the string
								 strncpy(yylval.str, &yytext[1], strlen(yytext-2));
								 // you set the NULL terminating at the end
								 yylval.str[strlen(yytext)-2] = '\0';
								return STRING;
							}

\"							return SQUOTE;
\<							return IREDIR;
\>							return OREDIR;
\|							return PIPE;
\\							return BSLASH;
\&							return NPERCENT;
\=							return EQUAL;

%%

void init_lex_parser() {
	YY_FLUSH_BUFFER;		// Flush any remaining tokens out 
}

void redFlexinput(char* filename) {
    YY_BUFFER_STATE my_string_buffer = yy_scan_string(filename); 
    yy_switch_to_buffer( my_string_buffer ); // switch flex to the buffer we just created
    yyparse(); 
    yy_delete_buffer(my_string_buffer );
}