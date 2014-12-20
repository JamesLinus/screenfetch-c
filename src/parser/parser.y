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
	char name[128], value[128];
	yyin = fopen(path, "r");

	if (!yyin)
	{
		ERROR_OUT("Error: ", "Could not open the configuration file.");
		exit(-1);
	}

	do
	{
		yyparse(name, value);
		printf("Name: %s, value: %s\n", name, value);
	} while (!feof(yyin));

	fclose(yyin);

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
%token DECLARATION DATA SEMICOLON QUOTE

%%
lines:
	|	/* empty */
	lines line
	;

line:
	declaration SEMICOLON
	;

declaration:
	variable content
	{
		printf("Found variable %s and content: %s\n", $1, $2);
		strncpy(name, $1, 128);
		strncpy(value, $2, 128);
		free($1);
		free($2);
	}
	;

variable:
	DECLARATION
	{
		$$ = $1;
	}

content:
	QUOTE DATA QUOTE
	{
		$$ = $2;
	}
	;
%%
