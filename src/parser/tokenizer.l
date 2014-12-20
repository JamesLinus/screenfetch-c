/*	tokenizer.l
 *	Author: William Woodruff
 *	-------------
 *
 *	The (f)lex tokenizer used by screenfetch-c's configuration file parser.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

%{
#define YYSTYPE char *
#include <stdio.h>
#include <string.h>
#include "parser-defines.h"
%}

%option outfile="tokenizer.c" header-file="tokenizer.h"
%option nounput
%option noinput

%%
host	{ yylval = strdup(yytext); return DECLARATION; }
distro	{ yylval = strdup(yytext); return DECLARATION; }
kernel	{ yylval = strdup(yytext); return DECLARATION; }
architecture	{ yylval = strdup(yytext); return DECLARATION; }
cpu	{ yylval = strdup(yytext); return DECLARATION; }
gpu	{ yylval = strdup(yytext); return DECLARATION; }
shell	{ yylval = strdup(yytext); return DECLARATION; }
resolution	{ yylval = strdup(yytext); return DECLARATION; }
desktop_environment	{ yylval = strdup(yytext); return DECLARATION; }
window_manager	{ yylval = strdup(yytext); return DECLARATION; }
window_manager_theme	{ yylval = strdup(yytext); return DECLARATION; }
gtk_info	{ yylval = strdup(yytext); return DECLARATION; }
[a-zA-Z0-9]+[ ]*[a-zA-Z0-9]+	{ yylval = strdup(yytext); return DATA; }
\"	return QUOTE;
;	return SEMICOLON;
[#]+[a-zA-Z0-9 ]*	;
.	;
\n	;
%%