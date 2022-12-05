top_builddir = ..

include ../config.mk

CPPFLAGS = $(BASECPPFLAGS) $(INC)../ $(INC)../include
LIBS=$(RUDIMENTSLIB) $(SOCKETLIBS) $(PTHREADLIB) $(LIBEDITLIBS) $(SSLLIBS) $(GSSLIBS) $(PCRELIBS) $(LIBCURLLIBS) $(NANOSLEEPLIB) $(CLOCKNANOSLEEPLIB) $(CRYPTLIB) $(INETATONLIB) $(BELIB) $(GNULIB) $(CRTLIB) $(DLLIB) $(GETPGIDLIB) $(MASLIB) $(DEBUGLIB)

PROGS = charsets \
	vswprintf

.SUFFIXES: .lo .obj .cpp

.cpp.lo:
	$(LTCOMPILE) $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $< $(OUT)$@

.cpp.obj:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMPILE) $<

all: $(PROGS)

clean:
	$(LTCLEAN) $(RM) $(PROGS)
	$(LTCLEAN) $(RM) *.exe *.ilk *.idb *.pdb *.manifest *.lo *.o *.obj *.ii

charsets: charsets.cpp charsets.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) charsets.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)

vswprintf: vswprintf.cpp vswprintf.$(OBJ) 
	$(LTLINK) $(LINK) $(OUT)$@$(EXE) vswprintf.$(OBJ) $(ALLSTATIC) $(LDFLAGS) $(LIBS)
