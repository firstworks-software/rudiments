top_builddir = ../..

include ../../config.mk

SLN = setupx86.sln
VDPROJ = setupx86.vdproj
SKEL = setupx86.vdproj.skel
DAT = setupx86-vdproj.dat
STAGE = $(MAKEDIR)\stage
PRODROOT = $(STAGE)\C\Program Files\Firstworks

DEVENV = "$(VS100COMNTOOLS)..\IDE\devenv.com"

all: release

release: $(VDPROJ)
	$(DEVENV) $(SLN) /build Release

debug: $(VDPROJ)
	$(DEVENV) $(SLN) /build Debug

$(VDPROJ): $(SKEL) $(DAT) gen-vdproj.vbs
	-$(RMTREE) stage
	cd /d $(top_builddir) $(AND) set DESTDIR=$(STAGE)$(AND) $(MAKE) install
	$(MKINSTALLDIRS) "$(PRODROOT)\doc"
	$(CP) $(top_builddir)\doc\rudiments-icon.ico "$(PRODROOT)\doc"
	cscript /nologo gen-vdproj.vbs $(SKEL) $(DAT) stage $(VDPROJ) $(RUDIMENTS_VERSION)

clean:
	-$(RMTREE) Release Debug stage
	-$(RM) $(VDPROJ)
