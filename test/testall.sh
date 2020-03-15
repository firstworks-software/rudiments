#!/bin/sh

echo "Rudiments test results:" > testresults.log
echo >> testresults.log

for program in \
	bytebuffer \
	bytestring \
	character \
	charstring \
	commandline \
	crypt \
	datetime \
	dictionary \
	directory \
	dynamicarray \
	dynamiclib \
	environment \
	error \
	file \
	groupentry \
	handoff \
	hostentry \
	linkedlist \
	listener \
	logger \
	memorymap \
	memorypool \
	parameterstring \
	permissions \
	process \
	protocolentry \
	randomnumber \
	regularexpression \
	semaphoreset \
	serviceentry \
	sharedmemory \
	signal \
	singlylinkedlist \
	snooze \
	staticarray \
	stringbuffer \
	sys \
	thread \
	threadmutex \
	url \
	userentry \
	xmldom \
	csvdom \
	jsondom \
	inidom \
	propdom
do
	./$program

	if ( test "$?" = "0" )
	then
		printf "    % 20s: success\n" "$program" >> testresults.log
	else
		printf "    % 20s: failed\n" "$program" >> testresults.log
	fi
done

cat testresults.log
