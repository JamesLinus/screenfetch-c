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
extern int yylex();
extern int yyparse();
extern FILE *yyin;

char *str1, *str2;

void parse_config(const char *path)
{
	FILE *f = fopen(path, "r");

	if (!f)
	{
		ERROR_OUT("Error: ", "Could not open the configuration file.");
		exit(-1);
	}
	yyin = f;
	yyparse();
}

void yyerror(const char *str)
{
	fprintf(stderr, "error: %s\n", str);
}

int yywrap()
{
	return 1;
}
%}

%output "parser.c"
%defines "parser-defines.h"
%define parse.error verbose
%token VARTOK PHRASE SEMICOLON

%%
commands: commands command
		| command
		;

command:
		var_set SEMICOLON
		;

var_set:
		variable var_content
		{
			str1 = $1;
			str2 = $2;
			printf("Found token %s and word: %s\n", $1, $2);
		}
		;

variable:
		VARTOK
		{
			$$ = $1;
		}

var_content:
		PHRASE
		{
			$$ = $1;
		}
		;
%%
