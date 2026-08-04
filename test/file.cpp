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
#include "test.cpp"

#ifndef WIN32
const char	*testfiletxt="/tmp/testfile.txt";
#else
const char	*testfiletxt="testfile.txt";
#endif

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
