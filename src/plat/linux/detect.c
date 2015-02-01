/*	detect.c
 *	Author: William Woodruff
 *  Edited by: Aaron Caffrey
 *	-------------
 *
 *	The detection functions used by screenfetch-c on Linux are implemented here.
 *	Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>

/* linux-specific includes */
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <pwd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <glob.h>

/* program includes */
#include "../../misc.h"
#include "../../disp.h"
#include "../../util.h"
#include "../../flags.h"
#include "../../arrays.h"

/*	detect_distro
	detects the computer's distribution (really only relevant on Linux)
	argument char *str1: the char array to be filled with the distro name
*/
void detect_distro(char *str1)
{
	/* if distro_str was NOT set by the -D flag */
	if (STREQ(str1, "Unknown"))
	{
		FILE *distro_file;

		char distro_name_str[MAX_STRLEN];

		if (FILE_EXISTS("/system/bin/getprop"))
			safe_strncpy(str1, "Android", MAX_STRLEN);

		else
		{
			bool detected = false;

			/* Bad solution, as /etc/issue contains junk on some distros */
			distro_file = fopen("/etc/issue", "r");

			if (distro_file != NULL)
			{
				/* get the first 4 chars, that's all we need */
				fscanf(distro_file, "%4s", distro_name_str);
				fclose(distro_file);

				if (STREQ(distro_name_str, "Kali"))
				{
					safe_strncpy(str1, "Kali Linux", MAX_STRLEN);
					detected = true;
				}
				else if (STREQ(distro_name_str, "Back"))
				{
					safe_strncpy(str1, "Backtrack Linux", MAX_STRLEN);
					detected = true;
				}
				else if (STREQ(distro_name_str, "Crun"))
				{
					safe_strncpy(str1, "CrunchBang", MAX_STRLEN);
					detected = true;
				}
				else if (STREQ(distro_name_str, "LMDE"))
				{
					safe_strncpy(str1, "LMDE", MAX_STRLEN);
					detected = true;
				}
				else if (STREQ(distro_name_str, "Debi")
						|| STREQ(distro_name_str, "Rasp"))
				{
					safe_strncpy(str1, "Debian", MAX_STRLEN);
					detected = true;
				}
			}

			if (!detected)
			{
				if (FILE_EXISTS("/etc/fedora-release"))
					safe_strncpy(str1, "Fedora", MAX_STRLEN);

				else if (FILE_EXISTS("/etc/SuSE-release"))
					safe_strncpy(str1, "OpenSUSE", MAX_STRLEN);

				else if (FILE_EXISTS("/etc/arch-release"))
					safe_strncpy(str1, "Arch Linux", MAX_STRLEN);

				else if (FILE_EXISTS("/etc/gentoo-release"))
					safe_strncpy(str1, "Gentoo", MAX_STRLEN);

				else if (FILE_EXISTS("/etc/angstrom-version"))
					safe_strncpy(str1, "Angstrom", MAX_STRLEN);

				else if (FILE_EXISTS("/etc/manjaro-release"))
					safe_strncpy(str1, "Manjaro", MAX_STRLEN);

				else if (FILE_EXISTS("/etc/lsb-release"))
				{
					distro_file = fopen("/etc/lsb-release", "r");
					fscanf(distro_file, "%s ", distro_name_str);
					fclose(distro_file);

					snprintf(str1, MAX_STRLEN, "%s", distro_name_str + 11);
				}
				else if (FILE_EXISTS("/etc/os-release"))
				{
					/*
						TODO: Parse NAME or PRETTY_NAME from os-release
						Until then, spit out an error message.
					*/
					if (error)
						ERR_REPORT("Failed to detect a Linux distro (1).");
				}
				else
				{
					safe_strncpy(str1, "Linux", MAX_STRLEN);

					if (error)
						ERR_REPORT("Failed to detect a Linux distro (2).");
				}
			}
		}
	}

	return;
}

/*	detect_arch
	detects the computer's architecture
	argument char *str: the char array to be filled with the architecture
*/
/*void detect_arch(char *str)
{
	struct utsname arch_info;
	uname(&arch_info);
	safe_strncpy(str, arch_info.machine, MAX_STRLEN);

	return;
}*/

/*	detect_host
	detects the computer's hostname and active user and formats them
*/
void detect_host(void)
{
	char given_user[MAX_STRLEN] = "Unknown";
	char given_host[MAX_STRLEN] = "Unknown";
	struct passwd *user_info;
	struct utsname host_info;

	if ((user_info = getpwuid(geteuid())))
		safe_strncpy(given_user, user_info->pw_name, MAX_STRLEN);

	else if (error)
		ERR_REPORT("Could not detect username.");
	
	if (!(uname(&host_info)))
		safe_strncpy(given_host, host_info.nodename, MAX_STRLEN);

	else if (error)
		ERR_REPORT("Could not detect hostname.");

	snprintf(UseR, MAX_STRLEN, "%s", given_user);
	snprintf(HosT, MAX_STRLEN, "%s", given_host);

	return;
}

/*	detect_kernel
	detects the computer's kernel
	argument char *str: the char array to be filled with the kernel name
*/
void detect_kernel(char *str)
{
	struct utsname kern_info;

	if (!(uname(&kern_info)))
	{
		snprintf(str, MAX_STRLEN, "%s %s %s", kern_info.sysname,
				kern_info.release, kern_info.machine);
	}
	else if (error)
	{
		ERR_REPORT("Could not detect kernel information.");
		safe_strncpy(str, "Linux", MAX_STRLEN);
	}

	return;
}

/*	detect_uptime
	detects the computer's uptime
	argument char *str: the char array to be filled with the uptime
*/
void detect_uptime(char *str)
{
	unsigned int secs = 0, mins = 0, hrs = 0, days = 0;
	struct sysinfo si_upt;

	if (!(sysinfo(&si_upt)))
	{
		split_uptime(si_upt.uptime, &secs, &mins, &hrs, &days);

		if (days > 0)
			snprintf(str, MAX_STRLEN, "%ud %uh %um %us", days, hrs, mins, secs);
		else
			snprintf(str, MAX_STRLEN, "%uh %um %us", hrs, mins, secs);
	}
	else
		ERR_REPORT("Could not detect system uptime.");

	return;
}

static unsigned short int glob_packages(char *str1);

static unsigned short int glob_packages(char *str1)
{
	unsigned short int packs_num = 0;
	glob_t gl;

	if (!(glob(str1, GLOB_NOSORT, NULL, &gl)))
		packs_num = gl.gl_pathc;

	else if (error)
		ERR_REPORT("Failure while globbing packages.");

	globfree(&gl);

	return packs_num;
}

/*	detect_pkgs
	detects the number of packages installed on the computer
	argument char *str: the char array to be filled with the number of packages
*/
void detect_pkgs(char *str, const char *distro_str)
{
	FILE *pkgs_file;
	unsigned short int packages = 0;

	if (STREQ(distro_str, "Arch Linux")
		|| STREQ(distro_str, "ParabolaGNU/Linux-libre")
		|| STREQ(distro_str, "Chakra") || STREQ(distro_str, "Manjaro"))

		packages = glob_packages("/var/lib/pacman/local/*");

	else if (STREQ(distro_str, "Frugalware"))
	{
		pkgs_file = popen("pacman-g2 -Q 2> /dev/null | wc -l", "r");
		fscanf(pkgs_file, "%hu", &packages);
		pclose(pkgs_file);
	}
	else if (STREQ(distro_str, "Ubuntu") || STREQ(distro_str, "Lubuntu")
			|| STREQ(distro_str, "Xubuntu") || STREQ(distro_str, "LinuxMint")
			|| STREQ(distro_str, "SolusOS") || STREQ(distro_str, "Debian")
			|| STREQ(distro_str, "LMDE") || STREQ(distro_str, "CrunchBang")
			|| STREQ(distro_str, "Peppermint")
			|| STREQ(distro_str, "LinuxDeepin")	|| STREQ(distro_str, "Trisquel")
			|| STREQ(distro_str, "elementary OS")
			|| STREQ(distro_str, "Backtrack Linux")
			|| STREQ(distro_str, "Kali Linux"))
		packages = glob_packages("/var/lib/dpkg/info/*.list");

	else if (STREQ(distro_str, "Slackware"))
		packages = glob_packages("/var/log/packages/*");

	else if (STREQ(distro_str, "Gentoo") || STREQ(distro_str, "Sabayon") 
			|| STREQ(distro_str, "Funtoo"))
		packages = glob_packages("/var/db/pkg/*/*");

	else if (STREQ(distro_str, "Fuduntu") || STREQ(distro_str, "Fedora")
			|| STREQ(distro_str, "OpenSUSE")
			|| STREQ(distro_str, "Red Hat Linux")
			|| STREQ(distro_str, "Mandriva") || STREQ(distro_str, "Mandrake")
			|| STREQ(distro_str, "Mageia") || STREQ(distro_str, "Viperr"))
	{
		/* RPM uses Berkeley DBs internally, so this won't change soon */
		pkgs_file = popen("rpm -qa 2> /dev/null | wc -l", "r");
		fscanf(pkgs_file, "%hu", &packages);
		pclose(pkgs_file);
	}
	else if (STREQ(distro_str, "Angstrom"))
	{
		pkgs_file = popen("opkg list-installed 2> /dev/null | wc -l", "r");
		fscanf(pkgs_file, "%hu", &packages);
		pclose(pkgs_file);
	}
	else if (STREQ(distro_str, "Linux")) /* if linux disto detection failed */
	{
		safe_strncpy(str, "Not Found", MAX_STRLEN);

		if (error)
			ERR_REPORT("Packages cannot be detected on an unknown "
						"Linux distro.");
	}

	snprintf(str, MAX_STRLEN, "%hu", packages);

	return;
}

/*	detect_cpu
	detects the computer's CPU brand/name-string
	argument char *str: the char array to be filled with the CPU name
*/
void detect_cpu(char *str)
{
	FILE *cpu_file;
	char cpuinfo_buf[MAX_STRLEN];
	char *cpuinfo_line;
	size_t end;
	unsigned short int i = 0;

	if ((cpu_file = fopen("/proc/cpuinfo", "r")))
	{
		/* read past the first 4 lines (#5 is model name) */
		for (i = 0; i < 5; i++)
		{
			if (!(fgets(cpuinfo_buf, MAX_STRLEN, cpu_file)))
			{
				ERR_REPORT("Fatal error while reading /proc/cpuinfo");
				return;
			}
		}

		/* fail to match a colon. this should never happen, but check anyways */
		if (!(cpuinfo_line = strchr(cpuinfo_buf, ':')))
		{
			ERR_REPORT("Fatal error matching in /proc/cpuinfo");
			return;
		}

		cpuinfo_line += 2;
		end = strlen(cpuinfo_line);

		if (cpuinfo_line[end - 1] == '\n')
			cpuinfo_line[end - 1] = '\0';

		if (STREQ(cpuinfo_line, "ARMv6-compatible processor rev 7 (v6l)"))
			safe_strncpy(str, "BCM2708 (Raspberry Pi)", MAX_STRLEN);

		else
			safe_strncpy(str, cpuinfo_line, MAX_STRLEN);
	}
	else if (error)
		ERR_REPORT("Failed to open /proc/cpuinfo. Ancient Linux kernel?");

	remove_excess_cpu_txt(str);

	return;
}

/*	detect_gpu
	detects the computer's GPU brand/name-string
	argument char *str: the char array to be filled with the GPU name
*/
void detect_gpu(char *str)
{
	Display *disp = NULL;
	Window wind;
	GLint attr[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo *visual_info = NULL;
	GLXContext context = NULL;

	if ((disp = XOpenDisplay(NULL)))
	{
		wind = DefaultRootWindow(disp);

		if ((visual_info = glXChooseVisual(disp, 0, attr)))
		{
			if ((context = glXCreateContext(disp, visual_info, NULL, GL_TRUE)))
			{
				glXMakeCurrent(disp, wind, context);
				safe_strncpy(str, (const char *) glGetString(GL_RENDERER),
						MAX_STRLEN);

				glXDestroyContext(disp, context);
			}
			else if (error)
			{
				ERR_REPORT("Failed to create OpenGL context.");
			}

			XFree((void *) visual_info);
		}
		else if (error)
		{
			ERR_REPORT("Failed to select a proper X visual.");
		}

		XCloseDisplay(disp);
	}
	else if (error)
	{
		safe_strncpy(str, "No X Server", MAX_STRLEN);
		ERR_REPORT("Could not open an X display (detect_gpu).");
	}

	return;
}

/*	detect_disk
	detects the computer's total disk capacity and usage
	argument char *str: the char array to be filled with the disk data
*/
void detect_disk(char *str)
{
	struct statvfs disk_info;
	unsigned long int disk_total = 0, disk_used = 0, disk_percentage = 0;

	if (!(statvfs(getenv("HOME"), &disk_info)))
	{
		disk_total = ((disk_info.f_blocks * disk_info.f_bsize) / GB);
		disk_used = (((disk_info.f_blocks - disk_info.f_bfree) * disk_info.f_bsize) / GB);
		disk_percentage = (((float) disk_used / disk_total) * 100);
		snprintf(str, MAX_STRLEN, "%luG / %luG (%lu%%)", disk_used, disk_total, disk_percentage);
	}
	else if (error)
		ERR_REPORT("Could not stat $HOME for filesystem statistics.");

	return;
}

/*	detect_mem
	detects the computer's total and used RAM
	argument char *str: the char array to be filled with the memory data
*/
void detect_mem(char *str)
{
	unsigned long int total_mem = 0, used_mem = 0;
	struct sysinfo si_mem;

	/* known problem: because linux utilizes free ram in caches/buffers,
	   the amount of memory sysinfo reports as free is very small.
	*/
	sysinfo(&si_mem);

	total_mem = (unsigned long int) (si_mem.totalram * si_mem.mem_unit) / MB;
	used_mem  = (unsigned long int) ((si_mem.totalram - si_mem.freeram - si_mem.bufferram
											 - si_mem.sharedram) * si_mem.mem_unit) / MB;

	snprintf(str, MAX_STRLEN, "%lu%s / %lu%s", used_mem, "MB", total_mem, "MB");

	return;
}

/*	detect_shell
	detects the shell currently running on the computer
	argument char *str: the char array to be filled with the shell name and version
*/
void detect_shell(char *str)
{
	char *shell_name;
	unsigned short int x = 0;
	bool found_shell = false;

	if (!(shell_name = getenv("SHELL")))
	{
		if (error)
			ERR_REPORT("Could not detect a shell - $SHELL not defined.");

		return;
	}

	if (STREQ(shell_name, "/bin/sh"))
		safe_strncpy(str, "POSIX sh", MAX_STRLEN);

	else
	{
		for (x = 0; x < 4; x++)
			if (strstr(shell_name, mult_shell_arr[x][1]))
			{
				found_shell = true;
				break;
			}

		if (found_shell)
			popen_raw_shell_version(mult_shell_arr[x][0], mult_shell_arr[x][1]);
		else
			safe_strncpy(str, shell_name, MAX_STRLEN);
	}
	/* Add support for moar shells: "dash", "ash", "ksh"
		include them in arrays.c */

	return;
}

/*	detect_res
	detects the combined resolution of all monitors attached to the computer
	argument char *str: the char array to be filled with the resolution
*/
void detect_res(char *str)
{
	unsigned short int width = 0, height = 0;
	Display *disp;
	Screen *screen;

	if ((disp = XOpenDisplay(NULL)))
	{
		screen = XDefaultScreenOfDisplay(disp);
		width = WidthOfScreen(screen);
		height = HeightOfScreen(screen);

		snprintf(str, MAX_STRLEN, "%hux%hu", width, height);

		XCloseDisplay(disp);
	}
	else
	{
		safe_strncpy(str, "No X Server", MAX_STRLEN);

		if (error)
			ERR_REPORT("Could not open an X display (detect_res)");
	}

	return;
}

/*	detect_de
	detects the desktop environment currently running on top of the OS
	argument char *str: the char array to be filled with the DE name
*/
void detect_de(char *str)
{
	char *curr_de;

	if ((curr_de = getenv("XDG_CURRENT_DESKTOP")))
		safe_strncpy(str, curr_de, MAX_STRLEN);

	else
	{
		if (getenv("GNOME_DESKTOP_SESSION_ID"))
			safe_strncpy(str, "Gnome", MAX_STRLEN);

		else if (getenv("MATE_DESKTOP_SESSION_ID"))
			safe_strncpy(str, "MATE", MAX_STRLEN);

		else if (getenv("KDE_FULL_SESSION"))
			/*	KDE_SESSION_VERSION only exists on KDE4+, so 
				getenv will return NULL on KDE <= 3.
			 */
			snprintf(str, MAX_STRLEN, "KDE%s", getenv("KDE_SESSION_VERSION"));

		else if (error)
			ERR_REPORT("No desktop environment found.");
	}

	return;
}

/*	detect_wm
	detects the window manager currently running on top of the OS
	argument char *str: the char array to be filled with the WM name
*/
void detect_wm(char *str)
{
	Display *disp;
	Atom actual_type;
	int actual_format;
	unsigned long nitems;
	unsigned long bytes;
	char *wm_name = '\0';
	Window *wm_check_window;

	if ((disp = XOpenDisplay(NULL)))
	{
		if (!(XGetWindowProperty(disp, DefaultRootWindow(disp),
			XInternAtom(disp, "_NET_SUPPORTING_WM_CHECK", true),
			0, KB, false,	XA_WINDOW, &actual_type, &actual_format, &nitems,
			&bytes, (unsigned char **) &wm_check_window)))
		{
			if (!(XGetWindowProperty(disp, *wm_check_window,
				XInternAtom(disp, "_NET_WM_NAME", true), 0, KB, false,
				XInternAtom(disp, "UTF8_STRING", true),	&actual_type,
				&actual_format, &nitems, &bytes, (unsigned char **) &wm_name)))
			{
				safe_strncpy(str, wm_name, MAX_STRLEN);
				XFree(wm_name);
			}
			else if (error)
			{
				ERR_REPORT("No _NET_WM_NAME property found.");
			}

			XFree(wm_check_window);
		}
		else if (error)
		{
			ERR_REPORT("No WM detected (non-EWMH compliant?)");
		}

		XCloseDisplay(disp);
	}
	else if (error)
	{
		ERR_REPORT("Could not open an X display. (detect_wm)");
	}

	return;
}

/*	detect_wm_theme
	detects the theme associated with the WM detected in detect_wm()
	argument char *str: the char array to be filled with the WM Theme name
	--
	CAVEAT: This function relies on the presence of 'detectwmtheme', a shell script. 
	If it isn't present somewhere in the PATH, the WM Theme will be set as 'Unknown'
	--
*/
void detect_wm_theme(char *str, const char *wm_str)
{
	char exec_str[MAX_STRLEN];
	FILE *wm_theme_file;

	snprintf(exec_str, MAX_STRLEN, "detectwmtheme %s 2> /dev/null", wm_str);

	wm_theme_file = popen(exec_str, "r");
	fgets(str, MAX_STRLEN, wm_theme_file);
	pclose(wm_theme_file);

	return;
}

/*	detect_gtk
	detects the theme, icon(s), and font(s) associated with a GTK DE (if present)
	argument char *str1: the char array to be filled with the GTK info
	argument char *str2: the char array to be filled with GTK icons
	argument char *str3: the char array to be filled with Font
	--
	CAVEAT: This function relies on the presence of 'detectgtk', a shell script.
	If it isn't present somewhere in the PATH, the GTK will be set as 'Unknown'
	--
*/
void detect_gtk(char *str1, char *str2, char *str3)
{
	FILE *gtk_file;
	char gtk2_str[MAX_STRLEN] = "Unknown";
	char gtk3_str[MAX_STRLEN] = "Unknown";
	char gtk_icons_str[MAX_STRLEN] = "Unknown";
	char font_str[MAX_STRLEN] = "Unknown";

	gtk_file = popen("detectgtk 2> /dev/null", "r");
	fscanf(gtk_file, "%s%s%s%s", gtk2_str, gtk3_str, gtk_icons_str, font_str);
	pclose(gtk_file);

	if (STREQ(gtk3_str, "Unknown"))
		snprintf(str1, MAX_STRLEN, "%s (GTK2), %s (Icons)", gtk2_str,
				gtk_icons_str);
	else if (STREQ(gtk2_str, "Unknown"))
		snprintf(str1, MAX_STRLEN, "%s (GTK3), %s (Icons)", gtk3_str,
				gtk_icons_str);
	else
		snprintf(str1, MAX_STRLEN, "%s (GTK2), %s (GTK3)", gtk2_str, gtk3_str);

	snprintf(str2, MAX_STRLEN, "%s", gtk_icons_str);

	snprintf(str3, MAX_STRLEN, "%s", font_str);

	return;
}
