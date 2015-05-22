# We will create variables just in case different
# compilers are preferred
LEX = flex
YACC = bison -d
CC = cc
CFLAGS = -c

all: clean parser.tab.c parser.tab.h lex.yy.c shell run

# The yacc file will generate 2files .h and .c files named y.tab.c and  y.tab.h
parser.tab.c parser.tab.h: parser.y
	$(YACC) -v parser.y

# The lex file will generate a .c file name lex.yy.c
lex.yy.c: tokenizer.l
	$(LEX) tokenizer.l

###############################################################

shell: shell.h
	$(CC) main.c parser.tab.c lex.yy.c alias.c builtin.c execute.c -o shell -ll

run:
	./shell

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h parser.output lex.yy.o parser.tab.o alias.o main.o lexYacc.o shell 
