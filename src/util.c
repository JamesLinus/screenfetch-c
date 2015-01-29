/*	util.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	Utility functions used by screenfetch-c.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>

/* program includes */
#include "disp.h"
#include "misc.h"
#include "flags.h"
#include "arrays.h"
#include "util.h"

#if defined(__CYGWIN__)
	#include <Windows.h>
#endif

/*	popen_raw_shell_version
	get the raw shell version line and workaround
	the problem when the shell version has:
	GNU bash, version x.x.x(x)-release (x86_64-unknown-linux-gnu)
	Even if the shell version is x.x.xx(x) it will be handled properly
*/
void popen_raw_shell_version(char *str1, char *str2)
{
	FILE *shell_file;
	char vers_arr[MAX_STRLEN], temp_arr[MAX_STRLEN];
	unsigned short int x = 0, z = 0;

	shell_file = popen(str1, "r");
	fgets(vers_arr, MAX_STRLEN, shell_file);
	pclose(shell_file);

	size_t str_len = strlen(vers_arr);

	for (z = 0, x = 0; x < str_len; x++)
	{
		if (isdigit(vers_arr[x]) || vers_arr[x] == '.') /* ispunct() includes comma */
			temp_arr[z++] = vers_arr[x];

		if (isspace(vers_arr[x]) && z > 0)
			break;
	}

	if (STREQ(str2, "bash"))
		temp_arr[z-1] = '\0'; /* we don't want (x) in x.x.x(x) */
	else
		temp_arr[z] = '\0';

	snprintf(shell_str, MAX_STRLEN, "%s %s", str2, temp_arr);

}

/*	remove_excess_cpu_txt
	Remove unnecessary cpu text like: (tm), (TM), Processor
	AMD Athlon(tm) II X3 455 Processor -> AMD Athlon II X3 455
*/
void remove_excess_cpu_txt(char *str1)
{
	if (!(STREQ(str1, "Unknown")) && !(STREQ(str1, "BCM2708 (Raspberry Pi)")))
	{

		unsigned short int x = 0, z = 0;
		const char tokseps[] = " ()"; /* (TM), (tm) */
		char temp_arr[MAX_STRLEN];
		char *sep_str = strtok(str1, tokseps);

		do
		{
			if (!(STREQ(sep_str, "Processor")) && !(STREQ(sep_str, "tm"))
				&& !(STREQ(sep_str, "TM")))
			{
				if (z > 0)
					temp_arr[z++] = ' ';

				for (x = 0; x < strlen(sep_str); x++)
					temp_arr[z++] = sep_str[x];
			}
		} while ((sep_str = strtok(NULL, tokseps)) != NULL);

		temp_arr[z] = '\0';
		snprintf(cpu_str, MAX_STRLEN, "%s", temp_arr);
	}
}

/*	safe_strncpy
	calls strncpy with the given params, then inserts a terminating NULL
	returns a pointer to a string containing the copied data
*/
char *safe_strncpy(char *destination, const char *source, size_t num)
{
	char *ret = strncpy(destination, source, num);
	ret[num - 1] = '\0';
	return ret;
}

/*	split_uptime
	splits param uptime into individual time-units
	argument long uptime: the uptime, in seconds, to be split
	arguments int *secs..*days: pointers to unsigned ints
	for storing the uptime
	--
	CAVEAT: uptime MUST be in seconds
	--
*/
void split_uptime(long uptime, unsigned int *secs, unsigned int *mins, unsigned int *hrs, unsigned int *days)
{
	*secs = (unsigned int) uptime % 60;
	*mins = (unsigned int) (uptime / 60) % 60;
	*hrs = (unsigned int) (uptime / 3600) % 24;
	*days = (unsigned int) (uptime / 86400);

	return;
}

/*	take_screenshot
	takes a screenshot and saves it to $HOME/screenfetch_screenshot.png
*/
void take_screenshot(bool verbose, char *monitor_res)
{
#if !defined(__CYGWIN__)
	int call_status = 1;
	char file_loc[MAX_STRLEN+MAX_STRLEN];
#endif

    char time_str[MAX_STRLEN], shot_location[MAX_STRLEN], shot_str[MAX_STRLEN+MAX_STRLEN];
    time_t t = time(NULL);

    strftime(time_str, MAX_STRLEN, "%Y-%m-%d-%H%M%S", localtime(&t));

    sprintf(shot_location, "/screenfetch-%s_%s.png", time_str, monitor_res);

	printf("%s", "Taking shot in 3..");
	fflush(stdout);
	sleep(1);
	printf("%s", "2..");
	fflush(stdout);
	sleep(1);
	printf("%s", "1..");
	fflush(stdout);
	sleep(1);
	printf("%s\n", "0");

#if defined(__CYGWIN__)
	/* terrible hack, the printscreen key is simulated */
	keybd_event(VK_SNAPSHOT, 0, 0, 0);

	if (verbose)
		VERBOSE_OUT("Screenshot has been saved to the clipboard.", "");

	/* NOT FINSISHED - HBITMAP needs to be saved
	HDC screen_dc = GetDC(NULL);
	HDC mem_dc = CreateCompatibleDC(screen_dc);

	int horiz = GetDeviceCaps(screen_dc, HORZRES);
	int vert = GetDeviceCaps(screen_dc, VERTRES);

	HBITMAP bitmap = CreateCompatibleBitmap(screen_dc, horiz, vert);
	HBITMAP old_bitmap = SelectObject(mem_dc, bitmap);

	BitBlt(mem_dc, 0, 0, horiz, vert, screen_dc, 0, 0, SRCCOPY);
	bitmap = SelectObject(mem_dc, old_bitmap);

	DeleteDC(screen_dc);
	DeleteDC(mem_dc);
	*/
#elif defined(__APPLE__) && defined(__MACH__)
		sprintf(shot_str, "screencapture -x ~%s 2> /dev/null", shot_location);
#else
		sprintf(shot_str, "scrot ~%s 2> /dev/null", shot_location);
#endif

	call_status = system(shot_str);

#if !defined(__CYGWIN__)
	safe_strncpy(file_loc, getenv("HOME"), MAX_STRLEN);
	strncat(file_loc, shot_location, MAX_STRLEN);

	if (FILE_EXISTS(file_loc) && verbose)
	{
		VERBOSE_OUT("Screenshot successfully saved.", "");
	}
	else if (call_status && error)
	{
		ERR_REPORT("Problem saving screenshot.");
	}
#endif

	return;
}

/*  manual_input
	generates (or reads) the ~/.screenfetchc file based upon user input
	returns an int indicating status (SUCCESS or FAILURE)
*/
int manual_input(char **data, bool verbose)
{
	FILE *config_file;
	char config_file_loc[MAX_STRLEN];

	safe_strncpy(config_file_loc, getenv("HOME"), MAX_STRLEN);
	strncat(config_file_loc, "/.screenfetchc", MAX_STRLEN);

	if (!FILE_EXISTS(config_file_loc))
	{
		#if defined(__CYGWIN__)
			printf("%s\n", TBLU "WARNING: There is currenly a bug involving manual mode on Windows." TNRM);
			printf("%s\n", TBLU "Only continue if you are ABSOLUTELY sure." TNRM);
		#endif

		printf("%s\n", "This appears to be your first time running screenfetch-c in manual mode.");
		printf("%s", "Would you like to continue? (y/n) ");

		char choice = getchar();
		getchar(); /* flush the newline */

		if (choice == 'y' || choice == 'Y')
		{
			char input[MAX_STRLEN];
			config_file = fopen(config_file_loc, "w");

			printf("%s\n", "We are now going to begin the manual mode input process.");
			printf("%s\n", "Please enter exactly what is asked for.");
			printf("%s\n", "If you are unsure about format, please consult the manpage.");

			printf("%s", "Please enter the name of your distribution/OS: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your architecture: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your username@hostname: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your kernel: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your CPU name: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your GPU name: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your shell name and version: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your monitor resolution: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your DE name: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your WM name: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter your WM Theme name: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s", "Please enter any GTK info: ");
			fgets(input, MAX_STRLEN, stdin);
			fprintf(config_file, "%s", input);

			printf("%s\n", "That concludes the manual input.");
			printf("%s\n", "From now on, screenfetch-c will use this information when called with -m.");

			fclose(config_file);
		}
		else
		{
			printf("%s\n", "Exiting manual mode and screenfetch-c.");
			printf("%s\n", "If you wish to run screenfetch-c normally, do not use the -m (--manual) flag next time.");
		}
		
		return EXIT_FAILURE;
	}
	else
	{
		if (verbose)
			VERBOSE_OUT("Found config file. Reading...", "");

		config_file = fopen(config_file_loc, "r");

		/*	store anything without a newline it it,
			then swallow any whitespace characters (" ").
		 */
		fscanf(config_file, "%[^\n] ", data[1]);
		fscanf(config_file, "%[^\n] ", data[3]);
		fscanf(config_file, "%[^\n] ", data[0]);
		fscanf(config_file, "%[^\n] ", data[2]);
		fscanf(config_file, "%[^\n] ", data[4]);
		fscanf(config_file, "%[^\n] ", data[5]);
		fscanf(config_file, "%[^\n] ", data[6]);
		fscanf(config_file, "%[^\n] ", data[11]);
		fscanf(config_file, "%[^\n] ", data[12]);
		fscanf(config_file, "%[^\n] ", data[13]);
		fscanf(config_file, "%[^\n] ", data[14]);
		fscanf(config_file, "%[^\n] ", data[15]);

		fclose(config_file);

		return EXIT_SUCCESS;
	}
}