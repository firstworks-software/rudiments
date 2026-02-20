top_builddir = ../..

include ../../config.mk

.SUFFIXES: .wt .html

SRCS = aes128.wt \
	avltree.wt \
	bytebuffer.wt \
	bytestring.wt \
	character.wt \
	charstring.wt \
	chat.wt \
	codetree.wt \
	commandline.wt \
	compiler.wt \
	csvdom.wt \
	csvsax.wt \
	datetime.wt \
	des.wt \
	device.wt \
	dh.wt \
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
	hostentry.wt \
	inidom.wt \
	inisax.wt \
	inetsocketclient.wt \
	inetsocketserver.wt \
	jsondom.wt \
	jsonsax.wt \
	linkedlist.wt \
	listener.wt \
	logger.wt \
	md5.wt \
	memorymap.wt \
	memorypool.wt \
	parameterstring.wt \
	pbkdf2.wt \
	permissions.wt \
	process.wt \
	prompt.wt \
	propdom.wt \
	propsax.wt \
	protocolentry.wt \
	randomnumber.wt \
	regularexpression.wt \
	scalar.wt \
	sensitivevalue.wt \
	semaphoreset.wt \
	serviceentry.wt \
	sha1.wt \
	sha256.wt \
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

OBJS = aes128.html \
	avltree.html \
	bytebuffer.html \
	bytestring.html \
	character.html \
	charstring.html \
	chat.html \
	codetree.html \
	commandline.html \
	compiler.html \
	csvdom.html \
	csvsax.html \
	datetime.html \
	des.html \
	device.html \
	dh.html \
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
	hostentry.html \
	inidom.html \
	inisax.html \
	inetsocketclient.html \
	inetsocketserver.html \
	jsondom.html \
	jsonsax.html \
	linkedlist.html \
	listener.html \
	logger.html \
	md5.html \
	memorymap.html \
	memorypool.html \
	parameterstring.html \
	pbkdf2.html \
	permissions.html \
	process.html \
	prompt.html \
	propdom.html \
	propsax.html \
	protocolentry.html \
	randomnumber.html \
	regularexpression.html \
	scalar.html \
	sensitivevalue.html \
	semaphoreset.html \
	serviceentry.html \
	sha1.html \
	sha256.html \
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
