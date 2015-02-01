/*	disp.c
 *	Author: William Woodruff
 *	-------------
 *  Edited by: Aaron Caffrey (trimmed 816 lines down to 155,
 *  while preserving the same screenfetch-c functionalities)
 *
 *	Functions used by screenfetch-c for displaying version
 *	and help output to the user.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* program includes */
#include "version.h"
#include "logos.h"
#include "colors.h"
#include "misc.h"
#include "disp.h"
#include "logos_length.h"
#include "arrays.h"
#include "flags.h"
#include "structs.h"

/*	display_version
	called if the -V (--version) flag is tripped
*/
void display_version(void)
{
	printf("%s\n", "screenfetch-c - Version " SCREENFETCH_C_VERSION_MAJOR "."
			SCREENFETCH_C_VERSION_MINOR ", revision "
			SCREENFETCH_C_VERSION_RELEASE ".");
	return;
}

/*	display_help
	called if the -h (--help) flag is tripped
*/
void display_help(void)
{
	display_version();
	printf("\n%s\n", "Options:\n"
			"  -v, --verbose\t\t\t Enable verbosity during output.\n"
			"  -s, --screenshot\t\t Take a screenshot.\n"
			"  -n, --no-logo\t\t\t Print output without a logo.\n"
			"  -D, --distro [DISTRO]\t\t Print output with DISTRO's logo.\n"
			"  -E, --suppress-errors\t\t Suppress error output.\n"
			"  -p, --portrait\t\t Print output in portrait mode.\n"
			"  -V, --version\t\t\t Output screenfetch-c's version and exit.\n"
			"  -h, --help\t\t\t Output this help information.\n"
			"  -L, --logo-only [DISTRO]\t Output only DISTRO's logo.\n"
			"For more information, consult screenfetch-c's man page.\n");

	return;
}

/* process_data
   print the detected and passed distro arguments
*/
void process_data(char *logo[], unsigned short int num1, unsigned short int num2, char *colour)
{
	unsigned short int x = 0;

	if (process_logo_only)
		for (x = 0; x < num1; x++)
			printf("%s\n", logo[x]);
	else
	{
		snprintf(host_str, MAX_STRLEN, "%s%s%s%s@%s%s%s%s",
			colour, UseR, TNRM, TWHT, TNRM, colour, HosT, TNRM);

		if (0 == num2)
			for (x = 0; x < num1; x++)
				printf("%s %s%s%s%s%s\n", logo[x], TNRM, colour,
					detected_arr_names[x], TNRM, detected_arr[x]);
		else
			for (x = 0; x < num1; x++)
				if (x < num2)
					printf("%s %s%s%s%s%s\n", logo[x], TNRM, colour,
						detected_arr_names[x], TNRM, detected_arr[x]);
				else
					printf("%s\n", logo[x]);
	}

	return;
}

/*	main_text_output
	the secondary output for screenfetch-c - all info WITHOUT ASCII art
	arguments bool verbose
*/
void main_text_output(bool verbose)
{
	unsigned short int i = 0;

	sprintf(host_str, "%s@%s", UseR, HosT);

	if (verbose)
		for (i = 0; i < 16; i++)
			VERBOSE_OUT(detected_arr_names[i], detected_arr[i]);
	else
		for (i = 0; i < 16; i++)
			printf("%s %s%s\n", detected_arr_names[i], TNRM, detected_arr[i]);

	return;
}

/*	main_ascii_output
	the primary output for screenfetch-c
*/
void main_ascii_output(char *passed_distro)
{
	bool distro_match = false;
	unsigned short int x = 0;
	char distro[MAX_STRLEN];

	process_logo_only = (STREQ(passed_distro, "") ? false : true);

	sprintf(distro, "%s", (STREQ(passed_distro, "") ? detected_arr[1] : passed_distro));

	for (x = 0; x < 36; x++)
		if (STREQ(distro, opt[x].distro))
		{
			distro_match = true;
			break;
		}

	if (distro_match)
		process_data(opt[x].logo, opt[x].len, opt[x].break_point, opt[x].colour);

	else
	{
		if (strstr(distro, "Microsoft"))
			process_data(windows_logo, MICROSOFT_LEN, 0, TRED);

		else if (strstr(distro, "OS X"))
			process_data(macosx_logo, MACOSX_LEN, 0, TLBL);

		else if (STREQ(distro, "Ubuntu") || STREQ(distro, "Lubuntu")
				|| STREQ(distro, "Xubuntu"))
			process_data(ubuntu_logo, UBUNTU_LEN, 16, TLRD);

		else if (STREQ(distro, "Mandriva") || STREQ(distro, "Mandrake"))
			process_data(mandriva_mandrake_logo, MANDRAKE_LEN, 16, TLBL);

		else
			ERR_REPORT("Could not find a logo for the distro.");
	}

	return;
}
