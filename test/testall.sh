#!/bin/sh

echo "Rudiments test results:" > testresults.log
echo >> testresults.log

exitcode=0

for program in \
	aes128 \
	avltree \
	bytebuffer \
	bytestring \
	character \
	wcharacter \
	charstring \
	wcharstring \
	commandline \
	crypt \
	csvdom \
	datetime \
	dictionary \
	directory \
	dynamicarray \
	dynamictable \
	dynamiclib \
	environment \
	error \
	file \
	groupentry \
	handoff \
	hostentry \
	inidom \
	jsondom \
	linkedlist \
	listener \
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
	scalar \
	semaphoreset \
	serviceentry \
	sharedmemory \
	signal \
	singlylinkedlist \
	snooze \
	sparsedynamictable \
	staticarray \
	stringbuffer \
	wstringbuffer \
	sys \
	thread \
	threadmutex \
	url \
	userentry \
	xmldom
do
	./$program

	if ( test "$?" = "0" )
	then
		printf "    % 20s: success\n" "$program" >> testresults.log
	else
		printf "    % 20s: failed\n" "$program" >> testresults.log
		exitcode=1
	fi
done

cat testresults.log

exit $exitcode
