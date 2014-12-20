/*	parser.y
 *	Author: William Woodruff
 *	-------------
 *
 *	The yacc/bison parser generator used by screenfetch-c on ~/.screenfetchc.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */
%{
#define YYSTYPE char *
#include <stdio.h>
#include <stdlib.h>
#include "../disp.h"

extern int yyparse(char *name, char *value);
extern FILE *yyin;

void parse_config(const char *path)
{
	char *name, *value;
	FILE *f = fopen(path, "r");

	if (!f)
	{
		ERROR_OUT("Error: ", "Could not open the configuration file.");
		exit(-1);
	}
	yyin = f;

	do
	{
		yyparse(name, value);
		printf("Name: %s, value: %s\n", name, value);
	} while (!feof(f));

	// yyparse(name, value);
	// printf("Name: %s, value: %s\n", name, value);
	// yyparse(name, value);
	// printf("Name: %s, value: %s\n", name, value);

}

void yyerror(char *name, char *value, const char *msg)
{
	ERROR_OUT("Error: ", msg);
}

int yywrap()
{
	return 1;
}
%}

%output "parser.c"
%defines "parser-defines.h"
%parse-param {char *name} {char *value}
%define parse.error verbose
%token VARTOK PHRASE SEMICOLON QUOTE

%%
lines:
	| /* empty */
	lines line
	;

line:
	var_set SEMICOLON
	;

var_set:
	variable var_content
	{
		printf("Found token %s and word: %s\n", $1, $2);
		name = $1;
		value = $2;
	}
	;

variable:
	VARTOK
	{
		$$ = $1;
	}

var_content:
	QUOTE PHRASE QUOTE
	{
		$$ = $2;
	}
	;
%%
