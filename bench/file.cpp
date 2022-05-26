// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/environment.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/file.h>
#include <rudiments/permissions.h>
#include <rudiments/filesystem.h>
#include <rudiments/stdio.h>
#include <rudiments/datetime.h>
#include <rudiments/process.h>
#include <rudiments/memorymap.h>
#include <rudiments/bytestring.h>
#include "../test/test.cpp"

//#define ONLYREAD 1
//#define SHOWPROGRESS 1

int main(int argc, const char **argv) {

	// 
	stringbuffer	filename;
	const char	*home=environment::getValue("HOME");
	if (charstring::isNullOrEmpty(home)) {
		filename.append("/home/");
		const char	*user=environment::getValue("USER");
		if (charstring::isNullOrEmpty(user)) {
			filename.append("dmuse");
		} else {
			filename.append(user);
		}
	} else {
		filename.append(home);
	}
	filename.append('/')->append("testfile.txt");
	stdoutput.printf("filename: %s\n",filename.getString());

	// create the file
	file	f;
#ifdef ONLYREAD
	if (!f.open(filename.getString(),O_RDWR)) {
		stdoutput.printf("open failed\n");
		process::exit(1);
	}
#else
	if (!f.create(filename.getString(),
			permissions::evalPermString("rw-r--r--"))) {
		stdoutput.printf("create failed\n");
		process::exit(1);
	}
#endif

	// use optimium block size for buffers
	filesystem	fs;
	fs.open(filename.getString());
	int64_t	blocksize=fs.getOptimumTransferBlockSize()*3;
	if (blocksize<=0) {
		stdoutput.printf("get optimum block size failed\n");
		process::exit(1);
	}
	fs.close();
	f.setWriteBufferSize(blocksize);

	datetime	start;
	datetime	end;

	// write to the file
	uint64_t	filesize=(uint64_t)1024*(uint64_t)1024*(uint64_t)512;
#ifndef ONLYREAD
	char	*hr=charstring::humanReadable(filesize);
	stdoutput.printf("writing %s bytes...",hr);
#ifdef SHOWPROGRESS
	stdoutput.printf("\n");
#else
	stdoutput.flush();
#endif
	delete[] hr;
	start.getSystemDateAndTime();
	for (uint64_t i=0; i<filesize; i++) {
		if (f.write('1')!=sizeof(char)) {
			stdoutput.printf("write failed at %lld\n",i);
			process::exit(1);
		}
		if (!(i%(1024*1024*50))) {
			stdoutput.printf("  %lld bytes written...\n",i);
		}
	}
	f.flushWriteBuffer(-1,-1);
	end.getSystemDateAndTime();
	stdoutput.printf("done writing %lld bytes\n",filesize);
	displayTime(&start,&end);
	stdoutput.printf("\n");
//process::exit(0);
#endif

	for (int64_t readsize=1; readsize<=blocksize; readsize*=2) {

		char	*buf=new char[readsize];

		// read tests
		const char	*type[]={
			"   unbuffered",
			"     buffered",
			"mmap-buffered"
		};
		//for (uint8_t i=0; i<3; i++) {
		// for now, skip unbuffered
		for (uint8_t i=1; i<3; i++) {
		// for now, skip unbuffered and regular-buffered
		//for (uint8_t i=2; i<3; i++) {

			stdoutput.printf("reading: %s "
						"(readsize=%04lld)...  ",
						type[i],readsize);
#ifdef SHOWPROGRESS
			stdoutput.printf("\n");
#else
			stdoutput.flush();
#endif

			f.setPositionRelativeToBeginning(0);
			f.sequentialAccess(0,filesize);
			f.willNeed(0,filesize);

			if (i==0) {
				f.setReadBufferSize(0);
			} else if (i==1) {
				f.setReadBufferSize(blocksize);
				f.setMmapBufferingEnabled(false);
			} else if (i==2) {
				f.setReadBufferSize(blocksize);
				f.setMmapBufferingEnabled(true);
			}

			start.getSystemDateAndTime();
			for (uint64_t i=0; i<filesize; i+=readsize) {
				if (f.read(buf,readsize)!=readsize) {
					stdoutput.printf(
						"read failed at %lld\n",i);
					process::exit(1);
				}
#ifdef SHOWPROGRESS
				if (!(i%(1024*1024*50))) {
					stdoutput.printf(
						"  %lld bytes read...\n",i);
				}
#endif
			}
			end.getSystemDateAndTime();

#ifdef SHOWPROGRESS
			stdoutput.printf("done reading %lld bytes\n",filesize);
#endif
			displayTime(&start,&end);
#ifdef SHOWPROGRESS
			stdoutput.printf("\n");
#endif
		}

		// manual mmap test
		memorymap	m;
		char		*p;
		stdoutput.printf("reading:   manual mmap "
					"(readsize=%04lld)...  ",
					readsize);
#ifdef SHOWPROGRESS
			stdoutput.printf("\n");
#else
			stdoutput.flush();
#endif
		f.sequentialAccess(0,filesize);
		f.willNeed(0,filesize);
		int32_t	fd=f.getFileDescriptor();
		start.getSystemDateAndTime();
		for (uint64_t i=0; i<filesize; i+=readsize) {
			if (!(i%blocksize)) {
				if (i) {
					m.detach();
				}
				if (!m.attach(fd,i,blocksize,PROT_READ,
								MAP_SHARED)) {
					stdoutput.printf(
						"  mmap attach failed\n");
					process::exit(1);
				}
				p=(char *)m.getData();
#ifdef SHOWPROGRESS
				if (!(i%(1024*1024*50))) {
					stdoutput.printf(
						"  %lld bytes read...\n",i);
				}
#endif
			}
			bytestring::copy(buf,p,readsize);
			p+=readsize;
		}
		m.detach();
		end.getSystemDateAndTime();
#ifdef SHOWPROGRESS
		stdoutput.printf("done reading %lld bytes\n",filesize);
#endif
		displayTime(&start,&end);
#ifdef SHOWPROGRESS
		stdoutput.printf("\n");
#endif

		stdoutput.printf("\n");
		delete[] buf;
	}

	// clean up
	f.close();
#ifdef ONLYREAD
	file::remove(filename.getString());
#endif

	process::exit(0);
}
