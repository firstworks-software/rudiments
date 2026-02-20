top_builddir = ../..

include ../../config.mk

.SUFFIXES: .wt .html

SRCS = avltree.wt \
	bytebuffer.wt \
	bytestring.wt \
	character.wt \
	charstring.wt \
	chat.wt \
	codetree.wt \
	commandline.wt \
	compiler.wt \
	datetime.wt \
	device.wt \
	dictionary.wt \
	directory.wt \
	domevents.wt \
	dynamicarray.wt \
	dynamiclib.wt \
	environment.wt \
	error.wt \
	filesystem.wt \
	file.wt \
	groupentry.wt \
	gss.wt \
	hash.wt \
	hostentry.wt \
	inetsocketclient.wt \
	inetsocketserver.wt \
	linkedlist.wt \
	listener.wt \
	logger.wt \
	memorymap.wt \
	memorypool.wt \
	parameterstring.wt \
	permissions.wt \
	process.wt \
	prompt.wt \
	protocolentry.wt \
	randomnumber.wt \
	regularexpression.wt \
	scalar.wt \
	semaphoreset.wt \
	serviceentry.wt \
	sharedmemory.wt \
	signal.wt \
	singlylinkedlist.wt \
	snooze.wt \
	staticarray.wt \
	stdio.wt \
	stringbuffer.wt \
	sys.wt \
	table.wt \
	threadmutex.wt \
	thread.wt \
	tls.wt \
	unixsocketclient.wt \
	unixsocketserver.wt \
	url.wt \
	userentry.wt \
	xmldom.wt \
	xmlsax.wt

OBJS = avltree.html \
	bytebuffer.html \
	bytestring.html \
	character.html \
	charstring.html \
	chat.html \
	codetree.html \
	commandline.html \
	compiler.html \
	datetime.html \
	device.html \
	dictionary.html \
	directory.html \
	domevents.html \
	dynamicarray.html \
	dynamiclib.html \
	environment.html \
	error.html \
	filesystem.html \
	file.html \
	groupentry.html \
	gss.html \
	hash.html \
	hostentry.html \
	inetsocketclient.html \
	inetsocketserver.html \
	linkedlist.html \
	listener.html \
	logger.html \
	memorymap.html \
	memorypool.html \
	parameterstring.html \
	permissions.html \
	process.html \
	prompt.html \
	protocolentry.html \
	randomnumber.html \
	regularexpression.html \
	scalar.html \
	semaphoreset.html \
	serviceentry.html \
	sharedmemory.html \
	signal.html \
	singlylinkedlist.html \
	snooze.html \
	staticarray.html \
	stdio.html \
	stringbuffer.html \
	sys.html \
	table.html \
	threadmutex.html \
	thread.html \
	tls.html \
	unixsocketclient.html \
	unixsocketserver.html \
	url.html \
	userentry.html \
	xmldom.html \
	xmlsax.html

.wt.html:
	wt -i $< -o $@

all: $(OBJS)
	$(CHMOD) 755 $(OBJS)

clean:
	$(RM) $(OBJS)
