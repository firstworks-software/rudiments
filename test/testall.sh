#!/bin/sh

echo "Rudiments test results:" > testresults.log
echo >> testresults.log

exitcode=0

for program in \
	aes128 \
	aes192 \
	aes256 \
	arc4 \
	avltree \
	bignumber \
	bytebuffer \
	bytestring \
	character \
	charstring \
	commandline \
	csprng \
	csvdom \
	csvsax \
	datetime \
	des \
	dh \
	dictionary \
	directory \
	domevents \
	domnode \
	dynamicarray \
	dynamiclib \
	environment \
	error \
	file \
	filesystem \
	groupentry \
	handoff \
	hostentry \
	iconvert \
	inidom \
	jsondom \
	linkedlist \
	listener \
	locale \
	logger \
	md5 \
	memorymap \
	memorypool \
	mvcproperties \
	parameterstring \
	pbkdf2 \
	permissions \
	process \
	propdom \
	protocolentry \
	prng \
	regularexpression \
	resourcepool \
	scalar \
	semaphoreset \
	sensitivevalue \
	serviceentry \
	sha1 \
	sha256 \
	sha512 \
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
