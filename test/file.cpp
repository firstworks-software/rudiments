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

	process::exit(0);
}
