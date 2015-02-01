/*  arrays.h
 *  Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *  -------------
 *
 *  Most of the character arrays are externalized in this file, so we don't have
 *  to pass them as function arguments to some of the screenfetch-c modules.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#ifndef SCREENFETCH_C_CHAR_ARRAYS_H
#define SCREENFETCH_C_CHAR_ARRAYS_H

#define MAX_STR 128

extern char *detected_arr[];
extern char *detected_arr_names[];
extern char host_str[MAX_STR];
extern char distro_str[MAX_STR];
extern char kernel_str[MAX_STR];
extern char uptime_str[MAX_STR];
extern char pkgs_str[MAX_STR];
extern char cpu_str[MAX_STR];
extern char gpu_str[MAX_STR];
extern char disk_str[MAX_STR];
extern char mem_str[MAX_STR];
extern char shell_str[MAX_STR];
extern char res_str[MAX_STR];
extern char de_str[MAX_STR];
extern char wm_str[MAX_STR];
extern char wm_theme_str[MAX_STR];
extern char gtk_str[MAX_STR];
extern char icon_str[MAX_STR];
extern char font_str[MAX_STR];
extern char UseR[MAX_STR];
extern char HosT[MAX_STR];
extern char *mult_shell_arr[][2];

#endif /* SCREENFETCH_C_CHAR_ARRAYS_H */