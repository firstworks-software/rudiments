#!/bin/sh

echo "Rudiments test results:" > testresults.log
echo >> testresults.log

exitcode=0

for program in \
	character \
	charstring \
	commandline \
	csvdom \
	datetime \
	des \
	dictionary \
	directory \
	domevents \
	dynamicarray \
	dynamiclib \
	environment \
	error \
	file \
	filesystem \
	groupentry \
	handoff \
	hostentry \
	inidom \
	jsondom \
	linkedlist \
	listener \
	locale \
	logger \
	md5sum \
	memorymap \
	memorypool \
	mvcproperties \
	parameterstring \
	permissions \
	process \
	propdom \
	protocolentry \
	randomnumber \
	regularexpression \
	resourcepool \
	scalar \
	semaphoreset \
	sensitivevalue \
	serviceentry \
	sharedmemory \
	signal \
	singlylinkedlist \
	snooze \
	staticarray \
	stringbuffer \
	sys \
	table \
	thread \
	threadmutex \
	ucs2character \
	ucs2charstring \
	ucs2stringbuffer \
	url \
	userentry \
	wcharacter \
	wcharstring \
	wstringbuffer \
	xmldom
do
	./$program

	if ( test "$?" = "0" )
	then
		printf "    % 20s: success\n" "$program" >> testresults.log
	else
		printf "    % 20s: failed\n" "$program" >> testresults.log

		# handoff is black magic, and my implementation doesn't work
		# on a lot of systems.  We do want to report that it failed,
		# but for now, we won't consider it a general failure if it
		# doesn't work.
		if ( test "$program" != "handoff" )
		then
			exitcode=1
		fi
	fi
done

cat testresults.log

exit $exitcode
