top_builddir = ..

include ../config.mk

.SUFFIXES: .wt .html

SRCS = betterc.wt \
	compatibility.wt \
	datastructures.wt \
	documentation.wt \
	download.wt \
	frameworks.wt \
	installing.wt \
	license.wt \
	lightweight.wt \
	prerequisites.wt \
	utility.wt

OBJS = betterc.html \
	compatibility.html \
	datastructures.html \
	documentation.html \
	download.html \
	frameworks.html \
	installing.html \
	license.html \
	lightweight.html \
	prerequisites.html \
	utility.html

.wt.html:
	wt -i $< -o $@

all: $(OBJS)
	$(CHMOD) 755 $(OBJS)

clean:
	$(RM) $(OBJS)

install:
	$(MKINSTALLDIRS) $(docdir)
	$(CP) .htaccess $(docdir)
	$(CHMOD) 644 $(docdir)/.htaccess
	$(CP) *.html $(docdir)
	$(CHMOD) 755 $(docdir)/*.html
	$(MKINSTALLDIRS) $(docdir)/cards
	$(CP) cards/* $(docdir)/cards
	$(CHMOD) 755 $(docdir)/cards/*
	$(MKINSTALLDIRS) $(docdir)/classes/html
	$(CP) classes/html/a*.html $(docdir)/classes/html
	$(CP) classes/html/b*.html $(docdir)/classes/html
	$(CP) classes/html/c*.html $(docdir)/classes/html
	$(CP) classes/html/d*.html $(docdir)/classes/html
	$(CP) classes/html/e*.html $(docdir)/classes/html
	$(CP) classes/html/f*.html $(docdir)/classes/html
	$(CP) classes/html/g*.html $(docdir)/classes/html
	$(CP) classes/html/h*.html $(docdir)/classes/html
	$(CP) classes/html/i*.html $(docdir)/classes/html
	$(CP) classes/html/j*.html $(docdir)/classes/html
	$(CP) classes/html/l*.html $(docdir)/classes/html
	$(CP) classes/html/m*.html $(docdir)/classes/html
	$(CP) classes/html/n*.html $(docdir)/classes/html
	$(CP) classes/html/o*.html $(docdir)/classes/html
	$(CP) classes/html/p*.html $(docdir)/classes/html
	$(CP) classes/html/r*.html $(docdir)/classes/html
	$(CP) classes/html/s*.html $(docdir)/classes/html
	$(CP) classes/html/t*.html $(docdir)/classes/html
	$(CP) classes/html/u*.html $(docdir)/classes/html
	$(CP) classes/html/w*.html $(docdir)/classes/html
	$(CP) classes/html/x*.html $(docdir)/classes/html
	$(CP) classes/html/*.png $(docdir)/classes/html
	$(CP) classes/html/*.css $(docdir)/classes/html
	$(CP) classes/html/*.js $(docdir)/classes/html
	$(CHMOD) 644 $(docdir)/classes/html/a*.html
	$(CHMOD) 644 $(docdir)/classes/html/b*.html
	$(CHMOD) 644 $(docdir)/classes/html/c*.html
	$(CHMOD) 644 $(docdir)/classes/html/d*.html
	$(CHMOD) 644 $(docdir)/classes/html/e*.html
	$(CHMOD) 644 $(docdir)/classes/html/f*.html
	$(CHMOD) 644 $(docdir)/classes/html/g*.html
	$(CHMOD) 644 $(docdir)/classes/html/h*.html
	$(CHMOD) 644 $(docdir)/classes/html/i*.html
	$(CHMOD) 644 $(docdir)/classes/html/j*.html
	$(CHMOD) 644 $(docdir)/classes/html/l*.html
	$(CHMOD) 644 $(docdir)/classes/html/m*.html
	$(CHMOD) 644 $(docdir)/classes/html/n*.html
	$(CHMOD) 644 $(docdir)/classes/html/o*.html
	$(CHMOD) 644 $(docdir)/classes/html/p*.html
	$(CHMOD) 644 $(docdir)/classes/html/r*.html
	$(CHMOD) 644 $(docdir)/classes/html/s*.html
	$(CHMOD) 644 $(docdir)/classes/html/t*.html
	$(CHMOD) 644 $(docdir)/classes/html/u*.html
	$(CHMOD) 644 $(docdir)/classes/html/w*.html
	$(CHMOD) 644 $(docdir)/classes/html/x*.html
	$(CHMOD) 644 $(docdir)/classes/html/*.png
	$(CHMOD) 644 $(docdir)/classes/html/*.css
	$(CHMOD) 644 $(docdir)/classes/html/*.js
	$(MKINSTALLDIRS) $(docdir)/css
	$(CP) css/* $(docdir)/css
	$(CHMOD) 644 $(docdir)/css/*
	$(MKINSTALLDIRS) $(docdir)/images
	$(CP) images/* $(docdir)/images
	$(CHMOD) 644 $(docdir)/images/*
	$(MKINSTALLDIRS) $(docdir)/js
	$(CP) js/* $(docdir)/js
	$(CHMOD) 644 $(docdir)/js/*
	$(MKINSTALLDIRS) $(docdir)/licenses
	$(CP) licenses/* $(docdir)/licenses
	$(CHMOD) 644 $(docdir)/licenses/*
	$(MKINSTALLDIRS) $(docdir)/programming
	$(CP) programming/*.html $(docdir)/programming
	$(CHMOD) 755 $(docdir)/programming/*
	$(MKINSTALLDIRS) $(docdir)/programming/js
	$(CP) programming/js/* $(docdir)/programming/js
	$(CHMOD) 644 $(docdir)/programming/js/*
	$(MKINSTALLDIRS) $(docdir)/slides
	$(CP) slides/* $(docdir)/slides
	$(CHMOD) 755 $(docdir)/slides/*

uninstall:
	$(RMTREE) $(docdir)
