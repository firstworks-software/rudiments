top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include -std=c++11
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(LIBEDITLIBS) $(SSLLIBS) $(GSSLIBS) $(PCRELIBS) $(LIBCURLLIBS) $(LIBICONVLIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(GETPGIDLIB) $(MASLIB) $(DEBUGLIB)

PROGS = dictionary \
	dynamicarray \
	forward_list \
	linkedlist \
	list \
	map \
	singlylinkedlist \
	stringbuffer \
	bytebuffer \
	string \
	vector \
	memorypool \
	randomnumber \
	inetsocketclient \
	inetsocketserver \
	bytestring \
	charstring \
	socket \
	file \
	xmlsax

.SUFFIXES: .lo .obj .cpp

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS)

clean:
	$(RM) $(PROGS) *.exe *.ilk *.idb *.pdb *.manifest *.lo *.o *.obj *.ii
	$(RMTREE) .libs

dictionary: dictionary.cpp dictionary.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dictionary.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

dynamicarray: dynamicarray.cpp dynamicarray.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) dynamicarray.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

forward_list: forward_list.cpp forward_list.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) forward_list.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

linkedlist: linkedlist.cpp linkedlist.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) linkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

list: list.cpp list.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) list.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

map: map.cpp map.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) map.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

singlylinkedlist: singlylinkedlist.cpp singlylinkedlist.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) singlylinkedlist.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

stringbuffer: stringbuffer.cpp stringbuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) stringbuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bytebuffer: bytebuffer.cpp bytebuffer.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytebuffer.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

string: string.cpp string.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) string.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

vector: vector.cpp vector.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) vector.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

memorypool: memorypool.cpp memorypool.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) memorypool.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

randomnumber: randomnumber.cpp randomnumber.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) randomnumber.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

inetsocketclient: inetsocketclient.cpp inetsocketclient.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetsocketclient.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

inetsocketserver: inetsocketserver.cpp inetsocketserver.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) inetsocketserver.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

bytestring: bytestring.cpp bytestring.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) bytestring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

charstring: charstring.cpp charstring.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) charstring.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

socket: socket.cpp socket.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) socket.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

file: file.cpp file.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) file.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

xmlsax: xmlsax.cpp xmlsax.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) xmlsax.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)
