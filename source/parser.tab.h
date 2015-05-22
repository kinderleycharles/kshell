/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     WORD = 258,
     STRING = 259,
     NEWLINE = 260,
     TAB = 261,
     CDIR = 262,
     SENV = 263,
     PENV = 264,
     USETENV = 265,
     ALIAS = 266,
     UALIAS = 267,
     QUIT = 268,
     IREDIR = 269,
     OREDIR = 270,
     GREATER = 271,
     PIPE = 272,
     BSLASH = 273,
     NPERCENT = 274,
     EQUAL = 275,
     SQUOTE = 276
   };
#endif
/* Tokens.  */
#define WORD 258
#define STRING 259
#define NEWLINE 260
#define TAB 261
#define CDIR 262
#define SENV 263
#define PENV 264
#define USETENV 265
#define ALIAS 266
#define UALIAS 267
#define QUIT 268
#define IREDIR 269
#define OREDIR 270
#define GREATER 271
#define PIPE 272
#define BSLASH 273
#define NPERCENT 274
#define EQUAL 275
#define SQUOTE 276




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 13 "parser.y"
{
    int i;
    char* str;
}
/* Line 1529 of yacc.c.  */
#line 96 "parser.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

