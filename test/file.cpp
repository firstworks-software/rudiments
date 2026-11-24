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
