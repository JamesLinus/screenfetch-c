%{
#define YYSTYPE char *
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
%}

%option nounput
%option noinput

%%
host	{ yylval = strdup(yytext); return VARTOK; }
distro	{ yylval = strdup(yytext); return VARTOK; }
kernel	{ yylval = strdup(yytext); return VARTOK; }
architecture	{ yylval = strdup(yytext); return VARTOK; }
cpu	{ yylval = strdup(yytext); return VARTOK; }
gpu	{ yylval = strdup(yytext); return VARTOK; }
shell	{ yylval = strdup(yytext); return VARTOK; }
resolution	{ yylval = strdup(yytext); return VARTOK; }
desktop_environment	{ yylval = strdup(yytext); return VARTOK; }
window_manager	{ yylval = strdup(yytext); return VARTOK; }
window_manager_theme	{ yylval = strdup(yytext); return VARTOK; }
gtk_info	{ yylval = strdup(yytext); return VARTOK; }
\"[a-zA-Z0-9 ]+\"	{ yylval = strdup(yytext); return PHRASE; }
;	return SEMICOLON;
.	;
\n	;
%%