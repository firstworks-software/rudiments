// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/groupentry.h>
#include <rudiments/userentry.h>
#include <rudiments/file.h>
#include <rudiments/directory.h>
#include <rudiments/permissions.h>
#include <rudiments/datetime.h>
#include <rudiments/stdio.h>
#include <rudiments/process.h>
#include <rudiments/sys.h>
#include <rudiments/snooze.h>
#include <rudiments/error.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/filesystem.h>
#include <rudiments/signalclasses.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/wcharstring.h>
#include <rudiments/ucs2charstring.h>
#include <rudiments/locale.h>
#include <rudiments/memorymap.h>
#include <signal.h>
#include <wchar.h>
#ifndef _WIN32
	#include <unistd.h>
	#include <fcntl.h>
#endif
// a fully static link has no next lseek() to find
#if !defined(_WIN32) && !defined(TEST_ALLSTATIC) && \
	defined(RUDIMENTS_HAVE_DLOPEN) && defined(RUDIMENTS_HAVE_RTLD_NEXT)
	#define FILETEST_LSEEK_HOOK
	#if defined(RUDIMENTS_HAVE_MMAP)
		#define FILETEST_MMAP_HOOK
	#endif
	#include <rudiments/dynamiclib.h>
#endif
// an fstat() hook needs fstat() to be a symbol of the C library (2.33+)
#if defined(FILETEST_LSEEK_HOOK) && defined(__GLIBC__) && \
	(__GLIBC__>2 || (__GLIBC__==2 && __GLIBC_MINOR__>=33))
	#define FILETEST_FSTAT_HOOK
	#include <sys/stat.h>
#endif
#if defined(FILETEST_FSTAT_HOOK) && defined(RUDIMENTS_HAVE_BLKGETSIZE64)
	#define FILETEST_BLOCKDEV_HOOK
	#include <sys/ioctl.h>
	#include <linux/fs.h>
	#include <stdarg.h>
#endif
#include "test.cpp"

#ifndef WIN32
const char	*testfiletxt="/tmp/testfile.txt";
#else
const char	*testfiletxt="testfile.txt";
#endif

#ifndef WIN32
const char	*contentsfile="/tmp/testfilecontents.txt";
#else
const char	*contentsfile="testfilecontents.txt";
#endif

#ifndef WIN32
const char	*flushfile="/tmp/testfileflush.txt";
#else
const char	*flushfile="testfileflush.txt";
#endif

#ifndef WIN32
const char	*readfile="/tmp/testfileread.txt";
#else
const char	*readfile="testfileread.txt";
#endif

#ifndef WIN32
const char	*mmapfile="/tmp/testfilemmap.txt";
#else
const char	*mmapfile="testfilemmap.txt";
#endif

#ifndef WIN32
const char	*mmapfile2="/tmp/testfilemmap2.txt";
#else
const char	*mmapfile2="testfilemmap2.txt";
#endif

#ifndef WIN32
const char	*seekfile="/tmp/testfileseek.txt";
#else
const char	*seekfile="testfileseek.txt";
#endif

#ifndef WIN32
const char	*openfile="/tmp/testfileopen.txt";
#else
const char	*openfile="testfileopen.txt";
#endif

// getContents on files of known and unknown size and on read errors
static void contentsTests() {

	signalmanager::alarm(60);

	file::remove(contentsfile);
	file	fl;
	byte_t	*buf;
	size_t	size;
	char	*str;

	// empty file opened write-only: read fails
	test("create empty",fl.create(contentsfile,
			permissions::parsePermString("rw-rw----")));
	test("close empty",fl.close());
	test("open empty write-only",fl.open(contentsfile,O_WRONLY));
	str=fl.getContents();
	test("write-only empty, char *",!str);
	delete[] str;
	buf=(byte_t *)1;
	size=1;
	test("write-only empty, byte_t **",
			fl.getContents(&buf,&size)==-1);
	test("write-only empty, buffer NULL",!buf);
	test("write-only empty, size 0",!size);
	buf=(byte_t *)1;
	size=1;
	test("write-only empty, no size",
			fl.getContents(&buf,NULL)==-1 && !buf);
	test("write-only empty, no buffer",
			fl.getContents(NULL,&size)==-1 && !size);
	test("write-only empty, repeat",
			fl.getContents(&buf,&size)==-1 && !buf && !size);
	test("close write-only",fl.close());

	// empty file opened read-only: success
	test("open empty read-only",fl.open(contentsfile,O_RDONLY));
	str=fl.getContents();
	test("read-only empty, char *",str && !str[0]);
	delete[] str;
	buf=NULL;
	size=1;
	test("read-only empty, byte_t **",
			fl.getContents(&buf,&size)==0 && !size);
	delete[] buf;
	test("close read-only",fl.close());
	str=file::getContents(contentsfile);
	test("name empty, char *",str && !str[0]);
	delete[] str;
	buf=NULL;
	size=1;
	test("name empty, byte_t **",
			fl.getContents(contentsfile,&buf,&size)==0 &&
			!size);
	delete[] buf;

	// file of known size
	test("create contents",fl.create(contentsfile,
			permissions::parsePermString("rw-rw----")));
	test("write contents",fl.write("hello")==5);
	test("close contents",fl.close());
	str=file::getContents(contentsfile);
	test("name hello, char *",
			str && !charstring::compare(str,"hello"));
	delete[] str;
	buf=NULL;
	size=0;
	test("name hello, byte_t **",
			fl.getContents(contentsfile,&buf,&size)==5 &&
			size==5 && buf && !bytestring::compare(buf,"hello",5));
	delete[] buf;
	test("open hello write-only",fl.open(contentsfile,O_WRONLY));
	str=fl.getContents();
	test("write-only hello, char *",!str);
	delete[] str;
	test("close hello",fl.close());

	// nonexistent file
	str=file::getContents("/tmp/testfilenonexistent.txt");
	test("nonexistent, char *",!str);
	delete[] str;

	#ifdef __linux__
	// size 0 in st_size but has contents
	if (file::exists("/proc/self/status")) {
		str=file::getContents("/proc/self/status");
		test("proc, char *",str &&
			!charstring::compareIgnoringCase(str,"Name:",5));
		delete[] str;
		buf=NULL;
		size=0;
		test("proc, byte_t **",
			fl.getContents("/proc/self/status",&buf,&size)>0 &&
			buf && size>5 &&
			!bytestring::compare(buf,"Name:",5));
		delete[] buf;
	}
	#endif

	// file that is not open
	file	nf;
	str=nf.getContents();
	test("not open, char *",!str);
	delete[] str;
	buf=(byte_t *)1;
	size=1;
	test("not open, byte_t **",nf.getContents(&buf,&size)==-1);
	test("not open, buffer NULL",!buf);
	test("not open, size 0",!size);
	buf=(byte_t *)1;
	test("not open, no size",nf.getContents(&buf,NULL)==-1 && !buf);
	size=1;
	test("not open, no buffer",nf.getContents(NULL,&size)==-1 && !size);
	test("open for close",nf.open(contentsfile,O_RDONLY));
	test("close for not open",nf.close());
	buf=(byte_t *)1;
	size=1;
	test("closed, byte_t **",
			nf.getContents(&buf,&size)==-1 && !buf && !size);
	str=nf.getContents();
	test("closed, char *",!str);
	delete[] str;

	#ifdef __linux__
	// directory: st_size is not 0 but read fails
	if (file::exists("/etc")) {
		test("open directory",fl.open("/etc",O_RDONLY));
		buf=(byte_t *)1;
		size=1;
		test("directory, byte_t **",
			fl.getContents(&buf,&size)==-1 && !buf && !size);
		str=fl.getContents();
		test("directory, char *",!str);
		delete[] str;
		test("close directory",fl.close());
		buf=(byte_t *)1;
		size=1;
		test("directory by name, byte_t **",
			fl.getContents("/etc",&buf,&size)==-1 &&
			!buf && !size);
		str=file::getContents("/etc");
		test("directory by name, char *",!str);
		delete[] str;
	}

	// st_size is more than the data: short read is not an error
	const char	*sysfile="/sys/devices/system/cpu/online";
	if (file::exists(sysfile)) {
		buf=NULL;
		size=0;
		ssize_t	bytes=fl.getContents(sysfile,&buf,&size);
		test("sys, byte_t **",bytes>0 && (size_t)bytes==size && buf);
		test("sys, shorter than st_size",size<4096);
		str=file::getContents(sysfile);
		test("sys, char *",str && charstring::getLength(str)==size &&
			!bytestring::compare(str,buf,size) &&
			!str[size]);
		delete[] str;
		delete[] buf;
		test("open sys",fl.open(sysfile,O_RDONLY));
		buf=NULL;
		size=0;
		test("sys open, byte_t **",
			fl.getContents(&buf,&size)>0 && size>0 && buf);
		delete[] buf;
		test("sys open, position restored",fl.getPosition()==0);
		test("close sys",fl.close());
	}
	#endif

	// file larger than the limit on the size of a buffer
	if (sizeof(size_t)>=8 && file::truncate(contentsfile,
					(((off64_t)1<<62)-1)*2+1)) {
		test("open huge",fl.open(contentsfile,O_RDONLY));
		str=fl.getContents();
		test("huge, char *",!str);
		delete[] str;
		test("close huge",fl.close());
		str=file::getContents(contentsfile);
		test("huge by name, char *",!str);
		delete[] str;
	}

	file::remove(contentsfile);

	signalmanager::alarm(0);
}

// a file descriptor whose lowLevelWrite() can shorten or fail
class failfd : public filedescriptor {
	public:
			failfd(ssize_t mode);
		void	setMode(ssize_t mode);
		// the first count writes pass, the mode applies after them
		void	setPassFirst(uint32_t count);
		// after those, count writes are short, then the mode applies
		void	setShortFirst(uint32_t count);
		// errno that a failed write sets
		void	setFailErrorNumber(int32_t errnum);
		// bytes that lowLevelWrite() wrote
		size_t	getWritten();
	protected:
		ssize_t	lowLevelWrite(const void *buf, size_t count);
	private:
		ssize_t	countWritten(ssize_t result);

		// 0 is normal, 1 is a short write, anything else is returned
		ssize_t		mode;
		uint32_t	passfirst;
		uint32_t	shortfirst;
		int32_t		failerrnum;
		size_t		written;
};

failfd::failfd(ssize_t mode) : filedescriptor() {
	this->mode=mode;
	passfirst=0;
	shortfirst=0;
	failerrnum=0;
	written=0;
}

void failfd::setMode(ssize_t mode) {
	this->mode=mode;
}

void failfd::setPassFirst(uint32_t count) {
	passfirst=count;
}

void failfd::setShortFirst(uint32_t count) {
	shortfirst=count;
}

void failfd::setFailErrorNumber(int32_t errnum) {
	failerrnum=errnum;
}

size_t failfd::getWritten() {
	return written;
}

ssize_t failfd::lowLevelWrite(const void *buf, size_t count) {
	if (passfirst) {
		passfirst--;
		return countWritten(filedescriptor::lowLevelWrite(buf,count));
	}
	if (shortfirst) {
		shortfirst--;
		return countWritten(filedescriptor::lowLevelWrite(buf,count/2));
	}
	if (!mode) {
		return countWritten(filedescriptor::lowLevelWrite(buf,count));
	}
	if (mode==1) {
		return countWritten(filedescriptor::lowLevelWrite(buf,count/2));
	}
	if (mode<0 && failerrnum) {
		error::setErrorNumber(failerrnum);
	}
	return mode;
}

ssize_t failfd::countWritten(ssize_t result) {
	if (result>0) {
		written+=result;
	}
	return result;
}

// a file descriptor or file whose lowLevelRead() can shorten or fail
template <class base>
class failreader : public base {
	public:
			failreader(ssize_t mode);
		void	setMode(ssize_t mode);
		// the first count reads are short, then the mode applies
		void	setShortFirst(uint32_t count);
		// the first count reads meet an end of data, before any others
		void	setEofFirst(uint32_t count);
		// errno that a failed read sets
		void	setFailErrorNumber(int32_t errnum);
		// bytes that lowLevelRead() read
		size_t	getRead();
	protected:
		ssize_t	lowLevelRead(void *buf, size_t count);
	private:
		ssize_t	countRead(ssize_t result);

		// 0 is normal, anything else is returned
		ssize_t		mode;
		uint32_t	shortfirst;
		uint32_t	eoffirst;
		int32_t		failerrnum;
		size_t		bytesread;
};

template <class base>
failreader<base>::failreader(ssize_t mode) : base() {
	this->mode=mode;
	shortfirst=0;
	eoffirst=0;
	failerrnum=0;
	bytesread=0;
}

template <class base>
void failreader<base>::setMode(ssize_t mode) {
	this->mode=mode;
}

template <class base>
void failreader<base>::setShortFirst(uint32_t count) {
	shortfirst=count;
}

template <class base>
void failreader<base>::setEofFirst(uint32_t count) {
	eoffirst=count;
}

template <class base>
void failreader<base>::setFailErrorNumber(int32_t errnum) {
	failerrnum=errnum;
}

template <class base>
size_t failreader<base>::getRead() {
	return bytesread;
}

template <class base>
ssize_t failreader<base>::lowLevelRead(void *buf, size_t count) {
	if (eoffirst) {
		eoffirst--;
		return 0;
	}
	if (shortfirst) {
		shortfirst--;
		return countRead(base::lowLevelRead(buf,count/2));
	}
	if (!mode) {
		return countRead(base::lowLevelRead(buf,count));
	}
	if (mode<0 && failerrnum) {
		error::setErrorNumber(failerrnum);
	}
	return mode;
}

template <class base>
ssize_t failreader<base>::countRead(ssize_t result) {
	if (result>0) {
		bytesread+=result;
	}
	return result;
}

// a socket layer that keeps the data written through it
class stublayer : public socketlayer {
	public:
				stublayer(size_t sizemax);
		void		setFileDescriptor(filedescriptor *fd);
		filedescriptor	*getFileDescriptor();
		bool		connect();
		bool		accept();
		ssize_t		read(void *buf, size_t size);
		ssize_t		write(const void *buf, size_t size);
		size_t		getPendingSize();
		bool		close();
		size_t		getSizeMax();
		// data that write() was given
		const char	*getData();
		// count of write() calls and size of the largest one
		uint32_t	getWrites();
		size_t		getLargestWrite();
	private:
		filedescriptor	*fd;
		size_t		sizemax;
		stringbuffer	data;
		uint32_t	writes;
		size_t		largestwrite;
};

stublayer::stublayer(size_t sizemax) : socketlayer() {
	fd=NULL;
	this->sizemax=sizemax;
	writes=0;
	largestwrite=0;
}

void stublayer::setFileDescriptor(filedescriptor *fd) {
	this->fd=fd;
}

filedescriptor *stublayer::getFileDescriptor() {
	return fd;
}

bool stublayer::connect() {
	return true;
}

bool stublayer::accept() {
	return true;
}

ssize_t stublayer::read(void *buf, size_t size) {
	return 0;
}

ssize_t stublayer::write(const void *buf, size_t size) {
	data.append((const char *)buf,size);
	writes++;
	if (size>largestwrite) {
		largestwrite=size;
	}
	return size;
}

size_t stublayer::getPendingSize() {
	return 0;
}

bool stublayer::close() {
	return true;
}

size_t stublayer::getSizeMax() {
	return sizemax;
}

const char *stublayer::getData() {
	return data.getString();
}

uint32_t stublayer::getWrites() {
	return writes;
}

size_t stublayer::getLargestWrite() {
	return largestwrite;
}

static bool pipeHolds(filedescriptor *readfd, filedescriptor *writefd,
						const char *expected);
static bool pipeHolds(filedescriptor *readfd, filedescriptor *writefd,
				const char *expected, size_t length);

// set a UTF-8 character type locale, if the host has one
static bool setUtf8CType() {
	const char	*names[]={
		"C.UTF-8","C.utf8","en_US.UTF-8","en_US.utf8",
	#ifdef _WIN32
		".UTF8",
	#endif
		NULL
	};
	for (const char **name=names; *name; name++) {
		if (locale::setCType(*name)) {
			return true;
		}
	}
	return false;
}

// printf() on file descriptors that shorten or fail their writes
static void printfErrorTests() {

	signalmanager::alarm(60);

	// mode 0 is a clean run, 1 is a short write, -1 is a failed write
	ssize_t		modes[]={0,1,-1};
	const char	*names[]={"clean","short write","failed write"};
	ssize_t		expected[]={20,-1,-1};
	stringbuffer	title;

	// buffered, the formatted data is larger than the buffer
	for (uint16_t i=0; i<sizeof(modes)/sizeof(modes[0]); i++) {
		filedescriptor	readfd;
		failfd		writefd(modes[i]);
		test("buffered, create pipe",
				filedescriptor::createPipe(&readfd,&writefd));
		test("buffered, set buffer size",writefd.setWriteBufferSize(4));
		ssize_t	result=writefd.printf("%s","01234567890123456789");
		title.clear();
		title.append("buffered printf(), ")->append(names[i]);
		test(title.getString(),result==expected[i]);
	}
	if (wcharstring::supportsPrintf()) {
		for (uint16_t i=0; i<sizeof(modes)/sizeof(modes[0]); i++) {
			filedescriptor	readfd;
			failfd		writefd(modes[i]);
			test("buffered, create pipe",
				filedescriptor::createPipe(&readfd,&writefd));
			test("buffered, set buffer size",
					writefd.setWriteBufferSize(4));
			ssize_t	result=writefd.printf(L"%ls",
						L"01234567890123456789");
			title.clear();
			title.append("buffered wide printf(), ");
			title.append(names[i]);
			test(title.getString(),result==expected[i]);
		}

		// buffered, a wide format that cannot be converted
		bool	utf8=setUtf8CType();
		if (!utf8) {
			stdoutput.printf("no UTF-8 locale available, "
					"running in the C locale\n");
		}
		filedescriptor	readfd;
		failfd		writefd(0);
		test("buffered bad format, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("buffered bad format, set buffer size",
					writefd.setWriteBufferSize(4));
		#ifdef _WIN32
		ssize_t	result=writefd.printf(L"abc%hs","\xff");
		#else
		ssize_t	result=writefd.printf(L"abc%s","\xff");
		#endif
		if (result==-1) {
			test("buffered wide printf(), bad format",
				pipeHolds(&readfd,&writefd,""));
		} else if (utf8) {
			test("buffered wide printf(), bad format",false);
		} else {
			stdoutput.printf("buffered wide printf(), bad "
				"format converted without error, skipped\n");
		}
		locale::setCType("C");
	}

	// unbuffered, through the lowLevelWrite() override, for #10920
	// filedescriptor - unbuffered printf() bypasses write(), the socket
	// layer and lowLevelWrite()
	ssize_t		umodes[]={0,1,-1,1};
	bool		ushort[]={false,false,false,true};
	const char	*unames[]={"clean","short write","failed write",
					"short write allowed"};
	ssize_t		uexpected[]={20,-1,-1,-1};
	const char	*uheld[]={"01234567890123456789",
					"0123456789012345678",
					"",
					"0123456789"};
	for (uint16_t w=0; w<2; w++) {
		if (w && !wcharstring::supportsPrintf()) {
			break;
		}
		for (uint16_t i=0; i<sizeof(umodes)/sizeof(umodes[0]); i++) {
			filedescriptor	readfd;
			failfd		writefd(umodes[i]);
			test("unbuffered, create pipe",
				filedescriptor::createPipe(&readfd,&writefd));
			writefd.setAllowShortWrites(ushort[i]);
			ssize_t	result=(w)?
				writefd.printf(L"%ls",L"01234567890123456789"):
				writefd.printf("%s","01234567890123456789");
			title.clear();
			title.append((w)?"unbuffered wide printf(), ":
						"unbuffered printf(), ");
			title.append(unames[i]);
			test(title.getString(),result==uexpected[i]);
			size_t	length=title.getStringLength();
			title.append(", override count");
			test(title.getString(),writefd.getWritten()==
					charstring::getLength(uheld[i]));
			title.truncate(length);
			title.append(", pipe holds it");
			test(title.getString(),
				pipeHolds(&readfd,&writefd,uheld[i]));
		}
	}

	// unbuffered, the override fails and sets errno
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("unbuffered errno, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setFailErrorNumber(EIO);
		error::clearError();
		ssize_t	result=writefd.printf("%s","abc");
		int32_t	errnum=error::getErrorNumber();
		test("unbuffered printf(), EIO",result==-1);
		test("unbuffered printf(), EIO, errno EIO",errnum==EIO);
		test("unbuffered printf(), EIO, pipe empty",
				pipeHolds(&readfd,&writefd,""));
	}

	// unbuffered, the read side of the pipe is closed
	#ifndef _WIN32
	signalset	sigpipe;
	sigpipe.addSignal(SIGPIPE);
	signalmanager::ignoreSignals(&sigpipe);
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("unbuffered, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("unbuffered, close read side",readfd.close());
		ssize_t	result=writefd.printf("%s","01234567890123456789");
		test("unbuffered printf(), closed pipe",result==-1);
	}
	#endif

	// unbuffered, a wide format that cannot be converted
	if (wcharstring::supportsPrintf()) {
		bool	utf8=setUtf8CType();
		if (!utf8) {
			stdoutput.printf("no UTF-8 locale available, "
					"running in the C locale\n");
		}
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("unbuffered bad format, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		#ifdef _WIN32
		ssize_t	result=writefd.printf(L"abc%hs","\xff");
		#else
		ssize_t	result=writefd.printf(L"abc%s","\xff");
		#endif
		if (result==-1) {
			test("unbuffered wide printf(), bad format",true);
		} else if (utf8) {
			test("unbuffered wide printf(), bad format",false);
		} else {
			stdoutput.printf("unbuffered wide printf(), bad "
				"format converted without error, skipped\n");
		}
		locale::setCType("C");
	}

	// wide, a null in the data (a non-const variable, because a
	// constant 0 selects the va_list * overload)
	if (wcharstring::supportsPrintf()) {
		wint_t	nul=0;
		{
			filedescriptor	readfd;
			failfd		writefd(0);
			test("unbuffered null, create pipe",
				filedescriptor::createPipe(&readfd,&writefd));
			test("unbuffered wide printf(), null",
				writefd.printf(L"a%lcb",nul)==3);
			test("unbuffered wide printf(), null, pipe holds it",
				pipeHolds(&readfd,&writefd,"a\0b",3));
		}
		{
			filedescriptor	readfd;
			failfd		writefd(0);
			test("buffered null, create pipe",
				filedescriptor::createPipe(&readfd,&writefd));
			test("buffered null, set buffer size",
					writefd.setWriteBufferSize(4));
			test("buffered wide printf(), null",
				writefd.printf(L"a%lcb",nul)==3);
			test("buffered wide printf(), null, flush",
				writefd.flushWriteBuffer(-1,-1));
			test("buffered wide printf(), null, pipe holds it",
				pipeHolds(&readfd,&writefd,"a\0b",3));
		}
		if (setUtf8CType()) {
			filedescriptor	readfd;
			failfd		writefd(0);
			test("multibyte null, create pipe",
				filedescriptor::createPipe(&readfd,&writefd));
			test("multibyte wide printf(), null",
				writefd.printf(L"h%lsllo|%lc|x",
						L"\x00e9",nul)==10);
			test("multibyte wide printf(), null, pipe holds it",
				pipeHolds(&readfd,&writefd,
						"h\xc3\xa9llo|\0|x",10));
		} else {
			stdoutput.printf("no UTF-8 locale available, "
				"multibyte wide printf(), null, skipped\n");
		}
		locale::setCType("C");
	}

	// ucs2 printf is unsupported, fails and writes nothing
	if (!ucs2charstring::supportsPrintf()) {
		ucs2literal(format,"abc");
		filedescriptor	readfd;
		failfd		writefd(0);
		test("unsupported printfUcs2(), create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("unsupported printfUcs2()",
				writefd.printfUcs2(format)==-1);
		test("unsupported printfUcs2(), nothing written",
				writefd.getWritten()==0);
		test("unsupported printfUcs2(), pipe empty",
				pipeHolds(&readfd,&writefd,""));
	} else {
		// ucs2, a null in the data
		wint_t	nul=0;
		ucs2literal(nullformat,"a%lcb");
		filedescriptor	readfd;
		failfd		writefd(0);
		test("ucs2 null, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("unbuffered printfUcs2(), null",
			writefd.printfUcs2(nullformat,nul)==3);
		test("unbuffered printfUcs2(), null, pipe holds it",
			pipeHolds(&readfd,&writefd,"a\0b",3));
	}
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// unbuffered printf() through a socket layer, for #10920 filedescriptor -
// unbuffered printf() bypasses write(), the socket layer and lowLevelWrite()
static void printfSocketLayerTests() {

	signalmanager::alarm(60);

	// largest write the layer takes, data to print, writes expected
	size_t		sizemaxes[]={1024,4};
	const char	*data[]={"abc","0123456789"};
	uint32_t	writes[]={1,3};
	stringbuffer	title;

	for (uint16_t w=0; w<2; w++) {
		if (w && !wcharstring::supportsPrintf()) {
			break;
		}
		for (uint16_t i=0; i<sizeof(sizemaxes)/sizeof(sizemaxes[0]);
									i++) {
			stublayer	layer(sizemaxes[i]);
			filedescriptor	readfd;
			filedescriptor	writefd;
			test("socket layer, create pipe",
				filedescriptor::createPipe(&readfd,&writefd));
			writefd.setSocketLayer(&layer);
			size_t	length=charstring::getLength(data[i]);
			ssize_t	result;
			if (w) {
				wchar_t	*wdata=wcharstring::duplicate(data[i]);
				result=writefd.printf(L"%ls",wdata);
				delete[] wdata;
			} else {
				result=writefd.printf("%s",data[i]);
			}
			writefd.setSocketLayer(NULL);
			title.clear();
			title.append((w)?"wide printf() ":"printf() ");
			title.append(data[i]);
			size_t	titlelength=title.getStringLength();
			title.append(", count");
			test(title.getString(),result==(ssize_t)length);
			title.truncate(titlelength);
			title.append(", layer has it");
			test(title.getString(),
				!charstring::compare(layer.getData(),data[i]));
			title.truncate(titlelength);
			title.append(", layer writes");
			test(title.getString(),layer.getWrites()==writes[i] &&
				layer.getLargestWrite()<=sizemaxes[i]);
			title.truncate(titlelength);
			title.append(", pipe empty");
			test(title.getString(),pipeHolds(&readfd,&writefd,""));
		}
	}

	// wide, a null in the data, in one write
	if (wcharstring::supportsPrintf()) {
		stublayer	layer(1024);
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("socket layer null, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setSocketLayer(&layer);
		wint_t	nul=0;
		ssize_t	result=writefd.printf(L"a%lcb",nul);
		writefd.setSocketLayer(NULL);
		test("wide printf() null, count",result==3);
		test("wide printf() null, layer has it",
			!bytestring::compare(layer.getData(),"a\0b",3));
		test("wide printf() null, layer writes",layer.getWrites()==1);
		test("wide printf() null, pipe empty",
					pipeHolds(&readfd,&writefd,""));
	}
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

#ifndef _WIN32
// fill a pipe until a write would block, and leave it in blocking mode
static bool fillPipe(filedescriptor *writefd) {
	if (!writefd->setNonBlockingMode(true)) {
		return false;
	}
	char	filler[4096];
	bytestring::set(filler,'x',sizeof(filler));
	while (writefd->write(filler,sizeof(filler))==
					(ssize_t)sizeof(filler)) {}
	while (writefd->write(filler,1)==1) {}
	return writefd->setNonBlockingMode(false);
}

// An alarm can come before write() starts, so the handler counts the alarms
// and re-arms.  After five alarms, the test takes the write as retried.
static volatile sig_atomic_t	alarms;
static volatile sig_atomic_t	retried;
static void alarmHandler(int32_t signum) {
	alarms=alarms+1;
	if (alarms>=6) {

		// A retried write never returns on the full pipe.  test() is
		// not async-signal-safe, so write a fixed message and exit.
		const char	message[]=
			"\tprintf(), interrupted, write not retried: failed\n";
		if (::write(2,message,sizeof(message)-1)) {}
		::_exit(1);
	}
	if (alarms>=5) {
		retried=1;
	}
	signalmanager::alarm(1);
}
#endif

// errno after an unbuffered printf(), for #11166 filedescriptor - an
// unbuffered printf() through write() loses errno EAGAIN or EINTR
static void printfErrnoTests() {

	signalmanager::alarm(60);

	// success leaves errno as it was
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("success, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		error::setErrorNumber(ENOENT);
		ssize_t	result=writefd.printf("%s","abc");
		int32_t	errnum=error::getErrorNumber();
		test("printf(), success",result==3);
		test("printf(), success, errno unchanged",errnum==ENOENT);
		error::clearError();
		test("success, pipe holds abc",
				pipeHolds(&readfd,&writefd,"abc"));
	}

	// wide success leaves errno as it was, for #11168 filedescriptor -
	// a wide printf() that succeeds sets errno to 0
	if (wcharstring::supportsPrintf()) {
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("wide success, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		error::setErrorNumber(ENOENT);
		ssize_t	result=writefd.printf(L"%ls",L"def");
		int32_t	errnum=error::getErrorNumber();
		test("wide printf(), success",result==3);
		test("wide printf(), success, errno unchanged",
							errnum==ENOENT);
		error::clearError();
		test("wide success, pipe holds def",
				pipeHolds(&readfd,&writefd,"def"));
	}
	if (wcharstring::supportsPrintf()) {
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("wide empty, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		error::setErrorNumber(ENOENT);
		ssize_t	result=writefd.printf(L"%ls",L"");
		int32_t	errnum=error::getErrorNumber();
		test("wide printf(), empty",result==0);
		test("wide printf(), empty, errno unchanged",errnum==ENOENT);
		error::clearError();
		test("wide empty, pipe holds nothing",
				pipeHolds(&readfd,&writefd,""));
	}

	#ifndef _WIN32

	// full non-blocking pipe
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("full, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("full, fill pipe",fillPipe(&writefd));
		test("full, set non-blocking",
				writefd.setNonBlockingMode(true));
		error::clearError();
		ssize_t	result=writefd.printf("%s","abc");
		int32_t	errnum=error::getErrorNumber();
		test("printf(), full pipe",result==-1);
		test("printf(), full pipe, errno EAGAIN",errnum==EAGAIN);
		if (wcharstring::supportsPrintf()) {
			error::clearError();
			result=writefd.printf(L"%ls",L"abc");
			errnum=error::getErrorNumber();
			test("wide printf(), full pipe",result==-1);
			test("wide printf(), full pipe, errno EAGAIN",
							errnum==EAGAIN);
		}
		error::clearError();
	}

	// interrupted write on a full blocking pipe
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("interrupted, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("interrupted, fill pipe",fillPipe(&writefd));

		// catch SIGALRM without SA_RESTART
		signalhandler	alarmhandler;
		signalhandler	oldhandler;
		alarmhandler.setHandler(alarmHandler);
		test("interrupted, handle SIGALRM",
			alarmhandler.handleSignal(SIGALRM,&oldhandler));

		alarms=0;
		retried=0;
		signalmanager::alarm(1);
		error::clearError();
		ssize_t	result=writefd.printf("%s","abc");
		int32_t	errnum=error::getErrorNumber();
		signalmanager::alarm(0);

		// restore the old handler and the time limit
		test("interrupted, restore SIGALRM",
				oldhandler.handleSignal(SIGALRM));
		signalmanager::alarm(60);

		test("printf(), interrupted, write not retried",!retried);
		test("printf(), interrupted",result==-1);
		test("printf(), interrupted, errno EINTR",errnum==EINTR);
		error::clearError();
	}

	#endif
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// close the write side of a pipe and check that the read side holds
// exactly the expected data
static bool pipeHolds(filedescriptor *readfd, filedescriptor *writefd,
						const char *expected) {
	return pipeHolds(readfd,writefd,expected,
				charstring::getLength(expected));
}

// close the write side of a pipe and check that the read side holds
// exactly the "length" bytes of expected data
static bool pipeHolds(filedescriptor *readfd, filedescriptor *writefd,
				const char *expected, size_t length) {
	if (!writefd->close()) {
		return false;
	}
	char	buf[64];
	if (readfd->read(buf,sizeof(buf))!=(ssize_t)length ||
			bytestring::compare(buf,expected,length)) {
		return false;
	}
	return !readfd->read(buf,sizeof(buf));
}

// flushWriteBuffer() on file descriptors that shorten or fail their writes
static void flushErrorTests() {

	signalmanager::alarm(60);

	// stream, nothing pending
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("stream, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream, set buffer size",writefd.setWriteBufferSize(4));
		test("stream, clean flush",writefd.flushWriteBuffer(-1,-1));
		test("stream, write",writefd.write("abc")==3);
		test("stream, flush",writefd.flushWriteBuffer(-1,-1));
		test("stream, flush again",writefd.flushWriteBuffer(-1,-1));
		test("stream, pipe holds abc",
				pipeHolds(&readfd,&writefd,"abc"));
	}

	// stream, failed write
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("stream failed, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream failed, set buffer size",
					writefd.setWriteBufferSize(4));
		test("stream failed, write",writefd.write("abc")==3);
		writefd.setMode(-1);
		test("stream failed, flush",!writefd.flushWriteBuffer(-1,-1));
		writefd.setMode(0);
		test("stream failed, retry flush",
					writefd.flushWriteBuffer(-1,-1));
		test("stream failed, pipe holds abc",
				pipeHolds(&readfd,&writefd,"abc"));
	}

	// stream, short write
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("stream short, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream short, set buffer size",
					writefd.setWriteBufferSize(4));
		test("stream short, write",writefd.write("abc")==3);
		writefd.setMode(1);
		test("stream short, flush",!writefd.flushWriteBuffer(-1,-1));
		writefd.setMode(0);
		test("stream short, retry flush",
					writefd.flushWriteBuffer(-1,-1));
		test("stream short, pipe holds abc",
				pipeHolds(&readfd,&writefd,"abc"));
	}

	// stream, the auto-flush passes and the final flush fails
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("stream pass first, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream pass first, set buffer size",
					writefd.setWriteBufferSize(4));
		writefd.setPassFirst(1);
		test("stream pass first, write",writefd.write("abcdef")==6);
		test("stream pass first, flush",
					!writefd.flushWriteBuffer(-1,-1));
		writefd.setMode(0);
		test("stream pass first, retry flush",
					writefd.flushWriteBuffer(-1,-1));
		test("stream pass first, pipe holds abcdef",
				pipeHolds(&readfd,&writefd,"abcdef"));
	}

	// storage, failed and short writes
	ssize_t		modes[]={-1,1};
	const char	*names[]={"storage failed","storage short"};
	stringbuffer	title;
	for (uint16_t i=0; i<sizeof(modes)/sizeof(modes[0]); i++) {
		file::remove(flushfile);
		file	fl;
		test("storage, create file",fl.create(flushfile,
				permissions::parsePermString("rw-rw----")));
		failfd	writefd(0);
		writefd.setFileDescriptor(fl.duplicate());
		test("storage, set is stream",writefd.setIsStream(false));
		test("storage, set buffer size",writefd.setWriteBufferSize(4));
		test("storage, write",writefd.write("abc")==3);
		writefd.setMode(modes[i]);
		title.clear();
		title.append(names[i])->append(", flush");
		test(title.getString(),!writefd.flushWriteBuffer(-1,-1));
		writefd.setMode(0);
		title.clear();
		title.append(names[i])->append(", retry flush");
		test(title.getString(),writefd.flushWriteBuffer(-1,-1));
		writefd.close();
		fl.close();
		char	*contents=file::getContents(flushfile);
		title.clear();
		title.append(names[i])->append(", file holds abc");
		test(title.getString(),!charstring::compare(contents,"abc"));
		delete[] contents;
	}
	file::remove(flushfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// write() through a full buffer on file descriptors
// that shorten, fail, or time out their writes
static void writeErrorTests() {

	signalmanager::alarm(60);

	// stream, clean writes that fill the buffer more than once
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("clean, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("clean, set buffer size",writefd.setWriteBufferSize(4));
		test("clean, write abcdefghij",
				writefd.write("abcdefghij")==10);
		test("clean, flush",writefd.flushWriteBuffer(-1,-1));
		test("clean, pipe holds abcdefghij",
				pipeHolds(&readfd,&writefd,"abcdefghij"));
	}
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("clean buffered, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("clean buffered, set buffer size",
					writefd.setWriteBufferSize(4));
		test("clean buffered, write ab",writefd.write("ab")==2);
		test("clean buffered, write cde",writefd.write("cde")==3);
		test("clean buffered, write fghijklmn",
				writefd.write("fghijklmn")==9);
		test("clean buffered, flush",writefd.flushWriteBuffer(-1,-1));
		test("clean buffered, pipe holds abcdefghijklmn",
				pipeHolds(&readfd,&writefd,"abcdefghijklmn"));
	}

	// stream, failed flush with data in the buffer
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("failed buffered, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("failed buffered, set buffer size",
					writefd.setWriteBufferSize(4));
		test("failed buffered, write ab",writefd.write("ab")==2);
		writefd.setMode(-1);
		test("failed buffered, write cde",writefd.write("cde")==-1);
		test("failed buffered, write cdefgh",
					writefd.write("cdefgh")==-1);
		writefd.setMode(0);
		test("failed buffered, retry cdefgh",
					writefd.write("cdefgh")==6);
		test("failed buffered, flush",
					writefd.flushWriteBuffer(-1,-1));
		test("failed buffered, pipe holds abcdefgh",
				pipeHolds(&readfd,&writefd,"abcdefgh"));
	}

	// stream, short flush with data in the buffer
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("short buffered, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("short buffered, set buffer size",
					writefd.setWriteBufferSize(4));
		test("short buffered, write ab",writefd.write("ab")==2);
		writefd.setMode(1);
		test("short buffered, write cde",writefd.write("cde")==1);
		writefd.setMode(0);
		test("short buffered, retry de",writefd.write("de")==2);
		test("short buffered, flush",writefd.flushWriteBuffer(-1,-1));
		test("short buffered, pipe holds abcde",
				pipeHolds(&readfd,&writefd,"abcde"));
	}

	// stream, short flush with an empty buffer
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("short, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("short, set buffer size",writefd.setWriteBufferSize(4));
		writefd.setMode(1);
		test("short, write abcdefgh",writefd.write("abcdefgh")==3);
		writefd.setMode(0);
		test("short, retry defgh",writefd.write("defgh")==5);
		test("short, flush",writefd.flushWriteBuffer(-1,-1));
		test("short, pipe holds abcdefgh",
				pipeHolds(&readfd,&writefd,"abcdefgh"));
	}

	// stream, short flush in one pass with an empty buffer
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("one pass, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("one pass, set buffer size",writefd.setWriteBufferSize(4));
		writefd.setAllowShortWrites(true);
		writefd.setMode(1);
		test("one pass, write abcdefgh",writefd.write("abcdefgh")==2);
		writefd.setMode(0);
		test("one pass, retry cdefgh",writefd.write("cdefgh")==6);
		test("one pass, flush",writefd.flushWriteBuffer(-1,-1));
		test("one pass, pipe holds abcdefgh",
				pipeHolds(&readfd,&writefd,"abcdefgh"));
	}

	// stream, short flush in one pass that leaves earlier data
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("one pass buffered, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("one pass buffered, set buffer size",
					writefd.setWriteBufferSize(4));
		writefd.setAllowShortWrites(true);
		test("one pass buffered, write abc",writefd.write("abc")==3);
		writefd.setMode(1);
		test("one pass buffered, write de",writefd.write("de")==0);
		writefd.setMode(0);
		test("one pass buffered, retry de",writefd.write("de")==2);
		test("one pass buffered, flush",
					writefd.flushWriteBuffer(-1,-1));
		test("one pass buffered, pipe holds abcde",
				pipeHolds(&readfd,&writefd,"abcde"));
	}

	// stream, the first flush passes and the second fails
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("second failed, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("second failed, set buffer size",
					writefd.setWriteBufferSize(4));
		writefd.setPassFirst(1);
		test("second failed, write abcdefghij",
				writefd.write("abcdefghij")==4);
		writefd.setMode(0);
		test("second failed, retry efghij",
				writefd.write("efghij")==6);
		test("second failed, flush",writefd.flushWriteBuffer(-1,-1));
		test("second failed, pipe holds abcdefghij",
				pipeHolds(&readfd,&writefd,"abcdefghij"));
	}

	// stream, timed-out flush with data in the buffer
	#ifndef WIN32
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("timeout, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));

		// fill the pipe
		char	filler[4096];
		bytestring::set(filler,'x',sizeof(filler));
		test("timeout, set non-blocking",
				writefd.setNonBlockingMode(true));
		size_t	filled=0;
		ssize_t	result;
		do {
			result=writefd.write(filler,sizeof(filler));
			if (result>0) {
				filled+=result;
			}
		} while (result==(ssize_t)sizeof(filler));
		test("timeout, set blocking",
				writefd.setNonBlockingMode(false));

		// write through the buffer
		test("timeout, set buffer size",writefd.setWriteBufferSize(4));
		test("timeout, write ab",writefd.write("ab")==2);
		test("timeout, write cdefgh",
			writefd.write("cdefgh",0,1000)==RESULT_TIMEOUT);

		// drain the pipe
		size_t	drained=0;
		while (drained<filled) {
			result=readfd.read(filler,
				(filled-drained<sizeof(filler))?
					filled-drained:sizeof(filler));
			if (result<=0) {
				break;
			}
			drained+=result;
		}
		test("timeout, drain pipe",drained==filled);

		// retry
		test("timeout, retry cdefgh",writefd.write("cdefgh")==6);
		test("timeout, flush",writefd.flushWriteBuffer(-1,-1));
		test("timeout, pipe holds abcdefgh",
				pipeHolds(&readfd,&writefd,"abcdefgh"));
	}
	#endif

	// storage, failed and short realign after some bytes were copied in
	ssize_t		modes[]={-1,1};
	const char	*names[]={"storage failed","storage short"};
	stringbuffer	title;
	for (uint16_t i=0; i<sizeof(modes)/sizeof(modes[0]); i++) {
		file::remove(flushfile);
		file	fl;
		test("storage, create file",fl.create(flushfile,
				permissions::parsePermString("rw-rw----")));
		failfd	writefd(0);
		writefd.setFileDescriptor(fl.duplicate());
		test("storage, set is stream",writefd.setIsStream(false));
		test("storage, set buffer size",writefd.setWriteBufferSize(4));
		writefd.setMode(modes[i]);
		title.clear();
		title.append(names[i])->append(", write abcdefgh");
		test(title.getString(),writefd.write("abcdefgh")==4);
		writefd.setMode(0);
		title.clear();
		title.append(names[i])->append(", retry efgh");
		test(title.getString(),writefd.write("efgh")==4);
		test("storage, flush",writefd.flushWriteBuffer(-1,-1));
		writefd.close();
		fl.close();
		char	*contents=file::getContents(flushfile);
		title.clear();
		title.append(names[i])->append(", file holds abcdefgh");
		test(title.getString(),
				!charstring::compare(contents,"abcdefgh"));
		delete[] contents;
	}

	// storage, failed realign with nothing copied in
	{
		file::remove(flushfile);
		file	fl;
		test("storage, create file",fl.create(flushfile,
				permissions::parsePermString("rw-rw----")));
		failfd	writefd(0);
		writefd.setFileDescriptor(fl.duplicate());
		test("storage, set is stream",writefd.setIsStream(false));
		test("storage, set buffer size",writefd.setWriteBufferSize(4));
		test("storage full, write abcd",writefd.write("abcd")==4);
		writefd.setMode(-1);
		test("storage full, write efgh",writefd.write("efgh")==-1);
		writefd.setMode(0);
		test("storage full, retry efgh",writefd.write("efgh")==4);
		test("storage full, flush",writefd.flushWriteBuffer(-1,-1));
		writefd.close();
		fl.close();
		char	*contents=file::getContents(flushfile);
		test("storage full, file holds abcdefgh",
				!charstring::compare(contents,"abcdefgh"));
		delete[] contents;
	}
	file::remove(flushfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// unbuffered write() in several passes, where a later pass
// fails, is interrupted, or times out
static void partialWriteTests() {

	signalmanager::alarm(60);

	// write error on the second pass
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("second error, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setFailErrorNumber(EIO);
		writefd.setShortFirst(1);
		test("second error, write abcdefgh",
				writefd.write("abcdefgh")==4);
		test("second error, written",writefd.getWritten()==4);
		test("second error, pipe holds abcd",
				pipeHolds(&readfd,&writefd,"abcd"));
	}

	// write error on the first pass
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("first error, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setFailErrorNumber(EIO);
		test("first error, write abcdefgh",
				writefd.write("abcdefgh")==RESULT_ERROR);
		test("first error, written",!writefd.getWritten());
		test("first error, pipe holds nothing",
				pipeHolds(&readfd,&writefd,""));
	}

	// interrupt on the second pass, not retried
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("second eintr, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setFailErrorNumber(EINTR);
		writefd.setShortFirst(1);
		test("second eintr, write abcdefgh",
				writefd.write("abcdefgh")==4);
		test("second eintr, written",writefd.getWritten()==4);
		test("second eintr, pipe holds abcd",
				pipeHolds(&readfd,&writefd,"abcd"));
	}

	// interrupt on the first pass, not retried
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("first eintr, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setFailErrorNumber(EINTR);
		test("first eintr, write abcdefgh",
				writefd.write("abcdefgh")==RESULT_ERROR);
		test("first eintr, written",!writefd.getWritten());
		test("first eintr, pipe holds nothing",
				pipeHolds(&readfd,&writefd,""));
	}

	// shut-down flag and an interrupt on the second pass
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("second shutdown, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setFailErrorNumber(EINTR);
		writefd.setShortFirst(1);
		process::setShutDownFlag(true);
		ssize_t	result=writefd.write("abcdefgh");
		process::setShutDownFlag(false);
		test("second shutdown, write abcdefgh",result==4);
		test("second shutdown, written",writefd.getWritten()==4);
		test("second shutdown, pipe holds abcd",
				pipeHolds(&readfd,&writefd,"abcd"));
	}

	// shut-down flag and an interrupt on the first pass
	{
		filedescriptor	readfd;
		failfd		writefd(-1);
		test("first shutdown, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		writefd.setFailErrorNumber(EINTR);
		process::setShutDownFlag(true);
		ssize_t	result=writefd.write("abcdefgh");
		process::setShutDownFlag(false);
		test("first shutdown, write abcdefgh",result==RESULT_ERROR);
		test("first shutdown, written",!writefd.getWritten());
		test("first shutdown, pipe holds nothing",
				pipeHolds(&readfd,&writefd,""));
	}

	// timeout after a partial pass
	#ifndef WIN32
	{
		filedescriptor	readfd;
		failfd		writefd(0);
		test("partial timeout, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));

		// fill the pipe
		char	filler[4096];
		bytestring::set(filler,'x',sizeof(filler));
		test("partial timeout, set non-blocking",
				writefd.setNonBlockingMode(true));
		ssize_t	result;
		do {
			result=writefd.write(filler,sizeof(filler));
		} while (result==(ssize_t)sizeof(filler));

		// make room for part of the next write
		test("partial timeout, read 4096",
			readfd.read(filler,sizeof(filler))==
						(ssize_t)sizeof(filler));

		// write more than there is room for
		char	buf[8192];
		bytestring::set(buf,'y',sizeof(buf));
		size_t	before=writefd.getWritten();
		test("partial timeout, write 8192",
			writefd.write(buf,sizeof(buf),0,1000)==4096);
		test("partial timeout, written",
				writefd.getWritten()-before==4096);

		// write to the full pipe
		test("full timeout, write 8192",
			writefd.write(buf,sizeof(buf),0,1000)==RESULT_TIMEOUT);
		test("full timeout, written",
				writefd.getWritten()-before==4096);
	}
	#endif
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// unbuffered read() in several passes, where a later pass
// fails, is interrupted, times out, or meets the end of the data
static void partialReadTests() {

	signalmanager::alarm(60);

	char		buf[8];
	ssize_t		result;
	int32_t		errnum;
	stringbuffer	title;

	// read error on the second pass, with errno set and with errno 0,
	// either way errno is EIO afterwards
	int32_t		errnums[]={EIO,0};
	const char	*names[]={"second error","second silent error"};
	for (uint16_t i=0; i<sizeof(errnums)/sizeof(errnums[0]); i++) {
		failreader<filedescriptor>	readfd(-1);
		filedescriptor			writefd;
		title.clear();
		title.append(names[i])->append(", create pipe");
		test(title.getString(),
			filedescriptor::createPipe(&readfd,&writefd));
		title.clear();
		title.append(names[i])->append(", write abcdefgh");
		test(title.getString(),writefd.write("abcdefgh")==8);
		readfd.setFailErrorNumber(errnums[i]);
		readfd.setShortFirst(1);
		result=readfd.read(buf,sizeof(buf));
		errnum=error::getErrorNumber();
		title.clear();
		title.append(names[i])->append(", read 8");
		test(title.getString(),result==4);
		title.clear();
		title.append(names[i])->append(", errno");
		test(title.getString(),errnum==EIO);
		title.clear();
		title.append(names[i])->append(", read");
		test(title.getString(),readfd.getRead()==4);
		title.clear();
		title.append(names[i])->append(", buffer holds abcd");
		test(title.getString(),!bytestring::compare(buf,"abcd",4));
		readfd.setMode(0);
		title.clear();
		title.append(names[i])->append(", pipe holds efgh");
		test(title.getString(),pipeHolds(&readfd,&writefd,"efgh"));
	}

	// read error on the first pass
	{
		failreader<filedescriptor>	readfd(-1);
		filedescriptor			writefd;
		test("first error, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("first error, write abcdefgh",
				writefd.write("abcdefgh")==8);
		readfd.setFailErrorNumber(EIO);
		result=readfd.read(buf,sizeof(buf));
		errnum=error::getErrorNumber();
		test("first error, read 8",result==RESULT_ERROR);
		test("first error, errno",errnum==EIO);
		test("first error, read",!readfd.getRead());
		readfd.setMode(0);
		test("first error, pipe holds abcdefgh",
				pipeHolds(&readfd,&writefd,"abcdefgh"));
	}

	// interrupt on the second pass, not retried
	{
		failreader<filedescriptor>	readfd(-1);
		filedescriptor			writefd;
		test("second eintr, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("second eintr, write abcdefgh",
				writefd.write("abcdefgh")==8);
		readfd.setFailErrorNumber(EINTR);
		readfd.setShortFirst(1);
		result=readfd.read(buf,sizeof(buf));
		errnum=error::getErrorNumber();
		test("second eintr, read 8",result==4);
		test("second eintr, errno",errnum==EINTR);
		test("second eintr, read",readfd.getRead()==4);
		test("second eintr, buffer holds abcd",
				!bytestring::compare(buf,"abcd",4));
		readfd.setMode(0);
		test("second eintr, pipe holds efgh",
				pipeHolds(&readfd,&writefd,"efgh"));
	}

	// interrupt on the first pass, not retried
	{
		failreader<filedescriptor>	readfd(-1);
		filedescriptor			writefd;
		test("first eintr, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("first eintr, write abcdefgh",
				writefd.write("abcdefgh")==8);
		readfd.setFailErrorNumber(EINTR);
		result=readfd.read(buf,sizeof(buf));
		test("first eintr, read 8",result==RESULT_ERROR);
		test("first eintr, read",!readfd.getRead());
		readfd.setMode(0);
		test("first eintr, pipe holds abcdefgh",
				pipeHolds(&readfd,&writefd,"abcdefgh"));
	}

	// shut-down flag and an interrupt on the second pass
	{
		failreader<filedescriptor>	readfd(-1);
		filedescriptor			writefd;
		test("second shutdown, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("second shutdown, write abcdefgh",
				writefd.write("abcdefgh")==8);
		readfd.setFailErrorNumber(EINTR);
		readfd.setShortFirst(1);
		process::setShutDownFlag(true);
		result=readfd.read(buf,sizeof(buf));
		errnum=error::getErrorNumber();
		process::setShutDownFlag(false);
		test("second shutdown, read 8",result==4);
		test("second shutdown, errno",errnum==EINTR);
		test("second shutdown, read",readfd.getRead()==4);
		test("second shutdown, buffer holds abcd",
				!bytestring::compare(buf,"abcd",4));
		readfd.setMode(0);
		test("second shutdown, pipe holds efgh",
				pipeHolds(&readfd,&writefd,"efgh"));
	}

	// shut-down flag and an interrupt on the first pass
	{
		failreader<filedescriptor>	readfd(-1);
		filedescriptor			writefd;
		test("first shutdown, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("first shutdown, write abcdefgh",
				writefd.write("abcdefgh")==8);
		readfd.setFailErrorNumber(EINTR);
		process::setShutDownFlag(true);
		result=readfd.read(buf,sizeof(buf));
		process::setShutDownFlag(false);
		test("first shutdown, read 8",result==RESULT_ERROR);
		test("first shutdown, read",!readfd.getRead());
		readfd.setMode(0);
		test("first shutdown, pipe holds abcdefgh",
				pipeHolds(&readfd,&writefd,"abcdefgh"));
	}

	// timeout after a partial pass
	#ifndef WIN32
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("partial timeout, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("partial timeout, write abcd",writefd.write("abcd")==4);
		result=readfd.read(buf,sizeof(buf),0,1000);
		errnum=error::getErrorNumber();
		test("partial timeout, read 8",result==4);
		#ifdef ETIMEDOUT
		test("partial timeout, errno",errnum==ETIMEDOUT);
		#else
		test("partial timeout, errno",errnum==EIO);
		#endif
		test("partial timeout, buffer holds abcd",
				!bytestring::compare(buf,"abcd",4));

		// read from the empty pipe
		test("empty timeout, read 8",
			readfd.read(buf,sizeof(buf),0,1000)==RESULT_TIMEOUT);
	}
	#endif

	// end of data after a partial pass
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("partial eof, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("partial eof, write abcd",writefd.write("abcd")==4);
		test("partial eof, close",writefd.close());
		result=readfd.read(buf,sizeof(buf));
		errnum=error::getErrorNumber();
		test("partial eof, read 8",result==4);
		test("partial eof, errno",!errnum);
		test("partial eof, buffer holds abcd",
				!bytestring::compare(buf,"abcd",4));
	}

	// library callers that read in several passes,
	// with errno set and with errno 0
	file::remove(readfile);
	{
		file	fl;
		test("contents, create file",fl.create(readfile,
				permissions::parsePermString("rw-rw----")));
		test("contents, write abcdefgh",fl.write("abcdefgh")==8);
		test("contents, close",fl.close());
	}
	const char	*cnames[]={"contents error","contents silent error"};
	const char	*pnames[]={"pipe contents error",
					"pipe contents silent error"};
	const char	*snames[]={"storage error","storage silent error"};
	for (uint16_t i=0; i<sizeof(errnums)/sizeof(errnums[0]); i++) {

		// getContents() of a file of known size, read error mid-file
		{
			failreader<file>	fl(-1);
			title.clear();
			title.append(cnames[i])->append(", open");
			test(title.getString(),fl.open(readfile,O_RDONLY));
			fl.setFailErrorNumber(errnums[i]);
			fl.setShortFirst(1);
			byte_t	*contents=(byte_t *)1;
			size_t	size=1;
			title.clear();
			title.append(cnames[i])->append(", byte_t **");
			test(title.getString(),
				fl.getContents(&contents,&size)==
								RESULT_ERROR &&
				!contents && !size);
			title.clear();
			title.append(cnames[i])->append(", read");
			test(title.getString(),fl.getRead()==4);
		}

		// getContents() of a pipe, read error mid-data
		{
			failreader<file>	readfd(-1);
			filedescriptor		writefd;
			title.clear();
			title.append(pnames[i])->append(", create pipe");
			test(title.getString(),
				filedescriptor::createPipe(&readfd,&writefd));
			title.clear();
			title.append(pnames[i])->append(", write abcdefgh");
			test(title.getString(),writefd.write("abcdefgh")==8);
			readfd.setFailErrorNumber(errnums[i]);
			readfd.setShortFirst(1);
			byte_t	*contents=(byte_t *)1;
			size_t	size=1;
			title.clear();
			title.append(pnames[i])->append(", byte_t **");
			test(title.getString(),
				readfd.getContents(&contents,&size)==
								RESULT_ERROR &&
				!contents && !size);
			// Linux reports a size of 0 for a pipe, BSD reports
			// the bytes waiting in it, so the count of bytes read
			// depends on the platform.
			title.clear();
			title.append(pnames[i])->append(", read");
			test(title.getString(),readfd.getRead()>0);
		}

		// storage buffer, read error after part of the block
		{
			file	fl;
			title.clear();
			title.append(snames[i])->append(", open");
			test(title.getString(),fl.open(readfile,O_RDWR));
			failreader<filedescriptor>	readfd(-1);
			readfd.setFileDescriptor(fl.duplicate());
			title.clear();
			title.append(snames[i])->append(", set is stream");
			test(title.getString(),readfd.setIsStream(false));
			title.clear();
			title.append(snames[i])->append(", set buffer size");
			test(title.getString(),readfd.setWriteBufferSize(8));
			readfd.setFailErrorNumber(errnums[i]);
			readfd.setShortFirst(1);
			result=readfd.read(buf,sizeof(buf));
			title.clear();
			title.append(snames[i])->append(", read 8");
			test(title.getString(),result==RESULT_ERROR);
			title.clear();
			title.append(snames[i])->append(", read");
			test(title.getString(),readfd.getRead()==4);
			readfd.close();
			fl.close();
		}
	}

	// storage buffer, timeout after part of the block
	#ifdef ETIMEDOUT
	{
		file	fl;
		test("storage timeout, open",fl.open(readfile,O_RDWR));
		failreader<filedescriptor>	readfd(-1);
		readfd.setFileDescriptor(fl.duplicate());
		test("storage timeout, set is stream",
					readfd.setIsStream(false));
		test("storage timeout, set buffer size",
					readfd.setWriteBufferSize(8));
		readfd.setFailErrorNumber(ETIMEDOUT);
		readfd.setShortFirst(1);
		result=readfd.read(buf,sizeof(buf));
		test("storage timeout, read 8",result==RESULT_TIMEOUT);
		test("storage timeout, read",readfd.getRead()==4);
		readfd.close();
		fl.close();
	}
	#endif
	file::remove(readfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// stream buffered read() with short reads off, where filling the read buffer
// meets an end of data and the data goes on after it
static void bufferedEofReadTests() {

	signalmanager::alarm(60);

	char	data[64];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}

	// the data after the end of data fills the request, is shorter than
	// the request but longer than the read buffer, or fits the read buffer
	size_t		sizes[]={64,40,8};
	ssize_t		firsts[]={64,40,0};
	ssize_t		seconds[]={0,0,8};
	const char	*names[]={"fill","short","fits"};
	stringbuffer	title;
	for (uint16_t i=0; i<sizeof(sizes)/sizeof(sizes[0]); i++) {

		failreader<filedescriptor>	readfd(0);
		filedescriptor			writefd;
		title.clear();
		title.append(names[i])->append(", create pipe");
		test(title.getString(),
			filedescriptor::createPipe(&readfd,&writefd));
		title.clear();
		title.append(names[i])->append(", write");
		test(title.getString(),
			writefd.write(data,sizes[i])==(ssize_t)sizes[i]);
		title.clear();
		title.append(names[i])->append(", close");
		test(title.getString(),writefd.close());
		title.clear();
		title.append(names[i])->append(", set read buffer size");
		test(title.getString(),readfd.setReadBufferSize(16));
		readfd.setAllowShortReads(false);
		readfd.setEofFirst(1);

		// the first read returns what it copied out before an end of
		// data, the second returns the rest
		char	buf[128];
		bytestring::zero(buf,sizeof(buf));
		ssize_t	result=readfd.read(buf,sizeof(data));
		title.clear();
		title.append(names[i])->append(", first read 64");
		test(title.getString(),result==firsts[i]);
		ssize_t	result2=readfd.read(buf+firsts[i],sizeof(data));
		title.clear();
		title.append(names[i])->append(", second read 64");
		test(title.getString(),result2==seconds[i]);
		title.clear();
		title.append(names[i])->append(", data");
		test(title.getString(),
			!bytestring::compare(buf,data,sizes[i]));
		title.clear();
		title.append(names[i])->append(", read");
		test(title.getString(),readfd.getRead()==sizes[i]);
	}
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// delete of a file with a mapped block and no close(), for #11194
// filedescriptor - the destructor frees the memory map of the write buffer
// with delete[] when the file was not closed
static void mmapDeleteTests() {

	signalmanager::alarm(60);

	if (!memorymap::isSupported()) {
		stdoutput.printf("memory maps not supported\n");
		signalmanager::alarm(0);
		return;
	}

	// create a file of three 8k blocks
	char	data[24576];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}
	file::remove(mmapfile);
	file	fl;
	test("create",fl.create(mmapfile,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,sizeof(data))==(ssize_t)sizeof(data));
	test("close",fl.close());

	// map the first block with a read
	file	*mfl=new file();
	test("open",mfl->open(mmapfile,O_RDWR));
	mfl->setMmapBufferingEnabled(true);
	test("set read buffer size",mfl->setReadBufferSize(8192));
	char	buf[100];
	test("read 100",mfl->read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("data",!bytestring::compare(buf,data,sizeof(buf)));
	test("block is mapped",mfl->isCurrentBlockMmapBuffered());

	// delete with no close()
	delete mfl;
	test("delete",true);

	file::remove(mmapfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// buffered read after a seek past the end of the file, for #11190
// filedescriptor - a buffered read after a seek past the end of the file
// returns a full read instead of 0, and for #11195 filedescriptor - with
// memory map buffering on, a read after a seek into a block wholly past the
// end of the file crashes with SIGBUS
static void seekPastEndReadTests() {

	signalmanager::alarm(60);

	// a file of two and a half 8k blocks
	char	data[20480];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}
	file::remove(seekfile);
	file	fl;
	test("create",fl.create(seekfile,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,sizeof(data))==(ssize_t)sizeof(data));
	test("close",fl.close());

	// a seek into the last block and into a block after it with map
	// buffering off and on
	off64_t		positions[]={22000,30000,22000,30000};
	bool		maps[]={false,false,true,true};
	const char	*names[]={"last block","block after",
				"last block, mmap","block after, mmap"};
	char		buf[8192];
	stringbuffer	title;
	for (uint16_t i=0; i<4; i++) {

		if (maps[i] && !memorymap::isSupported()) {
			stdoutput.printf("memory maps not supported\n");
			continue;
		}

		file	sfl;
		title.clear();
		title.append(names[i])->append(", open");
		test(title.getString(),sfl.open(seekfile,O_RDWR));
		sfl.setMmapBufferingEnabled(maps[i]);
		title.clear();
		title.append(names[i])->append(", set read buffer size");
		test(title.getString(),sfl.setReadBufferSize(8192));
		off64_t	position=positions[i];
		title.clear();
		title.append(names[i])->append(", seek");
		test(title.getString(),
			sfl.setPositionRelativeToBeginning(position)==
								position);

		// the read gets no data and the position does not move
		title.clear();
		title.append(names[i])->append(", read 8192");
		test(title.getString(),sfl.read(buf,sizeof(buf))==0);
		title.clear();
		title.append(names[i])->append(", position");
		test(title.getString(),sfl.getPosition()==position);
		if (maps[i]) {
			title.clear();
			title.append(names[i])->append(", block is not mapped");
			test(title.getString(),
				!sfl.isCurrentBlockMmapBuffered());
		}
		title.clear();
		title.append(names[i])->append(", close");
		test(title.getString(),sfl.close());
	}

	// a read in the last block, then a seek past the end inside that
	// block, with map buffering off and on
	bool		inmaps[]={false,true};
	const char	*innames[]={"in block","in block, mmap"};
	for (uint16_t i=0; i<2; i++) {

		if (inmaps[i] && !memorymap::isSupported()) {
			stdoutput.printf("memory maps not supported\n");
			continue;
		}

		file	sfl;
		title.clear();
		title.append(innames[i])->append(", open");
		test(title.getString(),sfl.open(seekfile,O_RDWR));
		sfl.setMmapBufferingEnabled(inmaps[i]);
		title.clear();
		title.append(innames[i])->append(", set read buffer size");
		test(title.getString(),sfl.setReadBufferSize(8192));
		title.clear();
		title.append(innames[i])->append(", seek to 20000");
		test(title.getString(),
			sfl.setPositionRelativeToBeginning(20000)==20000);
		title.clear();
		title.append(innames[i])->append(", read 1");
		test(title.getString(),sfl.read(buf,1)==1);
		title.clear();
		title.append(innames[i])->append(", seek to 22000");
		test(title.getString(),
			sfl.setPositionRelativeToBeginning(22000)==22000);

		// the read gets no data and the position does not move
		title.clear();
		title.append(innames[i])->append(", read 8192");
		test(title.getString(),sfl.read(buf,sizeof(buf))==0);
		title.clear();
		title.append(innames[i])->append(", position");
		test(title.getString(),sfl.getPosition()==22000);
		title.clear();
		title.append(innames[i])->append(", close");
		test(title.getString(),sfl.close());
	}

	// a read of a block that is wholly inside the file with map buffering
	// on still maps the block
	if (memorymap::isSupported()) {
		file	cfl;
		test("inside, open",cfl.open(seekfile,O_RDWR));
		cfl.setMmapBufferingEnabled(true);
		test("inside, set read buffer size",cfl.setReadBufferSize(8192));
		test("inside, seek",
			cfl.setPositionRelativeToBeginning(8192)==8192);
		test("inside, read 100",cfl.read(buf,100)==100);
		test("inside, data",!bytestring::compare(buf,data+8192,100));
		test("inside, block is mapped",cfl.isCurrentBlockMmapBuffered());
		test("inside, close",cfl.close());
	} else {
		stdoutput.printf("memory maps not supported\n");
	}

	// the same with a file that ends on a block boundary
	if (memorymap::isSupported()) {
		file::remove(seekfile);
		file	efl;
		test("full last block, create",efl.create(seekfile,
				permissions::parsePermString("rw-rw----")));
		test("full last block, write",efl.write(data,16384)==16384);
		test("full last block, close",efl.close());
		file	cfl;
		test("full last block, open",cfl.open(seekfile,O_RDWR));
		cfl.setMmapBufferingEnabled(true);
		test("full last block, set read buffer size",
						cfl.setReadBufferSize(8192));
		test("full last block, seek",
			cfl.setPositionRelativeToBeginning(8192)==8192);
		test("full last block, read 100",cfl.read(buf,100)==100);
		test("full last block, data",
				!bytestring::compare(buf,data+8192,100));
		test("full last block, block is mapped",
					cfl.isCurrentBlockMmapBuffered());
		test("full last block, close",cfl.close());
	} else {
		stdoutput.printf("memory maps not supported\n");
	}

	// a write after the seek and read lands at the seek position, with
	// map buffering off and on
	bool		wmaps[]={false,true};
	const char	*wnames[]={"write after","write after, mmap"};
	char		*contents=new char[30010];
	char		*zeros=new char[30010];
	bytestring::zero(zeros,30010);
	for (uint16_t i=0; i<2; i++) {

		if (wmaps[i] && !memorymap::isSupported()) {
			stdoutput.printf("memory maps not supported\n");
			continue;
		}

		// start with the original file
		file::remove(seekfile);
		file	nfl;
		title.clear();
		title.append(wnames[i])->append(", create");
		test(title.getString(),nfl.create(seekfile,
				permissions::parsePermString("rw-rw----")));
		title.clear();
		title.append(wnames[i])->append(", write file");
		test(title.getString(),
			nfl.write(data,sizeof(data))==(ssize_t)sizeof(data));
		title.clear();
		title.append(wnames[i])->append(", close file");
		test(title.getString(),nfl.close());

		file	wfl;
		title.clear();
		title.append(wnames[i])->append(", open");
		test(title.getString(),wfl.open(seekfile,O_RDWR));
		wfl.setMmapBufferingEnabled(wmaps[i]);
		title.clear();
		title.append(wnames[i])->append(", set read buffer size");
		test(title.getString(),wfl.setReadBufferSize(8192));
		title.clear();
		title.append(wnames[i])->append(", seek");
		test(title.getString(),
			wfl.setPositionRelativeToBeginning(30000)==30000);
		title.clear();
		title.append(wnames[i])->append(", read 8192");
		test(title.getString(),wfl.read(buf,sizeof(buf))==0);
		title.clear();
		title.append(wnames[i])->append(", write 10");
		test(title.getString(),wfl.write("0123456789",10)==10);
		title.clear();
		title.append(wnames[i])->append(", flush");
		test(title.getString(),wfl.flushWriteBuffer(-1,-1));
		title.clear();
		title.append(wnames[i])->append(", close");
		test(title.getString(),wfl.close());

		// the data is intact, the gap is zeros and the write is in place
		file	rfl;
		title.clear();
		title.append(wnames[i])->append(", open for read");
		test(title.getString(),rfl.open(seekfile,O_RDONLY));
		title.clear();
		title.append(wnames[i])->append(", size");
		test(title.getString(),rfl.getSize()==30010);
		title.clear();
		title.append(wnames[i])->append(", read file");
		test(title.getString(),rfl.read(contents,30010)==30010);
		title.clear();
		title.append(wnames[i])->append(", data");
		test(title.getString(),
			!bytestring::compare(contents,data,sizeof(data)));
		title.clear();
		title.append(wnames[i])->append(", gap");
		test(title.getString(),
			!bytestring::compare(contents+sizeof(data),zeros,
							30000-sizeof(data)));
		title.clear();
		title.append(wnames[i])->append(", bytes");
		test(title.getString(),
			!bytestring::compare(contents+30000,"0123456789",10));
		title.clear();
		title.append(wnames[i])->append(", close for read");
		test(title.getString(),rfl.close());
	}
	delete[] contents;
	delete[] zeros;

	file::remove(seekfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// builds the title of one check from the case name and the check name
static const char *truncateTitle(stringbuffer *title,
					const char *name, const char *what) {
	title->clear();
	title->append(name)->append(", ")->append(what);
	return title->getString();
}

// #11198 filedescriptor - a buffered read after file::truncate() cuts off the
// buffered block crashes with SIGBUS (mmap) or returns old bytes
static void truncateBufferTests() {

	signalmanager::alarm(60);

	// a file of two and a half 8k blocks
	char	data[20480];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}

	// cases: read below, read inside, dirty below, dirty inside, write
	// after, each with map buffering off and on
	bool		maps[]={false,true};
	const char	*names[]={"read","read, mmap",
				"inside","inside, mmap",
				"dirty","dirty, mmap",
				"dirty inside","dirty inside, mmap",
				"write after","write after, mmap"};
	char		buf[1000];
	char		contents[20480];
	char		zeros[8192];
	bytestring::zero(zeros,sizeof(zeros));
	stringbuffer	title;
	const char	*wbytes="0123456789012345678901234567890123456789"
				"0123456789012345678901234567890123456789"
				"01234567890123456789";
	for (uint16_t n=0; n<5; n++) {
	for (uint16_t m=0; m<2; m++) {

		if (maps[m] && !memorymap::isSupported()) {
			stdoutput.printf("memory maps not supported\n");
			continue;
		}

		const char	*name=names[n*2+m];

		// start with the original file
		file::remove(seekfile);
		file	nfl;
		test(truncateTitle(&title,name,"create"),
			nfl.create(seekfile,
				permissions::parsePermString("rw-rw----")));
		test(truncateTitle(&title,name,"write file"),
			nfl.write(data,sizeof(data))==(ssize_t)sizeof(data));
		test(truncateTitle(&title,name,"close file"),nfl.close());

		// buffer the block at 8192
		file	tfl;
		test(truncateTitle(&title,name,"open"),
					tfl.open(seekfile,O_RDWR));
		tfl.setMmapBufferingEnabled(maps[m]);
		test(truncateTitle(&title,name,"set read buffer size"),
					tfl.setReadBufferSize(8192));
		test(truncateTitle(&title,name,"set write buffer size"),
					tfl.setWriteBufferSize(8192));
		test(truncateTitle(&title,name,"seek"),
			tfl.setPositionRelativeToBeginning(8192)==8192);
		if (n<2 || n==4) {
			test(truncateTitle(&title,name,"read 100"),
						tfl.read(buf,100)==100);
			test(truncateTitle(&title,name,"read 100 data"),
				!bytestring::compare(buf,data+8192,100));
			if (maps[m]) {
				test(truncateTitle(&title,name,
							"block is mapped"),
					tfl.isCurrentBlockMmapBuffered());
			}
		} else {
			// dirty write, not flushed
			test(truncateTitle(&title,name,"write 100"),
				tfl.write(wbytes,100)==100);
		}

		// cut below the block or inside it
		off64_t	size=(n==1 || n==3)?9000:4096;
		test(truncateTitle(&title,name,"truncate"),tfl.truncate(size));

		if (n==0) {

			// read after the cut
			test(truncateTitle(&title,name,"read after"),
						tfl.read(buf,100)==0);
			test(truncateTitle(&title,name,"close"),tfl.close());

		} else if (n==1) {

			// read the rest of the cut block
			test(truncateTitle(&title,name,"read after"),
						tfl.read(buf,1000)==708);
			test(truncateTitle(&title,name,"read after data"),
				!bytestring::compare(buf,data+8292,708));
			test(truncateTitle(&title,name,"read at end"),
						tfl.read(buf,1000)==0);
			test(truncateTitle(&title,name,"close"),tfl.close());

		} else if (n==3) {

			// check the file
			test(truncateTitle(&title,name,"close"),tfl.close());
			file	rfl;
			test(truncateTitle(&title,name,"open for read"),
					rfl.open(seekfile,O_RDONLY));
			test(truncateTitle(&title,name,"size"),
						rfl.getSize()==9000);
			test(truncateTitle(&title,name,"read file"),
				rfl.read(contents,9000)==9000);
			test(truncateTitle(&title,name,"data before"),
				!bytestring::compare(contents,data,8192));
			test(truncateTitle(&title,name,"bytes"),
				!bytestring::compare(contents+8192,
				wbytes,100));
			test(truncateTitle(&title,name,"data after"),
				!bytestring::compare(contents+8292,
							data+8292,708));
			test(truncateTitle(&title,name,"close for read"),
							rfl.close());

		} else if (n==2) {

			// check the file
			test(truncateTitle(&title,name,"close"),tfl.close());
			file	rfl;
			test(truncateTitle(&title,name,"open for read"),
					rfl.open(seekfile,O_RDONLY));
			test(truncateTitle(&title,name,"size"),
						rfl.getSize()==4096);
			test(truncateTitle(&title,name,"read file"),
				rfl.read(contents,4096)==4096);
			test(truncateTitle(&title,name,"data"),
				!bytestring::compare(contents,data,4096));
			test(truncateTitle(&title,name,"close for read"),
							rfl.close());

		} else {

			// write after the cut
			test(truncateTitle(&title,name,"seek after"),
				tfl.setPositionRelativeToBeginning(8192)==8192);
			test(truncateTitle(&title,name,"write after"),
				tfl.write(wbytes,100)==100);
			test(truncateTitle(&title,name,"close"),tfl.close());
			file	rfl;
			test(truncateTitle(&title,name,"open for read"),
					rfl.open(seekfile,O_RDONLY));
			test(truncateTitle(&title,name,"size"),
						rfl.getSize()==8292);
			test(truncateTitle(&title,name,"read file"),
				rfl.read(contents,8292)==8292);
			test(truncateTitle(&title,name,"data before"),
				!bytestring::compare(contents,data,4096));
			test(truncateTitle(&title,name,"gap"),
				!bytestring::compare(contents+4096,zeros,4096));
			test(truncateTitle(&title,name,"bytes"),
				!bytestring::compare(contents+8192,
				wbytes,100));
			test(truncateTitle(&title,name,"close for read"),
							rfl.close());
		}
	}
	}

	file::remove(seekfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// buffered write after a seek past the end of the file, for #11196
// filedescriptor - a buffered write after a seek past the end of the file
// fills the gap with old buffer bytes instead of zeros
static void seekPastEndWriteTests() {

	signalmanager::alarm(60);

	// a file of two and a half 8k blocks
	char	data[20480];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}

	// a seek into a block after the last block, or into the last block
	// past the end, then a write, with map buffering off and on, with no
	// read first, after a read of a full block, and after a read of the
	// last block
	off64_t		positions[]={30000,30000,30000,30000,30000,
					22000,22000,22000};
	bool		maps[]={false,true,false,true,true,
					false,true,false};
	off64_t		readpositions[]={-1,-1,0,0,16384,
					-1,-1,0};
	ssize_t		readsizes[]={0,0,8192,8192,4096,
					0,0,8192};
	const char	*names[]={
				"block after",
				"block after, mmap",
				"block after, full block first",
				"block after, full block first, mmap",
				"block after, last block first, mmap",
				"last block",
				"last block, mmap",
				"last block, full block first"
				};
	char		buf[8192];
	char		*contents=new char[30010];
	char		*zeros=new char[30010];
	bytestring::zero(zeros,30010);
	stringbuffer	title;
	for (uint16_t i=0; i<8; i++) {

		if (maps[i] && !memorymap::isSupported()) {
			stdoutput.printf("memory maps not supported\n");
			continue;
		}

		// start with the original file
		file::remove(seekfile);
		file	nfl;
		title.clear();
		title.append(names[i])->append(", create");
		test(title.getString(),nfl.create(seekfile,
				permissions::parsePermString("rw-rw----")));
		title.clear();
		title.append(names[i])->append(", write file");
		test(title.getString(),
			nfl.write(data,sizeof(data))==(ssize_t)sizeof(data));
		title.clear();
		title.append(names[i])->append(", close file");
		test(title.getString(),nfl.close());

		file	wfl;
		title.clear();
		title.append(names[i])->append(", open");
		test(title.getString(),wfl.open(seekfile,O_RDWR));
		wfl.setMmapBufferingEnabled(maps[i]);
		title.clear();
		title.append(names[i])->append(", set write buffer size");
		test(title.getString(),wfl.setWriteBufferSize(8192));

		// fill the buffer with file data first
		if (readpositions[i]!=-1) {
			title.clear();
			title.append(names[i])->append(", seek to read");
			test(title.getString(),
				wfl.setPositionRelativeToBeginning(
						readpositions[i])==
						readpositions[i]);
			title.clear();
			title.append(names[i])->append(", read");
			test(title.getString(),
				wfl.read(buf,readsizes[i])==readsizes[i]);
		}

		off64_t	position=positions[i];
		title.clear();
		title.append(names[i])->append(", seek");
		test(title.getString(),
			wfl.setPositionRelativeToBeginning(position)==
								position);
		title.clear();
		title.append(names[i])->append(", write 10");
		test(title.getString(),wfl.write("0123456789",10)==10);
		title.clear();
		title.append(names[i])->append(", flush");
		test(title.getString(),wfl.flushWriteBuffer(-1,-1));
		title.clear();
		title.append(names[i])->append(", close");
		test(title.getString(),wfl.close());

		// the data is intact, the gap is zeros and the write is in place
		file	rfl;
		title.clear();
		title.append(names[i])->append(", open for read");
		test(title.getString(),rfl.open(seekfile,O_RDONLY));
		title.clear();
		title.append(names[i])->append(", size");
		test(title.getString(),rfl.getSize()==position+10);
		title.clear();
		title.append(names[i])->append(", read file");
		test(title.getString(),
			rfl.read(contents,position+10)==position+10);
		title.clear();
		title.append(names[i])->append(", data");
		test(title.getString(),
			!bytestring::compare(contents,data,sizeof(data)));
		title.clear();
		title.append(names[i])->append(", gap");
		test(title.getString(),
			!bytestring::compare(contents+sizeof(data),zeros,
						position-sizeof(data)));
		title.clear();
		title.append(names[i])->append(", bytes");
		test(title.getString(),
			!bytestring::compare(contents+position,"0123456789",10));
		title.clear();
		title.append(names[i])->append(", close for read");
		test(title.getString(),rfl.close());
	}
	delete[] contents;
	delete[] zeros;

	file::remove(seekfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// buffered seek before the start of the file, for #11199 filedescriptor - a
// buffered seek to a position before the start of the file is not refused,
// so a later read or write can touch memory before the buffer
static void negativeSeekTests() {

	signalmanager::alarm(60);

	// a file of two and a half 8k blocks
	char	data[20480];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}

	// the expected file after the writes below
	char	expected[20480];
	bytestring::copy(expected,data,sizeof(data));
	bytestring::copy(expected+15,"XYZ012345",9);

	bool		maps[]={false,true};
	const char	*names[]={"negative seek","negative seek, mmap"};
	char		buf[64];
	char		*contents=new char[sizeof(data)];
	stringbuffer	title;
	for (uint16_t i=0; i<2; i++) {

		if (maps[i] && !memorymap::isSupported()) {
			stdoutput.printf("memory maps not supported\n");
			continue;
		}

		// start with the original file
		file::remove(seekfile);
		file	nfl;
		title.clear();
		title.append(names[i])->append(", create");
		test(title.getString(),nfl.create(seekfile,
				permissions::parsePermString("rw-rw----")));
		title.clear();
		title.append(names[i])->append(", write file");
		test(title.getString(),
			nfl.write(data,sizeof(data))==(ssize_t)sizeof(data));
		title.clear();
		title.append(names[i])->append(", close file");
		test(title.getString(),nfl.close());

		file	fl;
		title.clear();
		title.append(names[i])->append(", open");
		test(title.getString(),fl.open(seekfile,O_RDWR));
		fl.setMmapBufferingEnabled(maps[i]);
		title.clear();
		title.append(names[i])->append(", set write buffer size");
		test(title.getString(),fl.setWriteBufferSize(8192));

		// a seek before any buffer exists fails
		title.clear();
		title.append(names[i])->append(", seek -100 first");
		error::clearError();
		test(title.getString(),
			fl.setPositionRelativeToBeginning(-100)==-1 &&
			error::getErrorNumber()==EINVAL);
		title.clear();
		title.append(names[i])->append(", position 0");
		test(title.getString(),fl.getPosition()==0);

		// fill the buffer
		title.clear();
		title.append(names[i])->append(", read 10");
		test(title.getString(),fl.read(buf,10)==10 &&
					!bytestring::compare(buf,data,10));

		// seeks from the beginning, the current position and the end
		// fail and leave the position as it was
		title.clear();
		title.append(names[i])->append(", seek beginning -100");
		error::clearError();
		test(title.getString(),
			fl.setPositionRelativeToBeginning(-100)==-1 &&
			error::getErrorNumber()==EINVAL);
		title.clear();
		title.append(names[i])->append(", position 10");
		test(title.getString(),fl.getPosition()==10);
		title.clear();
		title.append(names[i])->append(", seek current -11");
		error::clearError();
		test(title.getString(),
			fl.setPositionRelativeToCurrent(-11)==-1 &&
			error::getErrorNumber()==EINVAL);
		title.clear();
		title.append(names[i])->append(", position 10 again");
		test(title.getString(),fl.getPosition()==10);
		title.clear();
		title.append(names[i])->append(", seek end -20481");
		error::clearError();
		test(title.getString(),
			fl.setPositionRelativeToEnd(-20481)==-1 &&
			error::getErrorNumber()==EINVAL);
		title.clear();
		title.append(names[i])->append(", position 10 once more");
		test(title.getString(),fl.getPosition()==10);

		// a read and a write work at the old position
		title.clear();
		title.append(names[i])->append(", read 5");
		test(title.getString(),fl.read(buf,5)==5 &&
					!bytestring::compare(buf,data+10,5));
		title.clear();
		title.append(names[i])->append(", write 3");
		test(title.getString(),fl.write("XYZ",3)==3);

		// a write after a failed seek to -100, then a failed seek to
		// -50, lands at the old position
		title.clear();
		title.append(names[i])->append(", seek -100");
		test(title.getString(),
			fl.setPositionRelativeToBeginning(-100)==-1);
		title.clear();
		title.append(names[i])->append(", write after -100");
		test(title.getString(),fl.write("012",3)==3);
		title.clear();
		title.append(names[i])->append(", seek -50");
		test(title.getString(),
			fl.setPositionRelativeToBeginning(-50)==-1);
		title.clear();
		title.append(names[i])->append(", write after -50");
		test(title.getString(),fl.write("345",3)==3);
		title.clear();
		title.append(names[i])->append(", position 24");
		test(title.getString(),fl.getPosition()==24);

		// the descriptor still works
		title.clear();
		title.append(names[i])->append(", seek 0");
		test(title.getString(),
			fl.setPositionRelativeToBeginning(0)==0);
		title.clear();
		title.append(names[i])->append(", read 24");
		test(title.getString(),fl.read(buf,24)==24 &&
					!bytestring::compare(buf,expected,24));
		title.clear();
		title.append(names[i])->append(", close");
		test(title.getString(),fl.close());

		// the writes are in the file
		file	rfl;
		title.clear();
		title.append(names[i])->append(", open for read");
		test(title.getString(),rfl.open(seekfile,O_RDONLY));
		title.clear();
		title.append(names[i])->append(", size");
		test(title.getString(),rfl.getSize()==(off64_t)sizeof(data));
		title.clear();
		title.append(names[i])->append(", read file");
		test(title.getString(),
			rfl.read(contents,sizeof(data))==(ssize_t)sizeof(data));
		title.clear();
		title.append(names[i])->append(", data");
		test(title.getString(),
			!bytestring::compare(contents,expected,sizeof(data)));
		title.clear();
		title.append(names[i])->append(", close for read");
		test(title.getString(),rfl.close());
	}
	delete[] contents;

	file::remove(seekfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

#ifdef FILETEST_BLOCKDEV_HOOK
// the file descriptor that fstat(), ioctl() and lseek() fake a block device
// on, and whether the fake device fails to report its size
static int	fakeblkfd=-1;
static bool	fakeblkfail=false;
#endif

#ifdef FILETEST_FSTAT_HOOK
// the file descriptor that fstat() fails on once
static int	failstatfd=-1;

typedef int	(*fstatfunc_t)(int,struct stat *);

// the C library's fstat()
static int realFstat(int fd, struct stat *st) {
	static fstatfunc_t	realfstat=NULL;
	if (!realfstat) {
		realfstat=(fstatfunc_t)dynamiclib::getNextSymbol("fstat");
		if (!realfstat) {
			error::setErrorNumber(ENOSYS);
			return -1;
		}
	}
	return realfstat(fd,st);
}
#endif

#ifdef FILETEST_LSEEK_HOOK
// This lseek() takes the place of the C library's, so the test can make a
// seek fail on one file descriptor.  It passes every other call on to the
// next definition, so it is only built where getNextSymbol() can find that.
static int	failseekfd=-1;

typedef off_t	(*lseekfunc_t)(int,off_t,int);

extern "C" off_t lseek(int fd, off_t offset, int whence) {
	if (fd==failseekfd) {
		error::setErrorNumber(EINVAL);
		return -1;
	}
	#ifdef FILETEST_BLOCKDEV_HOOK
	// a seek past the end of a block device fails, like in the kernel
	if (fd==fakeblkfd && whence==SEEK_SET) {
		struct stat	st;
		if (realFstat(fd,&st)==0 && offset>st.st_size) {
			error::setErrorNumber(EINVAL);
			return -1;
		}
	}
	#endif
	static lseekfunc_t	reallseek=NULL;
	if (!reallseek) {
		reallseek=(lseekfunc_t)dynamiclib::getNextSymbol("lseek");
		if (!reallseek) {
			error::setErrorNumber(ENOSYS);
			return -1;
		}
	}
	return reallseek(fd,offset,whence);
}

// This read() takes the place of the C library's, so the test can make a read
// fail once on one file descriptor, or first return part of what it asked
// for.  It passes every other call on to the next definition.
static int	failreadfd=-1;
static size_t	failreadshort=0;

typedef ssize_t	(*readfunc_t)(int,void *,size_t);

extern "C" ssize_t read(int fd, void *buf, size_t count) {
	static readfunc_t	realread=NULL;
	if (!realread) {
		realread=(readfunc_t)dynamiclib::getNextSymbol("read");
		if (!realread) {
			error::setErrorNumber(ENOSYS);
			return -1;
		}
	}
	if (fd==failreadfd) {
		if (failreadshort) {
			if (count>failreadshort) {
				count=failreadshort;
			}
			failreadshort=0;
			return realread(fd,buf,count);
		}
		failreadfd=-1;
		error::setErrorNumber(EIO);
		return -1;
	}
	return realread(fd,buf,count);
}

#ifdef FILETEST_MMAP_HOOK
// This mmap() takes the place of the C library's, so the test can make every
// map fail on one file descriptor.  It passes every other call on to the next
// definition.
static int	failmapfd=-1;

typedef void	*(*mmapfunc_t)(void *,size_t,int,int,int,off_t);

extern "C" void *mmap(void *addr, size_t length, int prot,
				int flags, int fd, off_t offset) {
	if (failmapfd!=-1 && fd==failmapfd) {
		error::setErrorNumber(ENODEV);
		return MAP_FAILED;
	}
	static mmapfunc_t	realmmap=NULL;
	if (!realmmap) {
		realmmap=(mmapfunc_t)dynamiclib::getNextSymbol("mmap");
		if (!realmmap) {
			error::setErrorNumber(ENOSYS);
			return MAP_FAILED;
		}
	}
	return realmmap(addr,length,prot,flags,fd,offset);
}
#endif

#ifdef FILETEST_FSTAT_HOOK
// This fstat() takes the place of the C library's, so the test can make it
// fail once on one file descriptor, or make one file descriptor look like a
// block device of size 0, as a real one does.  It passes every other call on
// to the next definition.
extern "C" int fstat(int fd, struct stat *st) {
	if (fd==failstatfd) {
		failstatfd=-1;
		error::setErrorNumber(EIO);
		return -1;
	}
	#ifdef FILETEST_BLOCKDEV_HOOK
	if (fd==fakeblkfd) {
		int	result=realFstat(fd,st);
		if (result==0) {
			st->st_mode=(st->st_mode&~S_IFMT)|S_IFBLK;
			st->st_size=0;
		}
		return result;
	}
	#endif
	return realFstat(fd,st);
}
#endif

#ifdef FILETEST_BLOCKDEV_HOOK
// This ioctl() takes the place of the C library's, so the test can make the
// fake block device report the size of its file, or fail to.  It passes every
// other call on to the next definition.
typedef int	(*ioctlfunc_t)(int,unsigned long,...);

extern "C" int ioctl(int fd, unsigned long req, ...) {
	va_list	ap;
	va_start(ap,req);
	void	*arg=va_arg(ap,void *);
	va_end(ap);
	if (fd==fakeblkfd && (unsigned int)req==(unsigned int)BLKGETSIZE64) {
		if (fakeblkfail) {
			error::setErrorNumber(ENOTTY);
			return -1;
		}
		struct stat	st;
		if (realFstat(fd,&st)!=0) {
			return -1;
		}
		*((uint64_t *)arg)=(uint64_t)st.st_size;
		return 0;
	}
	static ioctlfunc_t	realioctl=NULL;
	if (!realioctl) {
		realioctl=(ioctlfunc_t)dynamiclib::getNextSymbol("ioctl");
		if (!realioctl) {
			error::setErrorNumber(ENOSYS);
			return -1;
		}
	}
	return realioctl(fd,req,arg);
}
#endif

// open the file read-only, so a map fails, read the first block into a usual
// buffer, then fail the seek to the second block
static void mmapFailedSeekOpen(file *fl, const char *data) {
	test("open",fl->open(mmapfile,O_RDONLY));
	fl->setMmapBufferingEnabled(true);
	test("set read buffer size",fl->setReadBufferSize(8192));
	char	buf[100];
	test("read 100",fl->read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("data",!bytestring::compare(buf,data,sizeof(buf)));
	test("block is not mapped",!fl->isCurrentBlockMmapBuffered());
	test("seek",fl->setPositionRelativeToBeginning(8192)==8192);
	failseekfd=fl->getFileDescriptor();
	test("read, failed seek",fl->read(buf,sizeof(buf))==-1);
	failseekfd=-1;
}

// failed seek after a failed map, for #11193 filedescriptor - with memory
// map buffering on, a failed seek after a failed map leaks the usual write
// buffer
static void mmapFailedSeekTests() {

	signalmanager::alarm(60);

	if (!memorymap::isSupported()) {
		stdoutput.printf("memory maps not supported\n");
		signalmanager::alarm(0);
		return;
	}

	// create a file of three 8k blocks
	char	data[24576];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}
	file::remove(mmapfile);
	file	fl;
	test("create",fl.create(mmapfile,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,sizeof(data))==(ssize_t)sizeof(data));
	test("close",fl.close());

	// skip if the lseek() above does not take the place of the real one
	file	sfl;
	test("seek check, open",sfl.open(mmapfile,O_RDONLY));
	failseekfd=sfl.getFileDescriptor();
	bool	failed=(sfl.getPosition()==-1);
	failseekfd=-1;
	test("seek check, close",sfl.close());
	if (!failed) {
		stdoutput.printf("lseek() can not be replaced\n");
		file::remove(mmapfile);
		signalmanager::alarm(0);
		return;
	}

	char	buf[100];

	// delete with no close() after the failed seek
	stdoutput.printf("delete:\n");
	file	*dfl=new file();
	mmapFailedSeekOpen(dfl,data);
	delete dfl;
	test("delete",true);

	// close() after the failed seek, then open and read again
	stdoutput.printf("close and reopen:\n");
	file	cfl;
	mmapFailedSeekOpen(&cfl,data);
	test("close",cfl.close());
	test("reopen",cfl.open(mmapfile,O_RDONLY));
	test("reopen, read 100",
		cfl.read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("reopen, data",!bytestring::compare(buf,data,sizeof(buf)));
	test("reopen, close",cfl.close());

	// read the next block after the failed seek
	stdoutput.printf("read next block:\n");
	file	rfl;
	mmapFailedSeekOpen(&rfl,data);
	test("block is not mapped after failed seek",
					!rfl.isCurrentBlockMmapBuffered());
	test("read 100",rfl.read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("data",!bytestring::compare(buf,data+8192,sizeof(buf)));
	test("position",rfl.getPosition()==8292);
	test("close",rfl.close());

	file::remove(mmapfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// one run of failedSeekWriteTests(), false if lseek() can not be replaced
static bool failedSeekWriteRun(bool usemmap) {

	// create a file of three 8k blocks
	char	data[24576];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}
	file::remove(mmapfile);
	file	fl;
	test("create",fl.create(mmapfile,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,sizeof(data))==(ssize_t)sizeof(data));
	test("close",fl.close());

	// skip if the lseek() above does not take the place of the real one
	file	sfl;
	test("seek check, open",sfl.open(mmapfile,O_RDONLY));
	failseekfd=sfl.getFileDescriptor();
	bool	failed=(sfl.getPosition()==-1);
	failseekfd=-1;
	test("seek check, close",sfl.close());
	if (!failed) {
		stdoutput.printf("lseek() can not be replaced\n");
		file::remove(mmapfile);
		return false;
	}

	// read the first block into a usual buffer, with every map failing
	test("open",fl.open(mmapfile,O_RDWR));
	fl.setMmapBufferingEnabled(usemmap);
	test("set read buffer size",fl.setReadBufferSize(8192));
	#ifdef FILETEST_MMAP_HOOK
	if (usemmap) {
		failmapfd=fl.getFileDescriptor();
	}
	#endif
	char	buf[100];
	test("read 100",fl.read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("data",!bytestring::compare(buf,data,sizeof(buf)));

	// skip if the mmap() above does not take the place of the real one
	if (fl.isCurrentBlockMmapBuffered()) {
		stdoutput.printf("mmap() can not be replaced\n");
		test("close",fl.close());
		#ifdef FILETEST_MMAP_HOOK
		failmapfd=-1;
		#endif
		file::remove(mmapfile);
		return true;
	}
	test("block is not mapped",!fl.isCurrentBlockMmapBuffered());

	// fail the seek to the second block
	test("seek",fl.setPositionRelativeToBeginning(8192)==8192);
	failseekfd=fl.getFileDescriptor();
	test("read, failed seek",fl.read(buf,sizeof(buf))==-1);
	failseekfd=-1;

	// write and flush inside the second block
	test("seek 2",fl.setPositionRelativeToBeginning(8292)==8292);
	test("write",fl.write("XYZ",3)==3);
	test("flush",fl.flushWriteBuffer(-1,-1));
	test("close",fl.close());
	#ifdef FILETEST_MMAP_HOOK
	failmapfd=-1;
	#endif

	// only the written bytes may differ in the file
	bytestring::copy(data+8292,"XYZ",3);
	char	*contents=new char[sizeof(data)+1];
	file	vfl;
	test("verify, open",vfl.open(mmapfile,O_RDONLY));
	test("size",vfl.getSize()==(off64_t)sizeof(data));
	test("verify, read",vfl.read(contents,sizeof(data))==
						(ssize_t)sizeof(data));
	test("start of block",!bytestring::compare(contents+8192,
						"cdefghij",8));
	test("data after",!bytestring::compare(contents,data,sizeof(data)));
	test("verify, close",vfl.close());
	delete[] contents;

	file::remove(mmapfile);
	return true;
}

// failed seek to a new block, then write and flush, for #11201 filedescriptor
// - after a failed seek to a new block, the buffer keeps the old bytes, so a
// later write and flush save them into the new block
static void failedSeekWriteTests() {

	signalmanager::alarm(60);

	// memory map buffering off
	stdoutput.printf("memory map buffering off:\n");
	if (failedSeekWriteRun(false)) {

		// memory map buffering on
		stdoutput.printf("memory map buffering on:\n");
		#ifdef FILETEST_MMAP_HOOK
		if (memorymap::isSupported()) {
			failedSeekWriteRun(true);
		} else {
			stdoutput.printf("memory maps not supported\n");
		}
		#else
		stdoutput.printf("mmap() can not be replaced\n");
		#endif
	}
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// how failedFillWriteRun() fails the fill of the second block
enum fillfail {
	FILLFAIL_STAT,
	FILLFAIL_READ,
	FILLFAIL_SHORTREAD
};

// one run of failedFillWriteTests(), false if the fstat() or read() that "how"
// needs can not be replaced
static bool failedFillWriteRun(fillfail how, bool usemmap, bool mapfirst) {

	// create a file of three 8k blocks
	char	data[24576];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]='a'+i%26;
	}
	file::remove(mmapfile);
	file	fl;
	test("create",fl.create(mmapfile,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,sizeof(data))==(ssize_t)sizeof(data));
	test("close",fl.close());

	// skip if the fstat() or read() above does not take the place of the
	// real one
	file	sfl;
	test("hook check, open",sfl.open(mmapfile,O_RDONLY));
	bool	failed=false;
	if (how==FILLFAIL_STAT) {
		#ifdef FILETEST_FSTAT_HOOK
		failstatfd=sfl.getFileDescriptor();
		failed=!sfl.getCurrentProperties();
		failstatfd=-1;
		#endif
	} else {
		char	c;
		failreadfd=sfl.getFileDescriptor();
		failreadshort=0;
		failed=(sfl.read(&c,1)==-1);
		failreadfd=-1;
	}
	test("hook check, close",sfl.close());
	if (!failed) {
		stdoutput.printf("%s() can not be replaced\n",
				(how==FILLFAIL_STAT)?"fstat":"read");
		file::remove(mmapfile);
		return false;
	}

	// read the first block, into a usual buffer unless "mapfirst" is set
	test("open",fl.open(mmapfile,O_RDWR));
	fl.setMmapBufferingEnabled(usemmap);
	test("set read buffer size",fl.setReadBufferSize(8192));
	#ifdef FILETEST_MMAP_HOOK
	if (usemmap && !mapfirst) {
		failmapfd=fl.getFileDescriptor();
	}
	#endif
	char	buf[100];
	test("read 100",fl.read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("data",!bytestring::compare(buf,data,sizeof(buf)));
	if (mapfirst) {
		test("block is mapped",fl.isCurrentBlockMmapBuffered());
	} else {

		// skip if the mmap() above does not take the place of the
		// real one
		if (fl.isCurrentBlockMmapBuffered()) {
			stdoutput.printf("mmap() can not be replaced\n");
			test("close",fl.close());
			#ifdef FILETEST_MMAP_HOOK
			failmapfd=-1;
			#endif
			file::remove(mmapfile);
			return true;
		}
		test("block is not mapped",!fl.isCurrentBlockMmapBuffered());
	}

	// fail the fill of the second block
	test("seek",fl.setPositionRelativeToBeginning(8192)==8192);
	if (how==FILLFAIL_STAT) {
		#ifdef FILETEST_FSTAT_HOOK
		failstatfd=fl.getFileDescriptor();
		#endif
	} else {
		failreadfd=fl.getFileDescriptor();
		failreadshort=(how==FILLFAIL_SHORTREAD)?50:0;
	}
	test("read, failed fill",fl.read(buf,sizeof(buf))==-1);
	#ifdef FILETEST_FSTAT_HOOK
	failstatfd=-1;
	#endif
	failreadfd=-1;
	failreadshort=0;
	if (mapfirst) {
		test("block is not mapped after failed fill",
					!fl.isCurrentBlockMmapBuffered());
	}

	// write and flush inside the second block
	test("seek 2",fl.setPositionRelativeToBeginning(8292)==8292);
	test("write",fl.write("XYZ",3)==3);
	test("flush",fl.flushWriteBuffer(-1,-1));
	test("close",fl.close());
	#ifdef FILETEST_MMAP_HOOK
	failmapfd=-1;
	#endif

	// only the written bytes may differ in the file
	bytestring::copy(data+8292,"XYZ",3);
	char	*contents=new char[sizeof(data)+1];
	file	vfl;
	test("verify, open",vfl.open(mmapfile,O_RDONLY));
	test("size",vfl.getSize()==(off64_t)sizeof(data));
	test("verify, read",vfl.read(contents,sizeof(data))==
						(ssize_t)sizeof(data));
	test("start of block",!bytestring::compare(contents+8192,
						"cdefghij",8));
	test("data after",!bytestring::compare(contents,data,sizeof(data)));
	test("verify, close",vfl.close());
	delete[] contents;

	file::remove(mmapfile);
	return true;
}

// failed size check or failed fill of a new block, then write and flush, for
// #11206 filedescriptor - after a failed size check or a failed fill of a new
// block, the buffer keeps stale bytes, so a later write and flush save them
// into the new block
static void failedFillWriteTests() {

	signalmanager::alarm(60);

	// failed size check, with memory map buffering on
	stdoutput.printf("failed size check:\n");
	#if !defined(FILETEST_FSTAT_HOOK)
	stdoutput.printf("fstat() can not be replaced\n");
	#elif !defined(FILETEST_MMAP_HOOK)
	stdoutput.printf("mmap() can not be replaced\n");
	#else
	if (!memorymap::isSupported()) {
		stdoutput.printf("memory maps not supported\n");
	} else if (failedFillWriteRun(FILLFAIL_STAT,true,false)) {
		stdoutput.printf("failed size check, first block mapped:\n");
		failedFillWriteRun(FILLFAIL_STAT,true,true);
	}
	#endif

	// failed read, memory map buffering off
	stdoutput.printf("failed read, memory map buffering off:\n");
	if (failedFillWriteRun(FILLFAIL_READ,false,false)) {

		// failed read, memory map buffering on
		stdoutput.printf("failed read, memory map buffering on:\n");
		#ifdef FILETEST_MMAP_HOOK
		if (memorymap::isSupported()) {
			failedFillWriteRun(FILLFAIL_READ,true,false);
		} else {
			stdoutput.printf("memory maps not supported\n");
		}
		#else
		stdoutput.printf("mmap() can not be replaced\n");
		#endif

		// failed read after 50 bytes
		stdoutput.printf("failed read after 50 bytes:\n");
		failedFillWriteRun(FILLFAIL_SHORTREAD,false,false);
	}
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}
#endif

#ifdef FILETEST_BLOCKDEV_HOOK
// create a file of the pattern
static void blockDeviceCreate(const char *name, char *data, size_t size) {
	for (size_t i=0; i<size; i++) {
		data[i]='a'+i%26;
	}
	file::remove(name);
	file	fl;
	test("create",fl.create(name,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,size)==(ssize_t)size);
	test("close",fl.close());
}

// open the file with map buffering on and fake a block device on it
static void blockDeviceOpen(file *fl, const char *name) {
	test("open",fl->open(name,O_RDWR));
	fl->setMmapBufferingEnabled(true);
	test("set read buffer size",fl->setReadBufferSize(8192));
	fakeblkfd=fl->getFileDescriptor();
}

// seek and read 8192 bytes, in a block not wholly inside the device
static void blockDeviceReadNone(const char *name, off64_t position) {
	file	fl;
	char	buf[8192];
	blockDeviceOpen(&fl,name);
	test("seek",fl.setPositionRelativeToBeginning(position)==position);
	test("read 8192",fl.read(buf,sizeof(buf))==0);
	test("position",fl.getPosition()==position);
	test("block is not mapped",!fl.isCurrentBlockMmapBuffered());
	bool	closed=fl.close();
	fakeblkfd=-1;
	test("close",closed);
}

// seek and read 100 bytes, in a block that may be mapped
static void blockDeviceReadSome(const char *data, off64_t position,
							bool mapped) {
	file	fl;
	char	buf[100];
	blockDeviceOpen(&fl,mmapfile);
	test("seek",fl.setPositionRelativeToBeginning(position)==position);
	test("read 100",fl.read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("data",!bytestring::compare(buf,data+position,sizeof(buf)));
	test("position",fl.getPosition()==position+(off64_t)sizeof(buf));
	test((mapped)?"block is mapped":"block is not mapped",
				fl.isCurrentBlockMmapBuffered()==mapped);
	bool	closed=fl.close();
	fakeblkfd=-1;
	test("close",closed);
}

// memory map buffering on a block device, for #11200 filedescriptor - with
// memory map buffering on, a read after a seek past the end of a raw block
// device can crash with SIGBUS
static void blockDeviceTests() {

	signalmanager::alarm(60);

	if (!memorymap::isSupported()) {
		stdoutput.printf("memory maps not supported\n");
		signalmanager::alarm(0);
		return;
	}

	// a file of two and a half 8k blocks
	char	data[20480];
	blockDeviceCreate(mmapfile,data,sizeof(data));

	// skip if the fake block device doesn't work
	file	sfl;
	test("fake check, open",sfl.open(mmapfile,O_RDWR));
	fakeblkfd=sfl.getFileDescriptor();
	uint64_t	devsize=0;
	bool		fake=(sfl.getCurrentProperties() &&
				sfl.isBlockDevice() &&
				sfl.ioCtl(BLKGETSIZE64,&devsize)==0 &&
				devsize==sizeof(data));
	bool		closed=sfl.close();
	fakeblkfd=-1;
	test("fake check, close",closed);
	if (!fake) {
		stdoutput.printf("block device can not be faked\n");
		file::remove(mmapfile);
		signalmanager::alarm(0);
		return;
	}

	// a block wholly past the end of the device
	stdoutput.printf("seek 30000, read:\n");
	blockDeviceReadNone(mmapfile,30000);

	// a block wholly past the end, when the size is a multiple of the
	// block size
	stdoutput.printf("seek 16484 in a file of 16384, read:\n");
	char	smalldata[16384];
	blockDeviceCreate(mmapfile2,smalldata,sizeof(smalldata));
	blockDeviceReadNone(mmapfile2,16484);
	file::remove(mmapfile2);

	// The last block is partly past the end.  It gets a usual buffer and
	// has the file's bytes up to the end, none from 22000.
	stdoutput.printf("seek 22000, read:\n");
	blockDeviceReadNone(mmapfile,22000);

	// blocks wholly inside the device
	stdoutput.printf("seek 8192, read 100:\n");
	blockDeviceReadSome(data,8192,true);
	stdoutput.printf("seek 0, read 100:\n");
	blockDeviceReadSome(data,0,true);

	// a write in a block past the end goes into the buffer, and the flush
	// fails like it does on a device
	stdoutput.printf("seek 30000, read, write 10:\n");
	file	wfl;
	char	buf[8192];
	blockDeviceOpen(&wfl,mmapfile);
	test("seek",wfl.setPositionRelativeToBeginning(30000)==30000);
	test("read 8192",wfl.read(buf,sizeof(buf))==0);
	test("write 10",wfl.write("0123456789",10)==10);
	test("flush fails",!wfl.flushWriteBuffer(-1,-1));
	test("close fails",!wfl.close());
	fakeblkfd=-1;

	// the file is as it was
	char	*contents=new char[sizeof(data)+1];
	file	vfl;
	test("verify, open",vfl.open(mmapfile,O_RDONLY));
	test("verify, size",vfl.getSize()==(off64_t)sizeof(data));
	test("verify, read",
		vfl.read(contents,sizeof(data))==(ssize_t)sizeof(data));
	test("verify, data",!bytestring::compare(contents,data,sizeof(data)));
	test("verify, close",vfl.close());
	delete[] contents;

	// a device that does not report its size gets no map
	stdoutput.printf("no size, seek 8192, read 100:\n");
	fakeblkfail=true;
	blockDeviceReadSome(data,8192,false);
	fakeblkfail=false;

	file::remove(mmapfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// open the file with buffering on, a map or not, and fake a block device on it
static void blockDeviceEndOpen(file *fl, bool usemmap) {
	test("open",fl->open(mmapfile,O_RDWR));
	fl->setMmapBufferingEnabled(usemmap);
	test("set read buffer size",fl->setReadBufferSize(8192));
	fakeblkfd=fl->getFileDescriptor();
}

// check that the file holds the pattern with "len" bytes of "bytes" at "pos"
static void blockDeviceEndVerify(const char *data, size_t size,
					const char *bytes, size_t len,
					off64_t pos) {
	char	*expected=new char[size];
	bytestring::copy(expected,data,size);
	bytestring::copy(expected+pos,bytes,len);
	char	*contents=new char[size];
	file	vfl;
	test("verify, open",vfl.open(mmapfile,O_RDONLY));
	test("verify, size",vfl.getSize()==(off64_t)size);
	test("verify, read",vfl.read(contents,size)==(ssize_t)size);
	test("verify, data",!bytestring::compare(contents,expected,size));
	test("verify, close",vfl.close());
	delete[] contents;
	delete[] expected;
}

// seek from the end, write, and check where the write lands, then refuse a
// seek from the end on a device of no size and after a failed fstat()
static void blockDeviceEndRun(bool usemmap, off64_t unbufend,
					off64_t unbufback) {

	char	data[20480];
	off64_t	size=sizeof(data);

	stdoutput.printf("%s, seek from end, write 10:\n",
				(usemmap)?"map buffering":"buffering");
	blockDeviceCreate(mmapfile,data,sizeof(data));
	file	fl;
	blockDeviceEndOpen(&fl,usemmap);
	off64_t	end=fl.setPositionRelativeToEnd(0);
	test("seek end",end==size);
	test("same as unbuffered",end==unbufend);
	test("position",fl.getPosition()==size);
	off64_t	back=fl.setPositionRelativeToEnd(-100);
	test("seek end-100",back==size-100);
	test("same as unbuffered",back==unbufback);
	test("position",fl.getPosition()==size-100);
	test("write 10",fl.write("0123456789",10)==10);
	bool	closed=fl.close();
	fakeblkfd=-1;
	test("close",closed);
	blockDeviceEndVerify(data,sizeof(data),"0123456789",10,size-100);

	stdoutput.printf("%s, no size, seek from end, write 10:\n",
				(usemmap)?"map buffering":"buffering");
	blockDeviceCreate(mmapfile,data,sizeof(data));
	fakeblkfail=true;
	file	nfl;
	blockDeviceEndOpen(&nfl,usemmap);
	test("seek 50",nfl.setPositionRelativeToBeginning(50)==50);
	test("seek end-100 fails",nfl.setPositionRelativeToEnd(-100)==-1);
	test("errno",error::getErrorNumber()==EINVAL);
	test("position",nfl.getPosition()==50);
	test("write 10",nfl.write("0123456789",10)==10);
	closed=nfl.close();
	fakeblkfd=-1;
	fakeblkfail=false;
	test("close",closed);
	blockDeviceEndVerify(data,sizeof(data),"0123456789",10,50);

	stdoutput.printf("%s, failed fstat, seek from end:\n",
				(usemmap)?"map buffering":"buffering");
	file	sfl;
	blockDeviceEndOpen(&sfl,usemmap);
	test("seek 50",sfl.setPositionRelativeToBeginning(50)==50);
	failstatfd=sfl.getFileDescriptor();
	test("seek end-100 fails",sfl.setPositionRelativeToEnd(-100)==-1);
	test("errno",error::getErrorNumber()==EIO);
	failstatfd=-1;
	test("position",sfl.getPosition()==50);
	closed=sfl.close();
	fakeblkfd=-1;
	test("close",closed);
}

// a buffered seek from the end of a block device, for #11209 filedescriptor -
// with buffering on, a seek from the end of a raw block device uses a size of 0
static void blockDeviceEndTests() {

	signalmanager::alarm(60);

	// a file of two and a half 8k blocks
	char	data[20480];
	blockDeviceCreate(mmapfile,data,sizeof(data));

	// skip if the fake block device doesn't work
	file	sfl;
	test("fake check, open",sfl.open(mmapfile,O_RDWR));
	fakeblkfd=sfl.getFileDescriptor();
	uint64_t	devsize=0;
	bool		fake=(sfl.getCurrentProperties() &&
				sfl.isBlockDevice() &&
				sfl.ioCtl(BLKGETSIZE64,&devsize)==0 &&
				devsize==sizeof(data));

	// with buffering off, the system seeks from the end
	off64_t		unbufend=-1;
	off64_t		unbufback=-1;
	if (fake) {
		unbufend=sfl.setPositionRelativeToEnd(0);
		unbufback=sfl.setPositionRelativeToEnd(-100);
	}
	bool		closed=sfl.close();
	fakeblkfd=-1;
	test("fake check, close",closed);
	if (!fake) {
		stdoutput.printf("block device can not be faked\n");
		file::remove(mmapfile);
		signalmanager::alarm(0);
		return;
	}
	stdoutput.printf("no buffering, seek from end:\n");
	test("seek end",unbufend==(off64_t)sizeof(data));
	test("seek end-100",unbufback==(off64_t)sizeof(data)-100);

	if (memorymap::isSupported()) {
		blockDeviceEndRun(true,unbufend,unbufback);
	} else {
		stdoutput.printf("memory maps not supported\n");
	}
	blockDeviceEndRun(false,unbufend,unbufback);

	file::remove(mmapfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}
#endif

// fill the data with letters and create a file that holds it
static void unsetBufferingCreate(char *data, size_t size) {
	for (size_t i=0; i<size; i++) {
		data[i]='a'+i%26;
	}
	file::remove(mmapfile);
	file	fl;
	test("create",fl.create(mmapfile,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,size)==(ssize_t)size);
	test("close",fl.close());
}

// turn buffering off, write at the position that was set, and check that
// only the written bytes changed in the file
static void unsetBufferingWrite(file *fl, char *data, size_t size,
							off64_t position) {
	test("unset buffering",fl->setReadBufferSize(0));
	test("position",fl->getPosition()==position);
	test("write",fl->write("XYZ",3)==3);
	test("position after write",fl->getPosition()==position+3);
	test("close",fl->close());
	bytestring::copy(data+position,"XYZ",3);
	char	*contents=new char[size+1];
	file	vfl;
	test("verify, open",vfl.open(mmapfile,O_RDONLY));
	test("size",vfl.getSize()==(off64_t)size);
	test("verify, read",vfl.read(contents,size)==(ssize_t)size);
	test("written bytes",!bytestring::compare(contents+position,"XYZ",3));
	test("data",!bytestring::compare(contents,data,size));
	test("verify, close",vfl.close());
	delete[] contents;
	file::remove(mmapfile);
}

#ifdef FILETEST_LSEEK_HOOK
// failed seek to a new block, then turn buffering off and write, for #11201
// filedescriptor - after a failed seek to a new block, the buffer keeps the
// old bytes, so a later write and flush save them into the new block
static void failedSeekUnsetTests() {

	signalmanager::alarm(60);

	char	data[24576];
	unsetBufferingCreate(data,sizeof(data));

	// skip if the lseek() above does not take the place of the real one
	file	sfl;
	test("seek check, open",sfl.open(mmapfile,O_RDONLY));
	failseekfd=sfl.getFileDescriptor();
	bool	failed=(sfl.getPosition()==-1);
	failseekfd=-1;
	test("seek check, close",sfl.close());
	if (!failed) {
		stdoutput.printf("lseek() can not be replaced\n");
		file::remove(mmapfile);
		signalmanager::alarm(0);
		return;
	}

	// read the first block, then fail the seek to the second block
	file	fl;
	test("open",fl.open(mmapfile,O_RDWR));
	fl.setMmapBufferingEnabled(false);
	test("set read buffer size",fl.setReadBufferSize(8192));
	char	buf[100];
	test("read 100",fl.read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("seek",fl.setPositionRelativeToBeginning(8292)==8292);
	failseekfd=fl.getFileDescriptor();
	test("read, failed seek",fl.read(buf,sizeof(buf))==-1);
	failseekfd=-1;

	unsetBufferingWrite(&fl,data,sizeof(data),8292);

	signalmanager::alarm(0);
}
#endif

// turn buffering on, seek, then turn buffering off before any read or write,
// for #11207 filedescriptor - turning buffering off while no block is loaded
// skips the seek to the recorded position, so the next write goes to the
// wrong place
static void unsetBufferingTests() {

	signalmanager::alarm(60);

	char	data[24576];
	unsetBufferingCreate(data,sizeof(data));

	file	fl;
	test("open",fl.open(mmapfile,O_RDWR));
	test("set read buffer size",fl.setReadBufferSize(8192));
	test("seek",fl.setPositionRelativeToBeginning(8292)==8292);
	unsetBufferingWrite(&fl,data,sizeof(data),8292);

	signalmanager::alarm(0);
}

// turn buffering on, read, resize the buffer, then turn buffering off, with
// memory map buffering off and on
static void unsetBufferingResizeTests() {

	signalmanager::alarm(60);

	char	data[24576];
	for (uint8_t usemmap=0; usemmap<2; usemmap++) {
		stdoutput.printf("memory map buffering %s:\n",
						(usemmap)?"on":"off");
		unsetBufferingCreate(data,sizeof(data));
		file	fl;
		test("open",fl.open(mmapfile,O_RDWR));
		fl.setMmapBufferingEnabled(usemmap);
		test("set read buffer size",fl.setReadBufferSize(8192));
		char	buf[100];
		test("read 100",fl.read(buf,sizeof(buf))==
						(ssize_t)sizeof(buf));
		test("resize",fl.setReadBufferSize(4096));
		unsetBufferingWrite(&fl,data,sizeof(data),100);
	}

	signalmanager::alarm(0);
}

// turn buffering on for a regular file and close it with no read or write,
// then open a fifo, which can not seek, and turn buffering off
static void unsetBufferingFifoTests() {

	signalmanager::alarm(60);

	#ifndef WIN32
	const char	*fifofile="/tmp/testfilefifo";
	#else
	const char	*fifofile="testfilefifo";
	#endif
	char		data[100];
	unsetBufferingCreate(data,sizeof(data));
	file::remove(fifofile);
	if (!file::createFifo(fifofile,
			permissions::parsePermString("rw-rw----"))) {
		stdoutput.printf("fifos not supported\n");
		file::remove(mmapfile);
		signalmanager::alarm(0);
		return;
	}

	file	fl;
	test("open",fl.open(mmapfile,O_RDWR));
	test("set read buffer size",fl.setReadBufferSize(8192));
	test("seek",fl.setPositionRelativeToBeginning(50)==50);
	test("close",fl.close());
	test("open fifo",fl.open(fifofile,O_RDWR));
	test("buffered write fails",fl.write("XYZ",3)==-1);
	test("unset buffering",fl.setReadBufferSize(0));
	test("write",fl.write("XYZ",3)==3);
	char	buf[4];
	test("read",fl.read(buf,3)==3);
	test("data",!bytestring::compare(buf,"XYZ",3));
	test("close",fl.close());

	file::remove(fifofile);
	file::remove(mmapfile);

	signalmanager::alarm(0);
}

// write with buffering on, then turn buffering off or resize the buffer, with
// memory map buffering off and on, for #11208 filedescriptor - turning
// buffering off throws away buffered writes that no flush saved yet
static void unsetBufferingFlushTests() {

	signalmanager::alarm(60);

	char	data[24576];
	for (uint8_t usemmap=0; usemmap<2; usemmap++) {
		stdoutput.printf("memory map buffering %s:\n",
						(usemmap)?"on":"off");

		// turn buffering off
		unsetBufferingCreate(data,sizeof(data));
		file	fl;
		test("open",fl.open(mmapfile,O_RDWR));
		fl.setMmapBufferingEnabled(usemmap);
		test("set read buffer size",fl.setReadBufferSize(8192));
		test("seek",fl.setPositionRelativeToBeginning(200)==200);
		test("buffered write",fl.write("ABC",3)==3);
		bytestring::copy(data+200,"ABC",3);
		unsetBufferingWrite(&fl,data,sizeof(data),203);

		// resize the buffer, then turn buffering off
		unsetBufferingCreate(data,sizeof(data));
		test("open",fl.open(mmapfile,O_RDWR));
		fl.setMmapBufferingEnabled(usemmap);
		test("set read buffer size",fl.setReadBufferSize(8192));
		test("seek",fl.setPositionRelativeToBeginning(200)==200);
		test("buffered write",fl.write("ABC",3)==3);
		bytestring::copy(data+200,"ABC",3);
		test("resize",fl.setReadBufferSize(4096));
		unsetBufferingWrite(&fl,data,sizeof(data),203);
	}

	signalmanager::alarm(0);
}

#ifdef FILETEST_LSEEK_HOOK
// fail the flush when buffering is turned off or the buffer is resized, then
// retry, for #11208 filedescriptor - turning buffering off throws away
// buffered writes that no flush saved yet
static void unsetBufferingFlushFailedTests() {

	signalmanager::alarm(60);

	char	data[24576];
	unsetBufferingCreate(data,sizeof(data));

	// skip if the lseek() above does not take the place of the real one
	file	sfl;
	test("seek check, open",sfl.open(mmapfile,O_RDONLY));
	failseekfd=sfl.getFileDescriptor();
	bool	failed=(sfl.getPosition()==-1);
	failseekfd=-1;
	test("seek check, close",sfl.close());
	if (!failed) {
		stdoutput.printf("lseek() can not be replaced\n");
		file::remove(mmapfile);
		signalmanager::alarm(0);
		return;
	}

	// fail to turn buffering off, then retry
	file	fl;
	test("open",fl.open(mmapfile,O_RDWR));
	fl.setMmapBufferingEnabled(false);
	test("set read buffer size",fl.setReadBufferSize(8192));
	test("seek",fl.setPositionRelativeToBeginning(200)==200);
	test("buffered write",fl.write("ABC",3)==3);
	failseekfd=fl.getFileDescriptor();
	bool	unset=fl.setReadBufferSize(0);
	failseekfd=-1;
	test("unset fails",!unset);
	test("read buffer size kept",fl.getReadBufferSize()==8192);
	test("write buffer size kept",fl.getWriteBufferSize()==8192);
	bytestring::copy(data+200,"ABC",3);
	unsetBufferingWrite(&fl,data,sizeof(data),203);

	// fail to resize, then retry
	unsetBufferingCreate(data,sizeof(data));
	test("open",fl.open(mmapfile,O_RDWR));
	fl.setMmapBufferingEnabled(false);
	test("set read buffer size",fl.setReadBufferSize(8192));
	test("seek",fl.setPositionRelativeToBeginning(200)==200);
	test("buffered write",fl.write("ABC",3)==3);
	failseekfd=fl.getFileDescriptor();
	bool	resized=fl.setReadBufferSize(4096);
	failseekfd=-1;
	test("resize fails",!resized);
	test("read buffer size kept",fl.getReadBufferSize()==8192);
	test("retry resize",fl.setReadBufferSize(4096));
	test("read buffer size",fl.getReadBufferSize()==4096);
	bytestring::copy(data+200,"ABC",3);
	unsetBufferingWrite(&fl,data,sizeof(data),203);

	signalmanager::alarm(0);
}
#endif

// a failed or short send on a stream, then a retry of the resize
static void unsetBufferingStreamFailedTest(const char *label, ssize_t mode,
							size_t newsize) {
	stringbuffer	title;
	filedescriptor	readfd;
	failfd		writefd(0);
	title.append(label);
	size_t	titlelength=title.getStringLength();
	title.append(", create pipe");
	test(title.getString(),filedescriptor::createPipe(&readfd,&writefd));
	title.truncate(titlelength);
	title.append(", set buffer size");
	test(title.getString(),writefd.setWriteBufferSize(64));
	title.truncate(titlelength);
	title.append(", write");
	test(title.getString(),writefd.write("abc")==3);

	// fail the send
	writefd.setMode(mode);
	bool	result=writefd.setWriteBufferSize(newsize);
	writefd.setMode(0);
	title.truncate(titlelength);
	title.append(", resize fails");
	test(title.getString(),!result);
	title.truncate(titlelength);
	title.append(", buffer size kept");
	test(title.getString(),writefd.getWriteBufferSize()==64);

	// retry
	title.truncate(titlelength);
	title.append(", retry resize");
	test(title.getString(),writefd.setWriteBufferSize(newsize));
	title.truncate(titlelength);
	title.append(", buffer size");
	test(title.getString(),writefd.getWriteBufferSize()==newsize);
	title.truncate(titlelength);
	title.append(", pipe holds abc");
	test(title.getString(),pipeHolds(&readfd,&writefd,"abc"));
}

// stream write buffer turned off or resized with data in it
static void unsetBufferingStreamFlushTests() {

	signalmanager::alarm(60);

	// resize or turn off, then write more
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("stream off, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream off, set buffer size",
					writefd.setWriteBufferSize(64));
		test("stream off, write",writefd.write("abc")==3);
		test("stream off, turn off",writefd.setWriteBufferSize(0));
		test("stream off, write d",writefd.write("d")==1);
		test("stream off, pipe holds abcd",
				pipeHolds(&readfd,&writefd,"abcd"));
	}
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("stream smaller, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream smaller, set buffer size",
					writefd.setWriteBufferSize(64));
		test("stream smaller, write",writefd.write("abc")==3);
		test("stream smaller, resize",writefd.setWriteBufferSize(32));
		test("stream smaller, write d",writefd.write("d")==1);
		test("stream smaller, flush",writefd.flushWriteBuffer(-1,-1));
		test("stream smaller, pipe holds abcd",
				pipeHolds(&readfd,&writefd,"abcd"));
	}
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("stream same, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream same, set buffer size",
					writefd.setWriteBufferSize(64));
		test("stream same, write",writefd.write("abc")==3);
		test("stream same, resize",writefd.setWriteBufferSize(64));
		test("stream same, write d",writefd.write("d")==1);
		test("stream same, flush",writefd.flushWriteBuffer(-1,-1));
		test("stream same, pipe holds abcd",
				pipeHolds(&readfd,&writefd,"abcd"));
	}

	// failed write, then retry
	unsetBufferingStreamFailedTest("stream failed off",-1,0);
	unsetBufferingStreamFailedTest("stream failed smaller",-1,32);

	// short write, then retry
	unsetBufferingStreamFailedTest("stream short off",1,0);
	unsetBufferingStreamFailedTest("stream short smaller",1,32);

	// full nonblocking pipe
	#ifndef _WIN32
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("stream full, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream full, fill pipe",fillPipe(&writefd));
		test("stream full, set buffer size",
					writefd.setWriteBufferSize(64));
		test("stream full, write",writefd.write("abc")==3);

		// fillPipe leaves the pipe blocking
		test("stream full, nonblocking",
					writefd.setNonBlockingMode(true));
		test("stream full, turn off fails",
					!writefd.setWriteBufferSize(0));
		test("stream full, buffer size kept",
					writefd.getWriteBufferSize()==64);
		test("stream full, resize fails",
					!writefd.setWriteBufferSize(32));
		test("stream full, buffer size kept again",
					writefd.getWriteBufferSize()==64);
	}
	#endif

	signalmanager::alarm(0);
}

// read "length" bytes, looping over short reads, and compare them
static bool readExact(filedescriptor *fd, const char *expected,
							size_t length) {
	char	buf[64];
	size_t	total=0;
	while (total<length) {
		ssize_t	result=fd->read(buf+total,length-total);
		if (result<=0) {
			return false;
		}
		total+=result;
	}
	return !bytestring::compare(buf,expected,length);
}

// a pipe with "abcdef" written and "ab" read through a read buffer, so
// that "cdef" is unread in the buffer
static void readBufferStreamUnreadSetup(const char *label,
						filedescriptor *readfd,
						filedescriptor *writefd) {
	stringbuffer	title;
	title.append(label);
	size_t	titlelength=title.getStringLength();
	title.append(", create pipe");
	test(title.getString(),filedescriptor::createPipe(readfd,writefd));
	title.truncate(titlelength);
	title.append(", write unbuffered");
	test(title.getString(),writefd->setWriteBufferSize(0));
	title.truncate(titlelength);
	title.append(", set read buffer size");
	test(title.getString(),readfd->setReadBufferSize(64));
	title.truncate(titlelength);
	title.append(", write");
	test(title.getString(),writefd->write("abcdef")==6);
	title.truncate(titlelength);
	title.append(", read ab");
	test(title.getString(),readExact(readfd,"ab",2));
}

// resize the read buffer with "cdef" unread, then read it
static void readBufferStreamUnreadResizeTest(const char *label,
						size_t newsize,
						bool expectedresult,
						size_t expectedsize) {
	stringbuffer	title;
	filedescriptor	readfd;
	filedescriptor	writefd;
	readBufferStreamUnreadSetup(label,&readfd,&writefd);
	title.append(label);
	size_t	titlelength=title.getStringLength();

	// store results in order
	bool	result=readfd.setReadBufferSize(newsize);
	size_t	size=readfd.getReadBufferSize();
	title.append(", resize result");
	test(title.getString(),result==expectedresult);
	title.truncate(titlelength);
	title.append(", read buffer size");
	test(title.getString(),size==expectedsize);
	title.truncate(titlelength);
	title.append(", read cdef");
	test(title.getString(),readExact(&readfd,"cdef",4));
}

// stream read buffer turned off or resized with unread bytes in it
static void readBufferStreamUnreadTests() {

	signalmanager::alarm(60);

	// turn off or shrink below the unread bytes, refused
	readBufferStreamUnreadResizeTest("stream read off",0,false,64);
	readBufferStreamUnreadResizeTest("stream read too small",2,false,64);

	// sizes that hold the unread bytes
	readBufferStreamUnreadResizeTest("stream read smaller",32,true,32);
	readBufferStreamUnreadResizeTest("stream read exact",4,true,4);
	readBufferStreamUnreadResizeTest("stream read larger",128,true,128);
	readBufferStreamUnreadResizeTest("stream read same",64,true,64);

	// turn off after the unread bytes are read, then read unbuffered
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		readBufferStreamUnreadSetup("stream read drained",
							&readfd,&writefd);
		test("stream read drained, read cdef",
					readExact(&readfd,"cdef",4));
		bool	result=readfd.setReadBufferSize(0);
		size_t	size=readfd.getReadBufferSize();
		test("stream read drained, turn off",result);
		test("stream read drained, read buffer size",size==0);
		test("stream read drained, write xy",
					writefd.write("xy")==2);
		test("stream read drained, read xy",
					readExact(&readfd,"xy",2));
	}

	// no unread bytes at all
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("stream read empty, create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("stream read empty, write unbuffered",
					writefd.setWriteBufferSize(0));
		test("stream read empty, set read buffer size",
					readfd.setReadBufferSize(64));
		bool	result=readfd.setReadBufferSize(32);
		size_t	size=readfd.getReadBufferSize();
		test("stream read empty, resize",result);
		test("stream read empty, read buffer size",size==32);
		result=readfd.setReadBufferSize(0);
		size=readfd.getReadBufferSize();
		test("stream read empty, turn off",result);
		test("stream read empty, read buffer size off",size==0);
		test("stream read empty, write xy",writefd.write("xy")==2);
		test("stream read empty, read xy",
					readExact(&readfd,"xy",2));
	}

	signalmanager::alarm(0);
}

// the bytes that the close tests write
static void closeFlushData(char *data, size_t length) {
	for (size_t i=0; i<length; i++) {
		data[i]=(char)((i*7+3)%251);
	}
}

// open a new file with a write buffer
static bool closeFlushOpen(file *fl, bool mmap, size_t buffersize) {
	file::remove(mmapfile);
	fl->setMmapBufferingEnabled(mmap);
	return (fl->create(mmapfile,
			permissions::parsePermString("rw-rw----")) &&
		fl->setWriteBufferSize(buffersize));
}

// the file holds exactly the data
static bool closeFlushHolds(const char *data, size_t length) {
	file	vfl;
	if (!vfl.open(mmapfile,O_RDONLY)) {
		return false;
	}
	bool	ok=(vfl.getSize()==(off64_t)length);
	char	*contents=new char[length+1];
	ok=(ok && vfl.read(contents,length)==(ssize_t)length &&
		!bytestring::compare(contents,data,length));
	delete[] contents;
	return (vfl.close() && ok);
}

#ifdef FILETEST_LSEEK_HOOK
// a seek fails on the file descriptor of an open file
static bool closeFlushSeekFails() {
	file	cfl;
	if (!closeFlushOpen(&cfl,false,7) || !cfl.close()) {
		return false;
	}
	file	fl;
	if (!fl.open(mmapfile,O_RDONLY)) {
		return false;
	}
	failseekfd=fl.getFileDescriptor();
	bool	failed=(fl.getPosition()==-1);
	failseekfd=-1;
	return (fl.close() && failed);
}

// close() and delete with a failed flush of the write buffer
static void closeFlushFailedTests() {

	char	data[33];
	closeFlushData(data,sizeof(data));

	// skip if the lseek() above does not take the place of the real one
	bool	seekfails=closeFlushSeekFails();
	file::remove(mmapfile);
	if (!seekfails) {
		stdoutput.printf("lseek() can not be replaced\n");
		return;
	}

	// close() with a failed flush
	stdoutput.printf("close, failed flush:\n");
	file	fl;
	test("create",closeFlushOpen(&fl,false,7));
	test("write",fl.write(data,sizeof(data))==(ssize_t)sizeof(data));
	failseekfd=fl.getFileDescriptor();
	error::clearError();
	bool	closed=fl.close();
	int32_t	errnum=error::getErrorNumber();
	failseekfd=-1;
	test("close fails",!closed);
	test("error number is set after close",errnum!=0);
	test("descriptor is closed",fl.getFileDescriptor()==-1);
	fl.close();
	test("close again",true);
	file::remove(mmapfile);

	// delete with a failed flush
	stdoutput.printf("delete, failed flush:\n");
	file	*dfl=new file();
	test("create",closeFlushOpen(dfl,false,7));
	test("write",dfl->write(data,sizeof(data))==(ssize_t)sizeof(data));
	int32_t	fd=dfl->getFileDescriptor();
	failseekfd=fd;
	delete dfl;
	failseekfd=-1;
	test("delete",true);
	error::clearError();
	test("descriptor is closed",fcntl(fd,F_GETFD)==-1 &&
					error::getErrorNumber()==EBADF);
	file::remove(mmapfile);
}

// open() and create() with a failed flush on close() of the previous file,
// for #11203 file - open() and create() ignore a failed close(), so a failed
// flush of the previous file's write buffer is not reported
static void openFailedCloseTests() {

	signalmanager::alarm(60);

	char	data[33];
	closeFlushData(data,sizeof(data));
	mode_t	perms=permissions::parsePermString("rw-rw----");

	// skip if the lseek() above does not take the place of the real one
	bool	seekfails=closeFlushSeekFails();
	file::remove(mmapfile);
	if (!seekfails) {
		stdoutput.printf("lseek() can not be replaced\n");
		signalmanager::alarm(0);
		return;
	}

	// open() of an existing file, open() with O_CREAT, then create()
	const char	*titles[]={"open","open with permissions","create"};
	for (uint16_t i=0; i<3; i++) {

		stdoutput.printf("%s, failed flush:\n",titles[i]);
		file::remove(mmapfile2);
		file::remove(openfile);
		if (!i) {
			test("create other",file::createFile(mmapfile2,perms));
		}

		// fill the write buffer of the previous file
		file	fl;
		test("create",closeFlushOpen(&fl,false,7));
		test("write",
			fl.write(data,sizeof(data))==(ssize_t)sizeof(data));

		// open the new file with a failed flush of the previous one
		failseekfd=fl.getFileDescriptor();
		error::clearError();
		bool	opened;
		if (i==0) {
			opened=fl.open(mmapfile2,O_RDWR);
		} else if (i==1) {
			opened=fl.open(openfile,O_RDWR|O_CREAT,perms);
		} else {
			opened=fl.create(openfile,perms);
		}
		int32_t	errnum=error::getErrorNumber();
		failseekfd=-1;
		test("fails",!opened);
		test("error number of the flush",errnum==EINVAL);
		test("new file is not open",fl.getFileDescriptor()==-1);
		if (i) {
			test("new file is not created",
						!file::exists(openfile));
		}

		// the file object still works
		test("close",fl.close());
		test("open again",fl.open(mmapfile,O_RDWR));
		test("close again",fl.close());
	}
	file::remove(mmapfile);
	file::remove(mmapfile2);
	file::remove(openfile);

	signalmanager::alarm(0);
}
#endif

// close() and delete of a file with data in the write buffer, for #11188
// filedescriptor - close() discards data still in the write buffer
static void closeFlushTests() {

	signalmanager::alarm(60);

	// buffer sizes and lengths, usual buffering then mmap buffering
	char	data[20000];
	closeFlushData(data,sizeof(data));
	bool	mmaps[]={false,true};
	size_t	sizes[]={7,8192};
	size_t	lengths[]={33,20000};

	for (uint16_t i=0; i<2; i++) {

		if (mmaps[i] && !memorymap::isSupported()) {
			stdoutput.printf("memory maps not supported\n");
			continue;
		}

		stringbuffer	title;
		title.append((mmaps[i])?"mmap":"usual");
		title.append(", ");
		const char	*t=title.getString();
		stringbuffer	msg;

		// close() with no flushWriteBuffer()
		stdoutput.printf("%sclose:\n",t);
		file	fl;
		msg.clear();
		msg.append(t)->append("create");
		test(msg.getString(),closeFlushOpen(&fl,mmaps[i],sizes[i]));
		msg.clear();
		msg.append(t)->append("write");
		test(msg.getString(),
			fl.write(data,lengths[i])==(ssize_t)lengths[i]);
		msg.clear();
		msg.append(t)->append("close");
		test(msg.getString(),fl.close());
		msg.clear();
		msg.append(t)->append("size and data");
		test(msg.getString(),closeFlushHolds(data,lengths[i]));

		// delete with no close()
		stdoutput.printf("%sdelete:\n",t);
		file	*dfl=new file();
		msg.clear();
		msg.append(t)->append("create");
		test(msg.getString(),closeFlushOpen(dfl,mmaps[i],sizes[i]));
		msg.clear();
		msg.append(t)->append("write");
		test(msg.getString(),
			dfl->write(data,lengths[i])==(ssize_t)lengths[i]);
		delete dfl;
		msg.clear();
		msg.append(t)->append("size and data");
		test(msg.getString(),closeFlushHolds(data,lengths[i]));
	}
	file::remove(mmapfile);

	// close() of a stream does not write the buffer
	stdoutput.printf("stream close:\n");
	{
		filedescriptor	readfd;
		filedescriptor	writefd;
		test("create pipe",
			filedescriptor::createPipe(&readfd,&writefd));
		test("set write buffer size",writefd.setWriteBufferSize(64));
		test("write",writefd.write("abc")==3);
		test("close",writefd.close());
		char	buf[10];
		test("reader sees end of data with no data",
					readfd.read(buf,sizeof(buf))==0);
	}

	#ifdef FILETEST_LSEEK_HOOK
	closeFlushFailedTests();
	#endif

	file::remove(mmapfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// buffered read and write from the middle of a mapped block, for #11191
// filedescriptor - a storage read or write that starts in the middle of a
// memory mapped block runs past the end of the map
static void mmapMidBlockTests() {

	signalmanager::alarm(60);

	if (!memorymap::isSupported()) {
		stdoutput.printf("memory maps not supported\n");
		signalmanager::alarm(0);
		return;
	}

	// create a file of three 8k blocks
	char	data[24576];
	for (uint16_t i=0; i<sizeof(data); i++) {
		data[i]=(char)(i%251);
	}
	file::remove(mmapfile);
	file	fl;
	test("create",fl.create(mmapfile,
				permissions::parsePermString("rw-rw----")));
	test("write",fl.write(data,sizeof(data))==(ssize_t)sizeof(data));
	test("close",fl.close());

	// read a block's worth of data from the middle of a block
	file	rfl;
	test("read, open",rfl.open(mmapfile,O_RDWR));
	rfl.setMmapBufferingEnabled(true);
	test("read, set read buffer size",rfl.setReadBufferSize(8192));
	test("read, seek",rfl.setPositionRelativeToBeginning(5000)==5000);
	char	buf[8192];
	test("read, read 8192",rfl.read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("read, data",!bytestring::compare(buf,data+5000,sizeof(buf)));
	test("read, position",rfl.getPosition()==13192);
	test("read, block is mapped",rfl.isCurrentBlockMmapBuffered());
	test("read, close",rfl.close());

	// write a block's worth of data from the middle of a block
	char	wdata[8192];
	for (uint16_t i=0; i<sizeof(wdata); i++) {
		wdata[i]=~data[5000+i];
	}
	file	wfl;
	test("write, open",wfl.open(mmapfile,O_RDWR));
	wfl.setMmapBufferingEnabled(true);
	test("write, set write buffer size",wfl.setWriteBufferSize(8192));
	test("write, seek",wfl.setPositionRelativeToBeginning(5000)==5000);
	test("write, write 8192",
		wfl.write(wdata,sizeof(wdata))==(ssize_t)sizeof(wdata));
	test("write, position",wfl.getPosition()==13192);
	test("write, block is mapped",wfl.isCurrentBlockMmapBuffered());
	test("write, flush",wfl.flushWriteBuffer(-1,-1));
	test("write, close",wfl.close());

	// the write is in place and the data around it is intact
	char	*contents=new char[sizeof(data)];
	file	vfl;
	test("verify, open",vfl.open(mmapfile,O_RDONLY));
	test("verify, size",vfl.getSize()==(off64_t)sizeof(data));
	test("verify, read file",
		vfl.read(contents,sizeof(data))==(ssize_t)sizeof(data));
	test("verify, before",!bytestring::compare(contents,data,5000));
	test("verify, written",
		!bytestring::compare(contents+5000,wdata,sizeof(wdata)));
	test("verify, after",
		!bytestring::compare(contents+13192,data+13192,
						sizeof(data)-13192));
	test("verify, close",vfl.close());
	delete[] contents;

	file::remove(mmapfile);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// create two files of three 8k blocks with different data
static void mmapCloseReopenCreate(char *adata, char *bdata, size_t size) {

	for (size_t i=0; i<size; i++) {
		adata[i]='a'+i%26;
		bdata[i]='A'+i%26;
	}
	file::remove(mmapfile);
	file::remove(mmapfile2);
	file	fl;
	test("create a",fl.create(mmapfile,
				permissions::parsePermString("rw-rw----")));
	test("write a",fl.write(adata,size)==(ssize_t)size);
	test("close a",fl.close());
	test("create b",fl.create(mmapfile2,
				permissions::parsePermString("rw-rw----")));
	test("write b",fl.write(bdata,size)==(ssize_t)size);
	test("close b",fl.close());
}

// map a block of the first file, close it, and open the second file
static void mmapCloseReopenOpen(file *fl, const char *adata) {

	test("open a",fl->open(mmapfile,O_RDWR));
	fl->setMmapBufferingEnabled(true);
	test("set read buffer size",fl->setReadBufferSize(8192));
	char	buf[100];
	test("read 100",fl->read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("read data",!bytestring::compare(buf,adata,sizeof(buf)));
	test("block is mapped",fl->isCurrentBlockMmapBuffered());
	test("close a, mapped",fl->close());
	test("open b",fl->open(mmapfile2,O_RDWR));
}

// check the contents of the second file with a fresh file object
static void mmapCloseReopenVerify(const char *expected, size_t size) {

	char	*contents=new char[size];
	file	vfl;
	test("verify, open",vfl.open(mmapfile2,O_RDONLY));
	test("verify, size",vfl.getSize()==(off64_t)size);
	test("verify, read file",
		vfl.read(contents,size)==(ssize_t)size);
	test("verify, contents",!bytestring::compare(contents,expected,size));
	test("verify, close",vfl.close());
	delete[] contents;
}

// read and write after a close with a mapped block and a new open, for #11197
// filedescriptor - close() with a mapped block keeps stale available counts,
// so a read or write after a new open() can use the dropped map
static void mmapCloseReopenTests() {

	signalmanager::alarm(60);

	if (!memorymap::isSupported()) {
		stdoutput.printf("memory maps not supported\n");
		signalmanager::alarm(0);
		return;
	}

	char	adata[24576];
	char	bdata[24576];
	char	expected[24576];
	char	wdata[100];
	for (uint16_t i=0; i<sizeof(wdata); i++) {
		wdata[i]='0'+i%10;
	}

	// write from the beginning of the new file
	stdoutput.printf("write at 0\n");
	mmapCloseReopenCreate(adata,bdata,sizeof(bdata));
	file	*fl=new file();
	mmapCloseReopenOpen(fl,adata);
	test("write 100",fl->write(wdata,sizeof(wdata))==(ssize_t)sizeof(wdata));
	test("flush",fl->flushWriteBuffer(-1,-1));
	test("close b",fl->close());
	bytestring::copy(expected,bdata,sizeof(expected));
	bytestring::copy(expected,wdata,sizeof(wdata));
	mmapCloseReopenVerify(expected,sizeof(expected));
	delete fl;

	// write after a seek in the new file
	stdoutput.printf("\nwrite after seek\n");
	mmapCloseReopenCreate(adata,bdata,sizeof(bdata));
	fl=new file();
	mmapCloseReopenOpen(fl,adata);
	test("seek",fl->setPositionRelativeToBeginning(50)==50);
	test("write 10",fl->write(wdata,10)==10);
	test("close b",fl->close());
	bytestring::copy(expected,bdata,sizeof(expected));
	bytestring::copy(expected+50,wdata,10);
	mmapCloseReopenVerify(expected,sizeof(expected));
	delete fl;

	// read from the beginning of the new file
	stdoutput.printf("\nread at 0\n");
	mmapCloseReopenCreate(adata,bdata,sizeof(bdata));
	fl=new file();
	mmapCloseReopenOpen(fl,adata);
	char	buf[100];
	test("read 100",fl->read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("read data",!bytestring::compare(buf,bdata,sizeof(buf)));
	test("close b",fl->close());
	delete fl;

	file::remove(mmapfile);
	file::remove(mmapfile2);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

// buffer a block of the first file without a map, close it, and open the
// second file
static void bufferCloseReopenOpen(file *fl, const char *adata) {

	test("open a",fl->open(mmapfile,O_RDWR));
	fl->setMmapBufferingEnabled(false);
	test("set read buffer size",fl->setReadBufferSize(8192));
	char	buf[100];
	test("read 100",fl->read(buf,sizeof(buf))==(ssize_t)sizeof(buf));
	test("read data",!bytestring::compare(buf,adata,sizeof(buf)));
	test("block is not mapped",!fl->isCurrentBlockMmapBuffered());
	test("close a, not mapped",fl->close());
	test("open b",fl->open(mmapfile2,O_RDWR));
}

// write after a close without a map and a new open, for #11204
// filedescriptor - with memory map buffering off, a write after close() and a
// new open() can save bytes of the old file into the new file
static void bufferCloseReopenTests() {

	signalmanager::alarm(60);

	char	adata[24576];
	char	bdata[24576];
	char	expected[24576];
	char	wdata[10];
	for (uint16_t i=0; i<sizeof(wdata); i++) {
		wdata[i]='0'+i%10;
	}

	// write at 0, seek forward in the block, and write again
	stdoutput.printf("write at 0, then after seek\n");
	mmapCloseReopenCreate(adata,bdata,sizeof(bdata));
	file	*fl=new file();
	bufferCloseReopenOpen(fl,adata);
	test("write 10",fl->write(wdata,sizeof(wdata))==(ssize_t)sizeof(wdata));
	test("seek",fl->setPositionRelativeToBeginning(50)==50);
	test("write 10",fl->write(wdata,sizeof(wdata))==(ssize_t)sizeof(wdata));
	test("close b",fl->close());
	bytestring::copy(expected,bdata,sizeof(expected));
	bytestring::copy(expected,wdata,sizeof(wdata));
	bytestring::copy(expected+50,wdata,sizeof(wdata));
	mmapCloseReopenVerify(expected,sizeof(expected));
	delete fl;

	// seek forward in the block before the first write
	stdoutput.printf("\nwrite after seek\n");
	mmapCloseReopenCreate(adata,bdata,sizeof(bdata));
	fl=new file();
	bufferCloseReopenOpen(fl,adata);
	test("seek",fl->setPositionRelativeToBeginning(50)==50);
	test("write 10",fl->write(wdata,sizeof(wdata))==(ssize_t)sizeof(wdata));
	test("close b",fl->close());
	bytestring::copy(expected,bdata,sizeof(expected));
	bytestring::copy(expected+50,wdata,sizeof(wdata));
	mmapCloseReopenVerify(expected,sizeof(expected));
	delete fl;

	file::remove(mmapfile);
	file::remove(mmapfile2);
	stdoutput.printf("\n");

	signalmanager::alarm(0);
}

int main(int argc, const char **argv) {

	char	*osname=sys::getOperatingSystemName();

	file	fl;

	if (argc==1) {

		header("file");

		file::remove(testfiletxt);

		error::clearError();
		test("create without perms",
				!fl.create(testfiletxt,0));
		test("error",error::getErrorNumber()==EINVAL);
		error::clearError();
		test("create without perms",
				!fl.open(testfiletxt,O_RDWR|O_CREAT));
		test("error",error::getErrorNumber()==EINVAL);
		error::clearError();
		test("create without perms",
				!fl.open(testfiletxt,O_RDWR|O_CREAT,0));
		test("error",error::getErrorNumber()==EINVAL);
		test("create",
			fl.open(testfiletxt,O_RDWR|O_CREAT,
				permissions::parsePermString("rw-rw----")));
		fl.close();
		test("create when already exists",
			fl.open(testfiletxt,O_RDWR|O_CREAT,
				permissions::parsePermString("rw-rw----")));
		fl.close();
		test("create with excl",
			!fl.open(testfiletxt,O_RDWR|O_CREAT|O_EXCL,
				permissions::parsePermString("rw-rw----")));
		file::remove(testfiletxt);
		test("create",
			fl.create(testfiletxt,
				permissions::parsePermString("rw-rw----")));
		test("write",fl.write("hello")==5);
		test("get properties",fl.getCurrentProperties());
		test("close",fl.close());
		test("exists",file::exists(testfiletxt));
		test("reopen",fl.open(testfiletxt,O_RDWR));

		uid_t	uid=fl.getOwnerUserId();
		char	*username=userentry::getName(uid);
		test("user",uid==process::getUserId());

		gid_t	gid=fl.getOwnerGroupId();
		// on bsd systems, the file is created with wheel
		// group ownership in some cases
		gid_t	wheelgid=groupentry::getGroupId("wheel");
		test("group",(gid==process::getGroupId() ||
				(wheelgid<65500 && gid==wheelgid)));

		// not supported on syllable
		if (charstring::compare(osname,"syllable")) {
			// not reliable over cifs
			#ifndef WIN32
			char	*groupname=groupentry::getName(gid);
			test("chown",fl.changeOwner(username,groupname));
			delete[] groupname;
			#endif
		}

		delete[] username;

		test("size",fl.getSize()==5);

		test("block size",fl.getBlockSize()>0);

		// not supported on syllable
		if (charstring::compare(osname,"syllable")) {
			test("blocks",fl.getBlockCount()!=0);
		}

		test("is a socket",!fl.isSocket());
		test("is a symlink",!fl.isSymbolicLink());
		test("is a regular file",fl.isRegularFile());
		test("is a block device",!fl.isBlockDevice());
		test("is a directory",!fl.isDirectory());
		test("is a character device",!fl.isCharacterDevice());
		test("is a fifo",!fl.isFifo());

		datetime	dt;
		dt.initFromSystemDateTime();

		// these aren't reliable over nfs/cifs...
		/*datetime	atime;
		atime.init(fl.getLastAccessTime());
		test("access time",atime.getEpoch()-dt.getEpoch()<=1);

		datetime	mtime;
		mtime.init(fl.getLastModificationTime());
		test("modification time",mtime.getEpoch()-dt.getEpoch()<=1);

		datetime	ctime;
		mtime.init(fl.getLastChangeTime());
		test("change time",ctime.getEpoch()-dt.getEpoch()<=1);*/

		test("inode",fl.getInode()!=0);
		test("hard links",fl.getNumberOfHardLinks()==1);

		const char	*path=
			"/usr/local/firstworks/include/rudiments/file.h";
		char	*dirname=file::getDirName(path);
		test("dirname",!charstring::compare(dirname,
				"/usr/local/firstworks/include/rudiments"));
		delete[] dirname;

		char	*basename=file::getBaseName(path);
		test("basename",!charstring::compare(basename,"file.h"));
		delete[] basename;

		basename=file::getBaseName(path,".h");
		test("basename again",!charstring::compare(basename,"file"));
		delete[] basename;

		// extensions the base name doesn't have
		basename=file::getBaseName(path,".xml");
		test("basename wrong extension",
				!charstring::compare(basename,"file.h"));
		delete[] basename;

		basename=file::getBaseName(path,".H");
		test("basename extension case",
				!charstring::compare(basename,"file.h"));
		delete[] basename;

		basename=file::getBaseName(path,"file.h.longer");
		test("basename extension longer than base name",
				!charstring::compare(basename,"file.h"));
		delete[] basename;

		// base name that is entirely the extension
		basename=file::getBaseName("/usr/local/.h",".h");
		test("basename extension only",
				!charstring::compare(basename,""));
		delete[] basename;

		// null and empty arguments
		basename=file::getBaseName(NULL,".h");
		test("basename null file name",basename==NULL);
		delete[] basename;

		basename=file::getBaseName(path,NULL);
		test("basename null extension",
				!charstring::compare(basename,"file.h"));
		delete[] basename;

		basename=file::getBaseName(path,"");
		test("basename empty extension",
				!charstring::compare(basename,"file.h"));
		delete[] basename;

		test("key",file::generateKey(testfiletxt,1)!=0);

		char	*pwd=directory::getCurrentWorkingDirectory();
		linkedlist<char *>	patterns;
		patterns.setManageArrayValues(true);
		stringbuffer	fn;
		fn.append(pwd);
		fn.append(sys::getDirectorySeparator());
		// glob on Linux libc doesn't work with **?ile*.cpp
		// but does work with *?ile*.cpp
		#ifndef __GLIBC__
		if (!charstring::compare(osname,"Linux")) {
			fn.append("*?ile*.cpp");
		} else
		#endif
		{
			fn.append("**?ile*.cpp");
		}
		patterns.append(fn.detachString());
		fn.append(pwd);
		fn.append(sys::getDirectorySeparator());
		fn.append("*a?ra**.cpp");
		patterns.append(fn.detachString());
		linkedlist<char *>	matches;
		matches.setManageArrayValues(true);
		test("getMatchingFileNames",
			file::getMatchingFileNames(&patterns,&matches));
		matches.sortQuickly();
		listnode<char *>	*node=matches.getFirst();
		test("getMatchingFileNames: match 1",
			charstring::endsWith(node->getValue(),
						"dynamicarray.cpp"));
		node=node->getNext();
		test("getMatchingFileNames: match 2",
			charstring::endsWith(node->getValue(),
						"file.cpp"));
		node=node->getNext();
		test("getMatchingFileNames: match 3",
			charstring::endsWith(node->getValue(),
						"filesystem.cpp"));
		node=node->getNext();
		test("getMatchingFileNames: match 4",
			charstring::endsWith(node->getValue(),
						"staticarray.cpp"));

		fl.open(testfiletxt,O_RDWR);
		test("lock 1",fl.lockFile(
				#if defined(F_WRLCK)
					F_WRLCK
				#else
					0
				#endif
					));

		stringbuffer	cmd;
		cmd.append(pwd)->append("/file");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;
		const char	*args1[]={"file","child",NULL};
		process::spawn(cmd.getString(),args1,true);

		snooze::macrosnooze(1);

		test("unlock 1",fl.unlockFile());

		snooze::macrosnooze(1);

		test("truncate",fl.truncate());
		test("getCurrentProperties after truncate",
					fl.getCurrentProperties());
		test("getSize after truncate",!fl.getSize());
		filesystem	fs;
		fs.open(testfiletxt);
		ssize_t	blocksize=
			filesystem::getOptimumTransferBlockSize(testfiletxt);
		fl.setWriteBufferSize(blocksize);
		test("getWriteBufferSize",
			fl.getWriteBufferSize()==(size_t)blocksize);
		test("getReadBufferSize",
			fl.getReadBufferSize()==(size_t)blocksize);

		// sequential writes/reads
		// write 400 a's, followed by 400 b's, followed by 400 c's, etc.
		// then
		// read 400 a's, followed by 400 b's, followed by 400 c's, etc.
		//
		// (note that 400 is not aligned with the buffer size which is
		// probably some multiple of 512)
		bool	success=true;
		for (uint8_t iter=0; iter<2; iter++) {
			for (char ch='a'; ch<='z'; ch++) {
				for (uint16_t i=0; i<400; i++) {
					char	rch;
					ssize_t	result=(!iter)?fl.write(ch):
								fl.read(&rch);
					if (result!=sizeof(char)) {
						success=false;
						break;
					}
					if (iter && rch!=ch) {
						success=false;
						break;
					}
				}
				if (!success) {
					break;
				}
			}
			test((!iter)?"sequential buffered writes":
					"sequential buffered reads",success);
			fl.flushWriteBuffer(-1,-1);
			fl.setPositionRelativeToBeginning(0);
		}

		// non-sequential reads
		// read the first a, first b, first c, etc.
		// then
		// read the second a, second b, second c, etc.
		// then
		// read the third a, third b, third c, etc.
		// etc.
		for (uint16_t i=0; i<400; i++) {
			for (char ch='a'; ch<='z'; ch++) {
				fl.setPositionRelativeToBeginning(
							(ch-'a')*400+i);
				char	rch;
				ssize_t	result=fl.read(&rch);
				if (result!=sizeof(char)) {
					success=false;
					break;
				}
				if (rch!=ch) {
					success=false;
					break;
				}
			}
			if (!success) {
				break;
			}
		}
		test("non-sequential buffered reads",success);

		// non-sequential writes
		// replace the zeroth a, zeroth b, zeroth c, etc. with 0
		// then
		// replace the tenth a, tenth b, tenth c, etc. with 0
		// etc.
		for (uint16_t i=0; i<400; i+=10) {
			for (char ch='a'; ch<='z'; ch++) {
				fl.setPositionRelativeToBeginning(
							(ch-'a')*400+i);
				ssize_t	result=fl.write('0');
				if (result!=sizeof(char)) {
					success=false;
					break;
				}
			}
			if (!success) {
				break;
			}
		}
		fl.flushWriteBuffer(-1,-1);
		test("non-sequential buffered writes",success);

		// verify that the above worked
		fl.setPositionRelativeToBeginning(0);
		for (char ch='a'; ch<='z'; ch++) {
			for (uint16_t i=0; i<400; i=i+1) {
				char	rch;
				ssize_t	result=fl.read(&rch);
				if (result!=sizeof(char)) {
					success=false;
					break;
				}
				if (rch!=((i%10)?ch:'0')) {
					success=false;
					break;
				}
			}
			if (!success) {
				break;
			}
		}
		test("sequential buffered reads",success);
		stdoutput.printf("\n");

		fl.close();

		file::remove(testfiletxt);

		stdoutput.printf("\ngetContents\n");
		contentsTests();

		stdoutput.printf("\nprintf errors\n");
		printfErrorTests();

		stdoutput.printf("\nprintf socket layer\n");
		printfSocketLayerTests();

		stdoutput.printf("\nprintf errno\n");
		printfErrnoTests();

		stdoutput.printf("\nflush errors\n");
		flushErrorTests();

		stdoutput.printf("\nwrite errors\n");
		writeErrorTests();

		stdoutput.printf("\npartial writes\n");
		partialWriteTests();

		stdoutput.printf("\npartial reads\n");
		partialReadTests();

		stdoutput.printf("\nbuffered reads after an end of data\n");
		bufferedEofReadTests();

		stdoutput.printf("\nmemory map delete without close\n");
		mmapDeleteTests();

		stdoutput.printf("\nbuffered reads after a seek past the end\n");
		seekPastEndReadTests();

		stdoutput.printf("\nbuffered reads and writes after a truncate\n");
		truncateBufferTests();

		stdoutput.printf("\nbuffered writes after a seek past the end\n");
		seekPastEndWriteTests();

		stdoutput.printf("\nbuffered seeks before the start of the file\n");
		negativeSeekTests();

		stdoutput.printf("\nmemory map reads and writes from the middle of a block\n");
		mmapMidBlockTests();

		stdoutput.printf("\nmemory map close and reopen\n");
		mmapCloseReopenTests();

		stdoutput.printf("\nbuffer close and reopen\n");
		bufferCloseReopenTests();

		#ifdef FILETEST_LSEEK_HOOK
		stdoutput.printf("\nmemory map failed seek after a failed map\n");
		mmapFailedSeekTests();
		#else
		stdoutput.printf("\nmemory map failed seek after a failed map\nlseek() can not be replaced\n");
		#endif

		#ifdef FILETEST_LSEEK_HOOK
		stdoutput.printf("\nfailed seek to a new block, then write and flush\n");
		failedSeekWriteTests();
		#else
		stdoutput.printf("\nfailed seek to a new block, then write and flush\nlseek() can not be replaced\n");
		#endif

		#ifdef FILETEST_LSEEK_HOOK
		stdoutput.printf("\nfailed fill of a new block, then write and flush\n");
		failedFillWriteTests();
		#else
		stdoutput.printf("\nfailed fill of a new block, then write and flush\nlseek() can not be replaced\n");
		#endif

		stdoutput.printf("\nmemory map on a block device\n");
		#ifdef FILETEST_BLOCKDEV_HOOK
		blockDeviceTests();
		#else
		stdoutput.printf("block device can not be faked\n");
		#endif

		stdoutput.printf("\nbuffered seek from the end of a block device\n");
		#ifdef FILETEST_BLOCKDEV_HOOK
		blockDeviceEndTests();
		#else
		stdoutput.printf("block device can not be faked\n");
		#endif

		stdoutput.printf("\nturn buffering off after a seek\n");
		#ifdef FILETEST_LSEEK_HOOK
		failedSeekUnsetTests();
		#else
		stdoutput.printf("lseek() can not be replaced\n");
		#endif
		unsetBufferingTests();

		stdoutput.printf("\nturn buffering off after a resize\n");
		unsetBufferingResizeTests();

		stdoutput.printf("\nturn buffering off on a fifo\n");
		unsetBufferingFifoTests();

		stdoutput.printf("\nturn buffering off with data in the write buffer\n");
		unsetBufferingFlushTests();
		#ifdef FILETEST_LSEEK_HOOK
		unsetBufferingFlushFailedTests();
		#else
		stdoutput.printf("lseek() can not be replaced\n");
		#endif
		unsetBufferingStreamFlushTests();

		stdoutput.printf("\nturn read buffering off or resize it with unread bytes\n");
		readBufferStreamUnreadTests();

		stdoutput.printf("\nclose and delete with data in the write buffer\n");
		closeFlushTests();

		#ifdef FILETEST_LSEEK_HOOK
		stdoutput.printf("\nopen and create with a failed flush on close\n");
		openFailedCloseTests();
		#else
		stdoutput.printf("\nopen and create with a failed flush on close\nlseek() can not be replaced\n");
		#endif

	} else {

		fl.open(testfiletxt,O_RDWR);

		test("try lock 2",!fl.tryLockFile(
				#if defined(F_WRLCK)
					F_WRLCK
				#else
					0
				#endif
					));

		test("lock 2",fl.lockFile(
				#if defined(F_WRLCK)
					F_WRLCK
				#else
					0
				#endif
					));
	}

	delete[] osname;

	return 0;
}
