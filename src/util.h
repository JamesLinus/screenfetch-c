/*	util.h
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	Function prototypes for util.c.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#ifndef SCREENFETCH_C_UTIL_H
#define SCREENFETCH_C_UTIL_H

void popen_raw_shell_version(char *str1, char *str2);
void remove_excess_cpu_txt(char *str1);
char *safe_strncpy(char *destination, const char *source, size_t num);
void split_uptime(long uptime, unsigned int *secs, unsigned int *mins, unsigned int *hrs, unsigned int *days);
void take_screenshot(bool verbose, char *monitor_res);

#endif /* SCREENFETCH_C_UTIL_H */
