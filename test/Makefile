top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(LIBEDITLIBS) $(SSLLIBS) $(GSSLIBS) $(PCRELIBS) $(LIBCURLLIBS) $(LIBICONVLIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(GETPGIDLIB) $(MASLIB) $(DEBUGLIB)

PROGS0 = listener \
	directory \
	groupentry \
	commandline \
	parameterstring \
	hostentry \
	userentry \
	serviceentry \
	permissions \
	protocolentry \
	regularexpression \
	semaphoreset \
	charstring \
	wcharstring \
	ucs2charstring \
	datetime

PROGS1= dictionary \
	environment \
	file \
	filesystem \
	linkedlist \
	avltree \
	singlylinkedlist \
	listmetrics \
	memorypool \
	prng \
	csprng \
	sharedmemory \
	signal \
	kill \
	stringbuffer \
	wstringbuffer \
	ucs2stringbuffer \
	bytebuffer \
	des

PROGS2= dynamicarray \
	staticarray \
	dynamiclib \
	error \
	thread \
	threadmutex \
	process \
	snooze \
	memorymap \
	character \
	wcharacter \
	ucs2character \
	bytestring \
	tlsserver \
	tlsclient

PROGS3= handoff \
	xmldom \
	logger \
	sys \
	reboot \
	halt \
	shutdown \
	url \
	gssserver \
	gssclient \
	md5 \
	sha1 \
	sha256 \
	sha512 \
	csvdom \
	jsondom \
	cgi.cgi \
	headers.cgi \
	multipart.cgi \
	pr.cgi \
	inidom \
	propdom \
	mvcproperties \
	aes128 \
	aes192 \
	aes256 \
	pbkdf2 \
	dh \
	arc4 \
	bignumber \
	table \
	scalar \
	sensitivevalue \
	resourcepool \
	domevents \
	locale \
	iconvert \
	collectionprint \
	client \
	server

PROGS = $(PROGS0) $(PROGS1) $(PROGS2) $(PROGS3)

#ALLSTATIC=-all-static 

.SUFFIXES: .lo .obj .cpp

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS) $(SERVERPEM)

tests: all
	$(SCRIPTINT) $(THISDIR)testall$(SCRIPTEXT)
	
clean:
	$(RM) $(PROGS0)
	$(RM) $(PROGS1)
	$(RM) $(PROGS2)
	$(RM) $(PROGS3)
	$(RM) *.exe *.ilk *.idb *.pdb *.manifest *.lo *.o *.obj test.log testfile *.pem *.pfx *.xml *.sck *.ii *.csv *.json *.ini .prop handofffile *.cgi app1/app2/*.cgi semout semkey
	$(RMTREE) .libs
	cd certificates $(AND) $(MAKE) clean

tlsserver: tlsserver.cpp tlsserver.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) tlsserver.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

listener: listener.cpp listener.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listener.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

tlsclient: tlsclient.cpp tlsclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) tlsclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

directory: directory.cpp directory.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) directory.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

commandline: commandline.cpp commandline.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) commandline.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

parameterstring: parameterstring.cpp parameterstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) parameterstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

logger: logger.cpp logger.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) logger.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

environment: environment.cpp environment.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) environment.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

file: file.cpp file.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) file.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

filesystem: filesystem.cpp filesystem.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) filesystem.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

groupentry: groupentry.cpp groupentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) groupentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

hostentry: hostentry.cpp hostentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) hostentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

linkedlist: linkedlist.cpp linkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) linkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

avltree: avltree.cpp avltree.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) avltree.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

singlylinkedlist: singlylinkedlist.cpp singlylinkedlist.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) singlylinkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

listmetrics: listmetrics.cpp listmetrics.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) listmetrics.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

memorypool: memorypool.cpp memorypool.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorypool.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

userentry: userentry.cpp userentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) userentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

serviceentry: serviceentry.cpp serviceentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) serviceentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

permissions: permissions.cpp permissions.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) permissions.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

protocolentry: protocolentry.cpp protocolentry.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) protocolentry.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

regularexpression: regularexpression.cpp regularexpression.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) regularexpression.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

semaphoreset: semaphoreset.cpp semaphoreset.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) semaphoreset.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sharedmemory: sharedmemory.cpp sharedmemory.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sharedmemory.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

charstring: charstring.cpp charstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) charstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

wcharstring: wcharstring.cpp wcharstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) wcharstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

ucs2charstring: ucs2charstring.cpp ucs2charstring.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) ucs2charstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

datetime: datetime.cpp datetime.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) datetime.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dictionary: dictionary.cpp dictionary.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dictionary.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

prng: prng.cpp prng.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) prng.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

csprng: csprng.cpp csprng.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) csprng.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

signal: signal.cpp signal.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) signal.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

kill: kill.cpp kill.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) kill.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

stringbuffer: stringbuffer.cpp stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) stringbuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

wstringbuffer: wstringbuffer.cpp wstringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) wstringbuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

ucs2stringbuffer: ucs2stringbuffer.cpp ucs2stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) ucs2stringbuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

handoff: handoff.cpp handoff.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) handoff.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bytebuffer: bytebuffer.cpp bytebuffer.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytebuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

xmldom: xmldom.cpp xmldom.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmldom.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

des: des.cpp des.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) des.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dynamicarray: dynamicarray.cpp dynamicarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamicarray.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

staticarray: staticarray.cpp staticarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) staticarray.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dynamiclib: dynamiclib.cpp dynamiclib.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamiclib.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

error: error.cpp error.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) error.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

thread: thread.cpp thread.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) thread.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

threadmutex: threadmutex.cpp threadmutex.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) threadmutex.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

process: process.cpp process.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) process.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

snooze: snooze.cpp snooze.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) snooze.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

memorymap: memorymap.cpp memorymap.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorymap.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

character: character.cpp character.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) character.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

wcharacter: wcharacter.cpp wcharacter.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) wcharacter.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

ucs2character: ucs2character.cpp ucs2character.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) ucs2character.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bytestring: bytestring.cpp bytestring.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytestring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sys: sys.cpp sys.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sys.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

reboot: reboot.cpp reboot.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) reboot.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

halt: halt.cpp halt.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) halt.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

shutdown: shutdown.cpp shutdown.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) shutdown.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

url: url.cpp url.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) url.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

gssserver: gssserver.cpp gssserver.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) gssserver.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

gssclient: gssclient.cpp gssclient.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) gssclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

md5: md5.cpp md5.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) md5.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sha1: sha1.cpp sha1.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sha1.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sha256: sha256.cpp sha256.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sha256.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sha512: sha512.cpp sha512.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sha512.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

csvdom: csvdom.cpp csvdom.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) csvdom.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

jsondom: jsondom.cpp jsondom.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) jsondom.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

cgi.cgi: cgi.cpp cgi.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) cgi.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

headers.cgi: headers.cpp headers.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) headers.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

multipart.cgi: multipart.cpp multipart.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) multipart.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

pr.cgi: pr.cpp pr.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) pr.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)
	$(CP) -f $@ app1/app2

inidom: inidom.cpp inidom.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inidom.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

propdom: propdom.cpp propdom.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) propdom.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

mvcproperties: mvcproperties.cpp mvcproperties.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) mvcproperties.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

aes128: aes128.cpp aes128.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) aes128.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

aes192: aes192.cpp aes192.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) aes192.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

aes256: aes256.cpp aes256.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) aes256.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

pbkdf2: pbkdf2.cpp pbkdf2.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) pbkdf2.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dh: dh.cpp dh.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dh.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

arc4: arc4.cpp arc4.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) arc4.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bignumber: bignumber.cpp bignumber.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bignumber.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

table: table.cpp table.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) table.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

scalar: scalar.cpp scalar.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) scalar.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

sensitivevalue: sensitivevalue.cpp sensitivevalue.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) sensitivevalue.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

resourcepool: resourcepool.cpp resourcepool.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) resourcepool.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

domevents: domevents.cpp domevents.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) domevents.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

locale: locale.cpp locale.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) locale.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

iconvert: iconvert.cpp iconvert.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) iconvert.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

collectionprint: collectionprint.cpp collectionprint.$(OBJ)
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) collectionprint.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

server: server.cpp server.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) server.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

client: client.cpp client.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) client.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

server.pem:
	cd certificates $(AND) $(MAKE) clean all
	cp certificates/certificatechains/ca.pem .
	cp certificates/certificatechains/server.pem .
	cp certificates/certificatechains/client.pem .
	cp certificates/certificatechains/ca.pfx .
	cp certificates/certificatechains/server.pfx .
	cp certificates/certificatechains/client.pfx .
