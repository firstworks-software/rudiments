dnl displays a message indicating that the include flags for $1 are $2
AC_DEFUN([FW_INCLUDES],
[
if ( test -n "$2" )
then
	echo "$1 includes... $2"
fi
])


dnl displays a message indicating that the lib flags for $1 are $2
AC_DEFUN([FW_LIBS],
[
if ( test -n "$2" )
then
	echo "$1 libs... $2"
fi
])


dnl if $1 is a readable file (full path name) then expression $2 is evaluated
AC_DEFUN([FW_CHECK_FILE],
[
if ( test -r "$1" )
then
	eval "$2"
fi
])


dnl attmepts to compile and link a program
dnl	$1 - includes and other statements to go before main()
dnl	$2 - body of main()
dnl	$3 - CPPFLAGS
dnl	$4 - LIBS
dnl	$5 - LD_LIBRARY_PATH
dnl	$6 - statement to evaluate if link succeeds
dnl	$7 - statement to evaluate if link fails
AC_DEFUN([FW_TRY_LINK],
[
SAVECPPFLAGS="$CPPFLAGS"
SAVELIBS="$LIBS"
SAVE_LD_LIBRARY_PATH="$LD_LIBRARY_PATH"
CPPFLAGS="$3"
LIBS="$4"
LD_LIBRARY_PATH="$5"
export LD_LIBRARY_PATH
AC_TRY_LINK([$1],[$2],[$6],[$7])
CPPFLAGS="$SAVECPPFLAGS"
LIBS="$SAVELIBS"
LD_LIBRARY_PATH="$SAVE_LD_LIBRARY_PATH"
export LD_LIBRARY_PATH
])


dnl attmepts to compile and link a program
dnl	$1 - includes and other statements to go before main()
dnl	$2 - body of main()
dnl	$3 - CPPFLAGS
dnl	$4 - statement to evaluate if link succeeds
dnl	$5 - statement to evaluate if link fails
AC_DEFUN([FW_TRY_COMPILE],
[
SAVECPPFLAGS="$CPPFLAGS"
CPPFLAGS="$3"
AC_TRY_COMPILE([$1],[$2],[$4],[$5])
CPPFLAGS="$SAVECPPFLAGS"
])


dnl checks for a library file
dnl	$1 - library file to check for (full path name)
dnl	$2 - statement to evaluate if library file is found
dnl	$3 - alternative library file to check for
dnl	$2 - statement to evaluate if alternative library file is found
AC_DEFUN([FW_CHECK_LIB],
[
FOUNDLIB=""
FW_CHECK_FILE($1, FOUNDLIB="yes")
if ( test -n "$FOUNDLIB" )
then
	eval "$2"
else
	if ( test -n "$3" )
	then
		FW_CHECK_FILE($3, FOUNDLIB="yes")
		if ( test -n "$FOUNDLIB" )
		then
			eval "$4"
		fi
	fi
fi
])


dnl checks for a header and library pair
dnl	$1 - header file to check for (full path name)
dnl	$2 - statement to evaluate if header and library are found
dnl	$3 - library file to check for (full path name)
dnl	$4 - statement to evaluate if header and library are found
dnl	$5 - alternative library file to check for (full path name)
dnl	$6 - statement to evaluate if header and alternative library are found
AC_DEFUN([FW_CHECK_HEADER_LIB],
[
FOUNDHEADER=""
FOUNDLIB=""
FW_CHECK_FILE([$1],[FOUNDHEADER=\"yes\"])
FW_CHECK_FILE([$3],[FOUNDLIB=\"yes\"])
if ( test -n "$FOUNDLIB" )
then
	if ( test -n "$FOUNDHEADER" -a -n "$FOUNDLIB" )
	then
		eval "$2"
		eval "$4"
	fi
else
	if ( test -n "$5" -a -n "$6" )
	then
		FW_CHECK_FILE([$5],[FOUNDLIB=\"yes\"])
		if ( test -n "$FOUNDHEADER" -a -n "$FOUNDLIB" )
		then
			eval "$2"
			eval "$6"
		fi
	fi
fi
])


dnl checks the specified search path and also various build-in paths for a
dnl header and library pair
dnl	$1 - search path
dnl	$2 - generic name of api or package, will be appended to various
dnl		partial paths in an attempt to search more exhaustively
dnl		eg. ssl, mysql, openssl, etc.
dnl	$3 - header file to check for (relative path name)
dnl		eg. api.h  not  /usr/include/api.h
dnl	$4 - library basename (excluding "lib" and suffix) to check for
dnl		eg. api  not  lbiapi, libapi.so, /usr/lib/libapi.so, etc.
dnl	$5 - flag to set $10 to if a static version of the library was found
dnl	$6 - ??? rpath related, but unused
dnl	$7 - variable that will be set to the include flags that were found
dnl		eg. -I/usr/local/include
dnl	$8 - variable that will be set to the lib flags that were found
dnl		eg. -L/usr/local/lib -lsomelib
dnl	$9 - variable that will be set to the path that the library was found in
dnl		eg. /usr/local/lib
dnl	$10 - variable that will be set to the value passed in $5 if a static
dnl		version of the library was found
dnl	$11 - variable that will be set to the base directory that the headers
dnl		and libs were found in
dnl		eg. /usr/local
dnl	$12 - whether or not to use the full library path when building the lib
dnl		flags - can be yes or no
dnl		eg. if yes, then $8 set to: -L/usr/local/lib -lsomelib
dnl		    if no, then $8 set to:  -Wl,/usr/local/lib/libsomelib.so
dnl
dnl If LIBDIR and/or LIBARCHDIR are set, then they are appended, as
dnl appropriate, to the various paths when searching for libraries
dnl	eg. if LIBDIR=lib and LIBARCHDIR=lib32 then /opt/sfw/lib and
dnl		/usr/freeware/lib32 are searched
dnl If either is not set, then "lib" is appended
dnl	eg. /opt/sfw/lib and /usr/freeware/lib are searched
AC_DEFUN([FW_CHECK_HEADERS_AND_LIBS],
[

SEARCHPATH=$1
NAME=$2
HEADER=$3
LIBNAME=$4
LINKSTATIC=$5
LINKRPATH=$6
USEFULLLIBPATH=$12
INCLUDESTRING=""
LIBSTRING=""
LIBPATH=""
STATIC=""
HEADERSANDLIBSPATH=""

eval "$7=\"\""
eval "$8=\"\""
eval "$9=\"\""
eval "$10=\"\""
if ( test -n "$11" )
then
	eval "$11=\"\""
fi

for path in "$SEARCHPATH" "/" "/usr" "/usr/local/$NAME" "/opt/$NAME" "/usr/$NAME" "/usr/local" "/usr/pkg" "/usr/pkg/$NAME" "/opt/sfw" "/opt/sfw/$NAME" "/usr/sfw" "/usr/sfw/$NAME" "/opt/csw" "/sw" "/usr/freeware" "/boot/common" "/resources/index" "/resources/firstworks" "/Library/$NAME" "/usr/local/firstworks"
do
	if ( test -n "$path" -a -d "$path" )
	then

		TEMPLIBDIR=$LIBDIR
		if ( test "$path" = "/usr/freeware" )
		then
			TEMPLIBDIR=$LIBARCHDIR
		fi
		if ( test -z "$TEMPLIBDIR" )
		then
			TEMPLIBDIR=lib
		fi

		if ( test "$path" = "/" )
		then
			if ( test "$USEFULLLIBPATH" = "yes" )
			then
				FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[],[/$TEMPLIBDIR/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"/$TEMPLIBDIR\"; LIBSTRING=\"-Wl,/$TEMPLIBDIR/lib$LIBNAME.$SOSUFFIX\"],[/$TEMPLIBDIR/lib$LIBNAME.a],[LIBSTRING=\"/$TEMPLIBDIR/lib$LIBNAME.a\"; STATIC=\"$LINKSTATIC\"])
			else
				FW_CHECK_HEADER_LIB([/usr/include/$HEADER],[],[/$TEMPLIBDIR/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"/$TEMPLIBDIR\"; LIBSTRING=\"-l$LIBNAME\"],[/$TEMPLIBDIR/lib$LIBNAME.a],[LIBSTRING=\"-l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
			fi
			dnl set path to "" so we won't get //'s from here on
			path=""
		fi
		if ( test -n "$LIBSTRING" )
		then
			HEADERSANDLIBSPATH="$path"
			break
		fi
		for libpath in "$path/$TEMPLIBDIR" "$path/$TEMPLIBDIR/$NAME" "$path/$TEMPLIBDIR/opt" "$path/$TEMPLIBDIR/$MULTIARCHDIR"
		do
			if ( test -n "$LIBSTRING" )
			then
				break
			fi
			for includepath in "$path/include" "$path/include/$NAME"
			do
				if ( test -n "$LIBSTRING" )
				then
					break
				fi
				if ( test "$USEFULLLIBPATH" = "yes" )
				then
					FW_CHECK_HEADER_LIB([$includepath/$HEADER],[INCLUDESTRING=\"-I$includepath\"],[$libpath/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"$libpath\"; LIBSTRING=\"-Wl,$libpath/lib$LIBNAME.$SOSUFFIX\"],[$libpath/lib$LIBNAME.a],[LIBSTRING=\"$libpath/lib$LIBNAME.a\"; STATIC=\"$LINKSTATIC\"])
				else
					FW_CHECK_HEADER_LIB([$includepath/$HEADER],[INCLUDESTRING=\"-I$includepath\"],[$libpath/lib$LIBNAME.$SOSUFFIX],[LIBPATH=\"$libpath\"; LIBSTRING=\"-L$libpath -l$LIBNAME\"],[$libpath/lib$LIBNAME.a],[LIBSTRING=\"-L$libpath -l$LIBNAME\"; STATIC=\"$LINKSTATIC\"])
				fi
			done
		done
		if ( test -n "$LIBSTRING" )
		then
			HEADERSANDLIBSPATH="$path"
			break
		fi
	fi
done

dnl remove -I/usr/include, -L/lib, -L/usr/lib, -L/lib64 and -L/usr/lib64
INCLUDESTRING=`echo $INCLUDESTRING | sed -e "s|-I/usr/include$||g" -e "s|-I/usr/include ||g"`
LIBSTRING=`echo $LIBSTRING | sed -e "s|-L/usr/lib$||g" -e "s|-L/lib$||g" -e "s|-L/usr/lib ||g" -e "s|-L/lib ||g"`
LIBSTRING=`echo $LIBSTRING | sed -e "s|-L/usr/lib64$||g" -e "s|-L/lib64$||g" -e "s|-L/usr/lib64 ||g" -e "s|-L/lib64 ||g"`

eval "$7=\"$INCLUDESTRING\""
eval "$8=\"$LIBSTRING\""
eval "$9=\"$LIBPATH\""
eval "$10=\"$STATIC\""
if ( test -n "$11" )
then
	eval "$11=\"$HEADERSANDLIBSPATH\""
fi
])


dnl removes -Wl,-Bsymbolic-functions from $2 and sets variable $1 to the result
AC_DEFUN([FW_STRIP_SYMBOLIC_FUNCTIONS],
[
dnl On some platforms (Ubuntu), the -Wl,-Bsymbolic-functions and flags end up
dnl coming through in ldflags, which can cause problems for apps (sqlrelay+db2
dnl on Ubuntu 18.04).  Filter those out.
STRIPPED=`echo "$2" | sed -e "s|-Wl,-Bsymbolic-functions||g"`
eval "$1=\"$STRIPPED\""
])


dnl removes -Wl,-z,relro from $2 and sets variable $1 to the result
AC_DEFUN([FW_STRIP_RELRO],
[
dnl On some platforms (Ubuntu), superfluous -Wl,-z,relro flags end up coming
dnl through in ldflags.  Filter those out.
STRIPPED=`echo "$2" | sed -e "s|-Wl,-z,relro||g"`
eval "$1=\"$STRIPPED\""
])


dnl if the variable USE_SYSTEM_LIBTOOL = "yes" then the variable LIBTOOL is
dnl overridden and set to "libtool" rather than its current value
dnl (likely "$(top_builddir)/libtool" so that the system-supplied libtool is
dnl used rather than the local version
AC_DEFUN([FW_CHECK_USE_SYSTEM_LIBTOOL],
[
if ( test "$USE_SYSTEM_LIBTOOL" = "yes" )
then
	LIBTOOL="libtool"
fi
])


dnl checks to see if -pipe compiler option works or not
dnl if it does, then it sets the variable PIPE="-pipe"
dnl if it doesnot , then it sets the variable PIPE=""
AC_DEFUN([FW_CHECK_PIPE],
[
AC_MSG_CHECKING(for -pipe option)
FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-pipe],[],[],[PIPE="-pipe"],[PIPE=""])
if ( test -n "$PIPE" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
AC_SUBST(PIPE)
])


dnl checks to see if -Werror compiler option works or not
dnl if it does, then WERROR="-Werror" is set
dnl if it does not, then WERROR="" is set
AC_DEFUN([FW_CHECK_WERROR],
[
WERROR=""
if ( test "$ENABLE_WERROR" = "yes" )
then
	AC_MSG_CHECKING(for -Werror)
	FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-Werror],[],[],[WERROR="-Werror"])

	dnl disable -Werror on Haiku, Minix, and Ultrix as their header files throw warnings
	dnl disable -Werror on mingw32 as the regex.cpp file has unused variables that
	dnl are hard to fix
	case $host_os in
		*haiku* )
			WERROR=""
			;;
		*minix* )
			WERROR=""
			;;
		*ultrix* )
			WERROR=""
			;;
		*mingw32* )
			WERROR=""
			;;
		*)
			;;
	esac

	dnl if -Werror appers to be supported...
	if ( test -n "$WERROR" )
	then

		dnl disable -Werror with gcc < 2.7 because
		dnl it misinterprets placement new
		CXX_VERSION=`$CXX --version 2> /dev/null | head -1 | tr -d '.' | cut -c1-2`

		dnl Newer versions of gcc output the version differently
		dnl and the above results in "g+".  These all work correctly.
		if ( test "$CXX_VERSION" != "g+" )
		then
			dnl older versions output something like 27, 28, 29, etc.
			if (  test "$CXX_VERSION" -lt "27" )
			then
				WERROR=""
			fi
		fi

	fi

	if ( test -n "$WERROR" )
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi
fi

AC_SUBST(WERROR)
])


dnl enables -Werror (if supported) in CPPFLAGS/CXXFLAGS if it wasn't previously
dnl enabled and sets HADWERROR to yes if it was previously enabled or to no if
dnl it wasn't
AC_DEFUN([FW_ENABLE_WERROR],
[
HADWERROR="yes"
if ( test -z "`echo $CPPFLAGS | grep Werror`" )
then
	CPPFLAGS="$CPPFLAGS $WERROR"
	CXXFLAGS="$CXXFLAGS $WERROR"
	HADWERROR="no"
fi
])


dnl evaluates HADWERROR and removes instances of -Werror from CPPFLAGS and
dnl CXXFLAGS if it wasn't previously enabled (see FW_ENABLE_WERROR)
dnl FIXME: also removes Wall, though that isn't enabled by FW_ENABLE_WERROR
AC_DEFUN([FW_RESTORE_WERROR],
[
if ( test "$HADWERROR" = "no" )
then
	CPPFLAGS=`echo $CPPFLAGS | sed -e "s|-Werror[[^ ]]*||g" -e "s|-Wall||g"`
	CXXFLAGS=`echo $CXXFLAGS | sed -e "s|-Werror[[^ ]]*||g" -e "s|-Wall||g"`
fi
])


dnl checks to see if -Wall compiler option works or not
dnl if it does, then WALL="-Wall" is set
dnl if it does not, then WALL="" is set
dnl
dnl also checks to see if -Wall includes -Wunused-variables and sets WALL="" if
dnl it does
dnl
dnl FIXME: that second bit should be split out into its own macro
AC_DEFUN([FW_CHECK_WALL],
[
WALL=""
if ( test "$ENABLE_WALL" = "yes" )
then
	AC_MSG_CHECKING(for -Wall)
	FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-Wall],[],[],[WALL="-Wall"])
	if ( test -n "$WALL" )
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi

	if ( test -n "$WALL" )
	then
		dnl Sometimes -Wall includes -Wunused-variables and
		dnl -Wunused-parameters which we don't care about.
		dnl Disable it if it does.
		OLDCPPFLAGS=$CPPFLAGS
		CPPFLAGS="$WALL $WERROR $CPPFLAGS"
		AC_MSG_CHECKING(whether -Wall includes -Wunused-*)
		AC_TRY_COMPILE([void f(int a) { return; }],[f(1);],AC_MSG_RESULT(no),WALL=""; AC_MSG_RESULT(yes))	
		CPPFLAGS=$OLDCPPFLAGS
	fi
fi
AC_SUBST(WALL)
])


dnl checks to see if -Wno-format option is necessary to compile class methods
dnl named printf()
dnl if it is, then WNOFORMAT="-Wno-format" is set
dnl if it is not, then WNOFORMAT="" is set
AC_DEFUN([FW_CHECK_WNOFORMAT],
[

WNOFORMAT=""
if ( test -n "$WERROR" )
then
	OLDCPPFLAGS=$CPPFLAGS
	CPPFLAGS="$WALL $WERROR $CPPFLAGS"
	AC_MSG_CHECKING(whether -Wno-format is needed)
	AC_TRY_COMPILE([#include <stdio.h>

class charstring {
	public:
		static void printf(char *buffer, const char *fmt);
};

void charstring::printf(char *buffer, const char *fmt) {
	vsprintf(buffer,fmt,NULL);
}],[char buf[10]; charstring::printf(buf,"hello");],AC_MSG_RESULT(no), AC_MSG_RESULT(yes); WNOFORMAT="-Wno-format")	
	CPPFLAGS=$OLDCPPFLAGS
fi

AC_SUBST(WNOFORMAT)
])


dnl checks to see if -Wall includes -Woverloaded-virtual
dnl if it does, then WNOOVERLOADEDVIRTUAL="-Wno-overloaded-virtual" is set
dnl if it does not, then WNOOVERLOADEDVIRTUAL="" is set
AC_DEFUN([FW_CHECK_WNOOVERLOADEDVIRTUAL],
[

WNOOVERLOADEDVIRTUAL=""
AC_MSG_CHECKING(whether -Wno-overloaded-virtual is needed)

# clang's -Wall includes -Woverloaded-virtual, which we don't want
if ( test -n "`$CC --version 2> /dev/null | grep clang`" )
then
	WNOOVERLOADEDVIRTUAL="-Wno-overloaded-virtual"
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi

AC_SUBST(WNOOVERLOADEDVIRTUAL)
])


dnl checks to see if -Wno-deprecated-declarations is permitted
dnl if it is, then WNODEPRECATEDDECLARATIONS="-Wno-deprecated-declarations"
dnl is set
dnl if it is not, then WNODEPRECATEDDECLARATIONS="" is set
AC_DEFUN([FW_CHECK_WNODEPRECATEDDECLARATIONS],
[

WNODEPRECATEDDECLARATIONS=""
if ( test -n "$WERROR" )
then
	OLDCPPFLAGS=$CPPFLAGS
	CPPFLAGS="$CPPFLAGS -Wno-deprecated-declarations"
	AC_MSG_CHECKING(whether -Wno-deprecated-declarations is permitted)
	AC_TRY_COMPILE([#include <stdio.h>],[printf("hello");],AC_MSG_RESULT(yes); WNODEPRECATEDDECLARATIONS="-Wno-deprecated-declarations",AC_MSG_RESULT(yes))	
	CPPFLAGS=$OLDCPPFLAGS
fi

AC_SUBST(WNODEPRECATEDDECLARATIONS)
])


dnl checks to see if the C++ compiler allows undefined functions
dnl if it does, then WERROR is added to CPPFLAGS then sets WERROR to "" so it
dnl won't be put back in again later
AC_DEFUN([FW_CHECK_UNDEFINED_FUNCTIONS],
[
AC_MSG_CHECKING(for whether undefined functions are allowed)
AC_TRY_COMPILE([],[printf("hello");],CPPFLAGS="$WERROR $CPPFLAGS"; WERROR=""; AC_MSG_RESULT(yes), AC_MSG_RESULT(no))
])


dnl checks to see if -g3 option works or not
dnl adds it to CXXFLAGS if it does
AC_DEFUN([FW_CHECK_DEBUG],
[
if ( test "$DEBUG" = "yes" )
then
	AC_MSG_CHECKING(for -g3)
	FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-g3],[],[],[DBG="-g3"],[DBG="-g"])
	if ( test "$DBG" = "-g3" )
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi
	CXXFLAGS="$CXXFLAGS $DBG"
fi
])


dnl Some environments define NULL as ((void *)0) but the compiler can't handle
dnl that when the NULL is used in a conditional.  On those platforms we'll
dnl set a flag to redefine NULL as 0.
AC_DEFUN([FW_CHECK_NULL_REDEFINE],
[
AC_MSG_CHECKING(whether NULL needs to be redefined)
AC_TRY_COMPILE([#include <stddef.h>
const char *f() { return (1)?"":NULL; }],[f();],AC_MSG_RESULT(no),AC_DEFINE(RUDIMENTS_REDEFINE_NULL,1,Redfine NULL as 0) AC_MSG_RESULT(yes))
])


dnl Some environments throw warnings if stdlib is used because it redefines
dnl built-in functions abort() exit().  On those platforms we'll include the
dnl -fno-builtin flag in CPPFLAGS
AC_DEFUN([FW_CHECK_F_NO_BUILTIN],
[
AC_MSG_CHECKING(whether -fno-builtin needs to be used)

STDLIB_TEST="no"
AC_TRY_COMPILE([#include <stdlib.h>],[],STDLIB_TEST="yes")

dnl If that failed, try again with -fno-builtin
if ( test "$STDLIB_TEST" = "no" )
then
	OLDCPPFLAGS="$CPPFLAGS"
	CPPFLAGS="-fno-builtin $CPPFLAGS"
	AC_TRY_COMPILE([#include <stdlib.h>],[],STDLIB_TEST="yes")

	dnl if that also failed then restore CPPFLAGS,
	dnl the platform probably just doesn't have stdlib.h
	if ( test "$STDLIB_TEST" = "no" )
	then
		CPPFLAGS="$OLDCPPFLAGS"
		AC_MSG_RESULT(no)
	else
		AC_MSG_RESULT(yes)
	fi
 else
	AC_MSG_RESULT(no)
fi
])


dnl Determines what extension shared object files have and sets SOSUFFIX to it
AC_DEFUN([FW_CHECK_SO_EXT],
[
AC_MSG_CHECKING(for dynamic library extension)
SOSUFFIX="so"
if ( test -n "$CYGWIN" )
then
	SOSUFFIX="dll.a"
fi
if ( test -n "$DARWIN" )
then
	SOSUFFIX="dylib"
fi
AC_MSG_RESULT($SOSUFFIX)
])


dnl determines the directory that libraries are usually found in (eg. lib or
dnl lib64) and the arch-spcific directory (eg. lib or lib32) for odd multiarch
dnl systems (eg. Irix)
AC_DEFUN([FW_CHECK_LIBDIR],
[
AC_MSG_CHECKING(for library directory)
LIBDIR="lib"
LIBARCHDIR="lib"
if ( test -z "$MULTIARCHDIR" )
then
	case $host_cpu in
		x86_64 )
			LIBDIR="lib64"
			LIBARCHDIR="lib64"
			;;
		mips64 )
			LIBDIR="lib64"
			LIBARCHDIR="lib64"
			;;
		mips )
			LIBARCHDIR="lib32"
			;;
	esac
fi
if ( test "$LIBDIR" = "lib64" -a ! -d "/lib64" )
then
	LIBDIR="lib"
fi
AC_MSG_RESULT($LIBDIR)
])


dnl checks for multiarch platform and sets MULTIARCHDIR to the multiarch name
AC_DEFUN([FW_CHECK_MULTIARCH],
[
AC_MSG_CHECKING(for multiarch platform)
MULTIARCHDIR="`$CC $CPPFLAGS -print-multiarch 2> /dev/null`"
if ( test -n "$MULTIARCHDIR" )
then
	AC_MSG_RESULT($MULTIARCHDIR)
else
	AC_MSG_RESULT(no)
fi
])


dnl checks for microsoft platform.
dnl sets MINGW32, CYGWIN, UWIN, and MICROSOFT to "yes" as appropriate
AC_DEFUN([FW_CHECK_MICROSOFT],
[
AC_MSG_CHECKING(for microsoft platform)
CYGWIN=""
MINGW32=""
UWIN=""
case $host_os in
	*cygwin* )
		CYGWIN="yes"
		AC_MSG_RESULT(cygwin)
		;;
	*mingw32* )
		MINGW32="yes"
		CPPFLAGS="$CPPFLAGS -DLIBRUDIMENTS_EXPORTS"
		AC_MSG_RESULT(mingw32)
		;;
	*uwin* )
		UWIN="yes"
		AC_MSG_RESULT(uwin)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
EXE=""

MICROSOFT=""
if ( test "$UWIN" = "yes" -o "$MINGW32" = "yes" -o "$CYGWIN" = "yes" )
then
	MICROSOFT="yes"
	EXE=".exe"
fi

AC_SUBST(EXE)

if ( test "$MINGW32" )
then
	AC_DEFINE(MINGW32,1,Mingw32 environment)

	dnl if we're building mingw32, we're cross-compiling by definition
	cross_compiling="yes"
fi
])


dnl checks for Mac OS X platform
dnl sets DARWIN to "yes" as appropriate
dnl sets SHELL="...path to bash shell..." if the bash shell is available
dnl sets a flag to enable some gcc 2.95.2 hacks if we're using it
AC_DEFUN([FW_CHECK_OSX],
[
DARWIN=""
AC_MSG_CHECKING(for OSX)
case $host_os in
	*darwin* )
		DARWIN="yes"
		AC_MSG_RESULT(yes)
		FW_CHECK_WNOLONGDOUBLE

		dnl prefer bash to the default shell, which could be tcsh or
		dnl zsh on older versions, and which doesn't run libtool very
		dnl well
		BASH=`which bash`
		if ( test -n "$BASH" )
		then
			SHELL="$BASH"
			AC_SUBST(SHELL)
		fi

		dnl detect gcc 2.95.2 and enable some hacks
		AC_MSG_CHECKING(for gcc 2.95.2)
		CXX_VERSION=`$CXX --version`
		if ( test "$CXX_VERSION" = "2.95.2" )
		then
			AC_MSG_RESULT(yes)
			AC_DEFINE_UNQUOTED(DARWIN_GCC_2952_HACKS,1,Enable some hacks on Darwin with gcc 2.95.2)
		else
			AC_MSG_RESULT(no)
		fi
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
])


dnl checks to see if the compiler allows the -Wno-long-double option
dnl if it does, then WNOLONGDOUBLE="-Wno-long-double" is set
dnl if it does not, then WNOLONGDOUBLE="" is set
AC_DEFUN([FW_CHECK_WNOLONGDOUBLE],
[
AC_MSG_CHECKING(for -Wno-long-double option)
FW_TRY_LINK([#include <stdio.h>],[printf("hello");],[-Wno-long-double],[],[],[WNOLONGDOUBLE="-Wno-long-double"],[WNOLONGDOUBLE=""])
if ( test -n "$WNOLONGDOUBLE" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
AC_SUBST(WNOLONGDOUBLE)
])


dnl checks for minix platform and adds some defines to CPPFLAGS if it is
AC_DEFUN([FW_CHECK_MINIX],
[
AC_MSG_CHECKING(for minix)
case $host_os in
	*minix* )
		CPPFLAGS="$CPPFLAGS -D_MINIX -D_POSIX_SOURCE -D_NETBSD_SOURCE -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED"
		AC_DEFINE(RUDIMENTS_DISABLE_FIONBIO,1,ioctl/FIONBIO doesn't work on some systems)
		AC_MSG_RESULT(yes)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
])


dnl checks for haiku platform
dnl if it is, then prefix="/boot/common" is set unless it was already set,
dnl BELIB="-lbe", and GNULIB="-lgnu" are also set
AC_DEFUN([FW_CHECK_HAIKU],
[
AC_MSG_CHECKING(for haiku)
BELIB=""
GNULIB=""
case $host_os in
	*haiku* )
		if ( test "$prefix" = "NONE" )
		then
			prefix="/boot/common"
		fi
		BELIB="-lbe"
		GNULIB="-lgnu"
		AC_MSG_RESULT(yes)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
AC_SUBST(BELIB)
AC_SUBST(GNULIB)
])


dnl checks for syllable platform
dnl if it is, then prefix="/resources/firstworks" is set unless it was already
dnl set, RUDIMENTS_DISABLE_FIONBIO=1 is defined, and _SYLLABLE=1 is defined
AC_DEFUN([FW_CHECK_SYLLABLE],
[
AC_MSG_CHECKING(for syllable)
case $host_os in
	*syllable* )
		if ( test "$prefix" = "NONE" )
		then
			prefix="/resources/firstworks"
		fi
		AC_DEFINE(RUDIMENTS_DISABLE_FIONBIO,1,ioctl/FIONBIO doesn't work on some systems)
		AC_DEFINE(_SYLLABLE,1,Syllable OS)
		AC_MSG_RESULT(yes)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
])


dnl checks for SCO OpenServer platform
dnl if it is then:
dnl 	sets RUDIMENTS_HAVE_SCO="yes"
dnl	sets RUDIMENTS_HAVE_SCO_OSR5="yes"
dnl	sets ENABLE_RUDIMENTS_THREADS="no"
dnl	defines RUDIMENTS_HAVE_SCO_AVENRUN=1
dnl if it's 6.0.0 then:
dnl 	adds -D__STDC__=0 to CPPFLAGS
dnl 	sets CRTLIB="-lcrt"
dnl if it's < 6.0.0 then:
dnl	defines RUDIMENTS_HAVE_BAD_SCO_MSGHDR=1
dnl if it's 5.0.0 then:
dnl 	adds -D_SVID3 to CPPFLAGS
AC_DEFUN([FW_CHECK_SCO_OSR],
[
RUDIMENTS_HAVE_SCO=""
RUDIMENTS_HAVE_SCO_OSR5=""
CRTLIB=""

AC_MSG_CHECKING(for SCO OSR)
if ( test "`uname -s`" = "SCO_SV" )
then
	AC_MSG_RESULT(yes)

	RUDIMENTS_HAVE_SCO="yes"
	RUDIMENTS_HAVE_SCO_OSR5="yes"

	dnl you can add FSU Pthreads to OSR5 but they cause odd problems
	ENABLE_RUDIMENTS_THREADS="no"

  	AC_DEFINE(RUDIMENTS_HAVE_SCO_AVENRUN,1,SCO has /dev/table/avenrun instead of getloadavg)

	AC_MSG_CHECKING(for SCO OSR < 6.0.0)
	if ( test "`uname -v | tr -d '.'`" -lt "600" )
	then
		AC_MSG_RESULT(yes)

  		AC_DEFINE(RUDIMENTS_HAVE_BAD_SCO_MSGHDR,1,SCO OSR5 has an incorrect struct msghdr definition)

		dnl OSR 5.0.0 needs -D_SVID3
		if ( test "`uname -v`" = "2" )
		then
			CPPFLAGS="$CPPFLAGS -D_SVID3"
		fi
	else
		AC_MSG_RESULT(no)

		AC_MSG_CHECKING(for SCO OSR = 6.0.0)
		if ( test "`uname -v | tr -d '.'`" -eq "600" )
		then
			CPPFLAGS="$CPPFLAGS -D__STDC__=0"
			CRTLIB="-lcrt"
			AC_MSG_RESULT(yes)
		else
			AC_MSG_RESULT(no)
		fi
	fi
else
	AC_MSG_RESULT(no)
fi

AC_SUBST(CRTLIB)
])


dnl checks for Irix platform
dnl if it is then:
dnl 	adds -D_XOPEN_SOURCE=500 to CPPFLAGS
dnl if also using the native compiler then:
dnl 	adds -D__SGICXX -diag_error 1035 -LANG:ansi-for-init-scope=on to
dnl 	CPPFLAGS
AC_DEFUN([FW_CHECK_IRIX],
[
AC_MSG_CHECKING(for irix)
case $host_os in
	*irix* )
		CPPFLAGS="$CPPFLAGS -D_XOPEN_SOURCE=500"
		if ( test "$CXX" = "CC" )
		then
			# define __SGICXX and interpret #error as an error
			CPPFLAGS="$CPPFLAGS -D__SGICXX -diag_error 1035 -LANG:ansi-for-init-scope=on"
		fi
		AC_MSG_RESULT(yes)
		;;
	* )
		AC_MSG_RESULT(no)
		;;
esac
])


dnl checks if the compiler supports the inline keyword
dnl if it does, then INLINE="inline" is set
dnl if it does not, then INLINE="" is set
AC_DEFUN([FW_CHECK_INLINE],
[
AC_MSG_CHECKING(for inline)
INLINE="inline"
dnl intel optimizing compiler doesn't have inlines, assume that CC doesn't
dnl either even though it might, this test needs to be more robust
if ( test "$CXX" = "icc" -o "$CXX" = "CC" )
then
	INLINE=""
else 
	dnl redhat's gcc 2.96 has problems with inlines
	CXX_VERSION=`$CXX --version`
	if ( test "$CXX_VERSION" = "2.96" )
	then
		INLINE=""
	fi
fi
if ( test "$INLINE" = "inline" )
then
	AC_MSG_RESULT(yes)
else
	AC_MSG_RESULT(no)
fi
AC_DEFINE_UNQUOTED(INLINE,$INLINE,Some compliers dont support the inline keyword)
])
