top_builddir = .

include config.mk

all:
	cd src $(AND) $(MAKE) all

clean:
	cd src $(AND) $(MAKE) clean
	cd test $(AND) $(MAKE) clean
	cd bench $(AND) $(MAKE) clean
	cd behaviors $(AND) $(MAKE) clean
	cd examples $(AND) $(MAKE) clean
	$(RMTREE) msvc/setupx86/Release msvc/setupx86/Debug msvc/setupx64/Release msvc/setupx64/Debug

install:
	cd src $(AND) $(MAKE) install
	cd include $(AND) $(MAKE) install
	cd bin $(AND) $(MAKE) install
	cd doc $(AND) $(MAKE) install
	cd man $(AND) $(MAKE) install
	cd autoconf $(AND) $(MAKE) install
	$(MKINSTALLDIRS) $(licensedir)
	$(CP) COPYING $(licensedir)
	$(CHMOD) 0644 $(licensedir)/COPYING
	$(MKINSTALLDIRS) $(libdir)/pkgconfig
	$(CP) rudiments.pc $(libdir)/pkgconfig
	$(CHMOD) 644 $(libdir)/pkgconfig/rudiments.pc

uninstall:
	cd src $(AND) $(MAKE) uninstall
	cd include $(AND) $(MAKE) uninstall
	cd bin $(AND) $(MAKE) uninstall
	cd doc $(AND) $(MAKE) uninstall
	cd man $(AND) $(MAKE) uninstall
	cd autoconf $(AND) $(MAKE) uninstall
	$(RM) $(libdir)/pkgconfig/rudiments.pc
	$(RMTREE) $(licensedir)
	$(RMTREE) $(datadir)/rudiments

distclean: clean
	$(RM) config.mk config.cache config.h config.log config.status features.mk libtool rudiments.pc include/Makefile include/rudiments/private/config.h include/rudiments/private/config.h.in~ bin/rudiments-config src/Makefile msvc/setupx86/rudiments.opensdf msvc/setupx86/rudiments.sdf msvc/setupx86/rudiments.suo msvc/setupx86/setupx86.vdproj msvc/setupx64/rudiments.opensdf msvc/setupx64/rudiments.sdf msvc/setupx64/rudiments.suo msvc/setupx64/setupx64.vdproj configure.lineno configure~ test/certificates/config/ca.cnf test/testresults.log so_locations .nfs*
	$(RMTREE) autom4te.cache

cppcheck:
	cppcheck -j4 --enable=warning,performance,portability src > /dev/null

tests:
	cd test $(AND) $(MAKE) tests

tests-clean:
	cd test $(AND) $(MAKE) clean
