/*  arrays.h
 *  Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *  -------------
 *
 *  Most of the character arrays are declared and initialized in this file,
 *  so we don't have to pass them as function arguments to some of the screenfetch-c modules.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/*#include "structs.h"
screenfetch_c ContaineR;*/

#include "colors.h"

#define MAX_STR 128

char host_str[MAX_STR]      = "Unknown";
char distro_str[MAX_STR]    = "Unknown";
char kernel_str[MAX_STR]    = "Unknown";
char uptime_str[MAX_STR]    = "Unknown";
char pkgs_str[MAX_STR]      = "Unknown";
char cpu_str[MAX_STR]       = "Unknown";
char gpu_str[MAX_STR]       = "Unknown";
char disk_str[MAX_STR]      = "Unknown";
char mem_str[MAX_STR]       = "Unknown";
char shell_str[MAX_STR]     = "Unknown";
char res_str[MAX_STR]       = "Unknown";
char de_str[MAX_STR]        = "Unknown";
char wm_str[MAX_STR]        = "Unknown";
char wm_theme_str[MAX_STR]  = "Unknown";
char gtk_str[MAX_STR]       = "Unknown";
char icon_str[MAX_STR]      = "Unknown";
char font_str[MAX_STR]      = "Unknown";

char UseR[MAX_STR], HosT[MAX_STR];

char *detected_arr[] =
{
    host_str,
    distro_str,
    kernel_str,
    cpu_str,
    gpu_str,
    shell_str,
    pkgs_str,
    disk_str,
    mem_str,
    uptime_str,
    res_str,
    de_str,
    wm_str,
    wm_theme_str,
    gtk_str,
    icon_str,
    font_str
};

char *detected_arr_names[] =
{
    "",
    "OS"TWHT":"TNRM" ",
    "Kernel"TWHT":"TNRM" ",
    "CPU"TWHT":"TNRM" ",
    "GPU"TWHT":"TNRM" ",
    "Shell"TWHT":"TNRM" ",
    "Packages"TWHT":"TNRM" ",
    "Disk"TWHT":"TNRM" ",
    "Memory"TWHT":"TNRM" ",
    "Uptime"TWHT":"TNRM" ",
    "Resolution"TWHT":"TNRM" ",
    "DE"TWHT":"TNRM" ",
    "WM"TWHT":"TNRM" ",
    "WM Theme"TWHT":"TNRM" ",
    "GTK"TWHT":"TNRM" ",
    "Icon Theme"TWHT":"TNRM" ",
    "Font"TWHT":"TNRM" "
};

/*  multidimensional array
    mult_shell_arr[x][0] - command to execute
    mult_shell_arr[x][1] - shell name
*/
char *mult_shell_arr[][2] =
{
    {"bash --version | head -1", "bash"},
    {"zsh --version", "zsh"},
    {"csh --version | head -1", "csh"},
    {"fish --version", "fish"}
};