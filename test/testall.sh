#!/bin/sh

echo "Rudiments test results:" > testresults.log
echo >> testresults.log

for program in \
	aes128 \
	avltree \
	bytebuffer \
	bytestring \
	character \
	charstring \
	commandline \
	crypt \
	csvdom \
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
	xmldom
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
