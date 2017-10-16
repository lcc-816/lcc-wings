/* 65816's running nothing */

#include <string.h>

static char rcsid[] = "$Id: scpu.c,v 1.3 2003/12/18 11:15:39 jmaginni Exp $";

#ifndef LCCDIR
#define LCCDIR "/home/jolz/lcc/output/"
#endif

#ifndef WGSDIR
#define WGSDIR "/home/jolz/cvs/wgs-src"
#endif

#define INCDIR WGSDIR "/include/c/"
#define LIB65DIR WGSDIR "/lib/"
#define BINDIR WGSDIR "/btools/"


char *suffixes[] = { ".c", ".i", ".a65", ".o65", ".out", 0 };
char inputs[256] = "";
char *cpp[] = { LCCDIR "cpp",
	"-D_POSIX_SOURCE", "-D__STDC__=1", "-D__JOS__",
	"$1", "$2", "$3", 0 };
char *include[] = {"-I" INCDIR, 0 };
char *com[] = { LCCDIR "rcc", "-target=mips/irix", "$1", "$2", "$3", 0 };
char *as[] = {  BINDIR "ja", "-o", "$3", "$1", "$2", 0 };
char *ld[] = {
	BINDIR "jl65", "-G", "-y", "$1", "-o", "$3", "-lcrt", "-llibc", "-lunilib",
	LIB65DIR "startwgs.o65", "$2", 0
	};

extern char *concat(char *, char *);

int option(char *arg) {
   	if (!strcmp(arg,"-pic"))
     		ld[9] = LIB65DIR "startpic.o65";
	return 1;
}
