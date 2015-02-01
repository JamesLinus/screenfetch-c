/*  structs.h
 *  Author: Aaron Caffrey
 *  -------------
 *  
 *  All structures and their members are declared, initialized (and externalized)
 *  here for use by other screenfetch-c modules.
 *  Like the rest of screenfetch-c, this file is licensed under the MIT license.
 */

#ifndef SCREENFETCH_C_STRUCTS_H
#define SCREENFETCH_C_STRUCTS_H

#include "logos_length.h"
#include "colors.h"

typedef struct
{
    char **logo;                    /* the ascii logo in logos.c */
    unsigned short int len;         /* controls the 'for' loop iteration cycles */
    unsigned short int break_point; /* do not index detected_arr_names[x] and detected_arr[x] after this value */
    char *colour;                   /* the colour for "user@hostname" */
    char *distro;                   /* the distro name to match in main_ascii_output() */
} mad_dog_struct;


/*  keep in mind to change the main_ascii_output()
    'for' loop value whenever you add another distro/OS
*/
static const mad_dog_struct opt[] = 
{
    {.logo = oldarch_logo, .len = ARCH_OLD_LEN, .break_point = 16, .colour = TLBL, .distro = "Arch Linux - Old"},

    {.logo = arch_logo, .len = ARCH_LEN, .break_point = 17, .colour = TLCY, .distro = "Arch Linux"},

    {.logo = mint_logo, .len = MINT_LEN, .break_point = 16, .colour = TLGN, .distro = "LinuxMint"},

    {.logo = lmde_logo, .len = LMDE_LEN, .break_point = 16, .colour = TLGN, .distro = "LMDE"},

    {.logo = debian_logo, .len = DEBIAN_LEN, .break_point = 16, .colour = TLRD, .distro = "Debian"},

    {.logo = crunchbang_logo, .len = CRUNCHBANG_LEN, .break_point = 16, .colour = TDGY, .distro = "CrunchBang"},

    {.logo = gentoo_logo, .len = GENTOO_LEN, .break_point = 16, .colour = TLPR, .distro = "Gentoo"},

    {.logo = funtoo_logo, .len = FUNTOO_LEN, .break_point = 16, .colour = TLPR, .distro = "Funtoo"},

    {.logo = fedora_logo, .len = FEDORA_LEN, .break_point = 16, .colour = TLBL, .distro = "Fedora"},

    {.logo = opensuse_logo, .len = OPENSUSE_LEN, .break_point = 16, .colour = TLGN, .distro = "OpenSUSE"},

    {.logo = slackware_logo, .len = SLACKWARE_LEN, .break_point = 16, .colour = TLBL, .distro = "Slackware"},

    {.logo = redhat_logo, .len = REDHAT_LEN, .break_point = 16, .colour = TRED, .distro = "Red Hat Linux"},

    {.logo = frugalware_logo, .len = FRUGALWARE_LEN, .break_point = 16, .colour = TLCY, .distro = "Frugalware"},

    {.logo = peppermint_logo, .len = PEPPERMINT_LEN, .break_point = 16, .colour = TLRD, .distro = "Peppermint"},

    {.logo = solusos_logo, .len = SOLUSOS_LEN, .break_point = 16, .colour = TLGY, .distro = "SolusOS"},

    {.logo = mageia_logo, .len = MAGEIA_LEN, .break_point = 16, .colour = TLGY, .distro = "Mageia"},

    {.logo = parabolagnu_linuxlibre_logo, .len = PARABOLA_LEN, .break_point = 16, .colour = TLGY, .distro = "ParabolaGNU/Linux-libre"},

    {.logo = viperr_logo, .len = VIPERR_LEN, .break_point = 16, .colour = TLGY, .distro = "Viperr"},

    {.logo = linuxdeepin_logo, .len = DEEPIN_LEN, .break_point = 16, .colour = TLGN, .distro = "LinuxDeepin"},

    {.logo = chakra_logo, .len = CHAKRA_LEN, .break_point = 16, .colour = TLBL, .distro = "Chakra"},

    {.logo = fuduntu_logo, .len = FUDUNTU_LEN, .break_point = 16, .colour = TLRD, .distro = "Fuduntu"},

    {.logo = trisquel_logo, .len = TRISQUEL_LEN, .break_point = 16, .colour = TLBL, .distro = "Trisquel"},

    {.logo = manjaro_logo, .len = MANJARO_LEN, .break_point = 16, .colour = TLGN, .distro = "Manjaro"},

    {.logo = elementaryos_logo, .len = ELEMENTARY_LEN, .break_point = 16, .colour = TLGN, .distro = "elementary OS"},

    {.logo = scientificlinux_logo, .len = SCIENTIFIC_LEN, .break_point = 16, .colour = TLRD, .distro = "Scientific Linux"},

    {.logo = backtracklinux_logo, .len = BACKTRACK_LEN, .break_point = 16, .colour = TLRD, .distro = "Backtrack Linux"},

    {.logo = backtracklinux_logo, .len = KALI_LEN, .break_point = 16, .colour = TLBL, .distro = "Kali Linux"},

    {.logo = sabayon_logo, .len = SABAYON_LEN, .break_point = 16, .colour = TLBL, .distro = "Sabayon"},

    {.logo = android_logo, .len = ANDROID_LEN, .break_point = 12, .colour = TLGN, .distro = "Android"},

    {.logo = angstrom_logo, .len = ANGSTROM_LEN, .break_point = 0, .colour = "", .distro = "Angstrom"},

    {.logo = linux_logo, .len = LINUX_LEN, .break_point = 16, .colour = TLGY, .distro = "Linux"},

    {.logo = freebsd_logo, .len = FREEBSD_LEN, .break_point = 16, .colour = TLRD, .distro = "FreeBSD"},

    {.logo = openbsd_logo, .len = OPENBSD_LEN, .break_point = 16, .colour = TYLW, .distro = "OpenBSD"},

    {.logo = netbsd_logo, .len = NETBSD_LEN, .break_point = 16, .colour = TLRD, .distro = "NetBSD"},

    {.logo = dragonflybsd_logo, .len = DRAGONFLYBSD_LEN, .break_point = 16, .colour = TLRD, .distro = "DragonFly BSD"},

    {.logo = solaris_logo, .len = SUNOS_LEN, .break_point = 16, .colour = TBLU, .distro = "SunOS"}

};


/*typedef struct screenfetch_c_ screenfetch_c;

struct screenfetch_c_
{
    char *UseR;
    char *HosT;
    char *User_hosT;
};

extern screenfetch_c ContaineR;*/

#endif /* SCREENFETCH_C_STRUCTS_H */