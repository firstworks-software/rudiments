dnl checks for the rudiments library
dnl requires:
dnl 	cross_compiling, RUDIMENTSPATH
dnl sets:
dnl	RUDIMENTSPATH, RUDIMENTSINCLUDES, RUDIMENTSLIBS, RUDIMENTSLIBSPATH
AC_DEFUN([FW_CHECK_RUDIMENTS],
[

RUDIMENTSVERSION=""
RUDIMENTSLIBS=""
RUDIMENTSLIBSPATH=""
RUDIMENTSINCLUDES=""

if ( test "$cross_compiling" = "yes" )
then

	dnl cross compiling
	echo "cross compiling"
	if ( test -n "$RUDIMENTSPATH" )
	then
		RUDIMENTSCONFIG="$RUDIMENTSPATH/bin/rudiments-config"
		if ( test -r "$RUDIMENTSCONFIG" )
		then
			RUDIMENTSINCLUDES="`$RUDIMENTSCONFIG --cflags`"
			RUDIMENTSLIBS="`$RUDIMENTSCONFIG --libs`"
		else
			RUDIMENTSINCLUDES="-I$RUDIMENTSPATH/include"
			RUDIMENTSLIBS="-L$RUDIMENTSPATH/lib -lrudiments"
		fi
	fi

else

	FW_SEARCH_PATHS([$RUDIMENTSPATH],[rudiments],[bin],[SEARCHPATHS])
	for i in $SEARCHPATHS
	do
		RUDIMENTSCONFIG="$i/rudiments-config"
		if ( test -r "$RUDIMENTSCONFIG" )
		then
			RUDIMENTSVERSION="`$RUDIMENTSCONFIG --version`"
			RUDIMENTSINCLUDES="`$RUDIMENTSCONFIG --cflags`"
			RUDIMENTSLIBS="`$RUDIMENTSCONFIG --libs`"
		fi
		if ( test -n "$RUDIMENTSLIBS" )
		then
			break
		fi
	done
fi

if ( test -z "$RUDIMENTSLIBS" )
then
	AC_MSG_ERROR(Rudiments not found.  SQL-Relay requires this package.)
	exit
fi

if ( test -n "$RUDIMENTSVERSION" )
then
	V1=`echo $RUDIMENTSVERSION | cut -d. -f1`
	V2=`echo $RUDIMENTSVERSION | cut -d. -f2`
	V3=`echo $RUDIMENTSVERSION | cut -d. -f3`
	if ( test "$V1" -lt "2")
	then
		AC_MSG_ERROR([Rudiments version must be >= 2.0.0, found version $RUDIMENTSVERSION])
		exit
	fi
fi

FW_INCLUDES(rudiments,[$RUDIMENTSINCLUDES])
FW_LIBS(rudiments,[$RUDIMENTSLIBS])

AC_SUBST(RUDIMENTSPATH)
AC_SUBST(RUDIMENTSINCLUDES)
AC_SUBST(RUDIMENTSLIBS)
AC_SUBST(RUDIMENTSLIBSPATH)
])
