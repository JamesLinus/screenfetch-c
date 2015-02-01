/*	disp.h
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	Function prototypes for disp.c and macros for error/verbose outut.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 *	You should have received a copy of it with this code.
 */

#ifndef SCREENFETCH_C_DISP_H
#define SCREENFETCH_C_DISP_H

#include <stdbool.h>
#include "colors.h"

void display_version(void);
void display_help(void);
void process_data(char *logo[], unsigned short int num1, unsigned short int num2, char *colour);
void main_text_output(bool verbose);
void main_ascii_output(char *passed_distro);

#define ERR_REPORT(str) (fprintf(stderr, TWHT "[[ " TLRD "!" TWHT " ]] " TNRM "Error: %s\n", str))
#define VERBOSE_OUT(str1, str2) (fprintf(stdout, TLRD ":: " TNRM "%s%s\n", str1, str2))

#endif /* SCREENFETCH_C_DISP_H */
