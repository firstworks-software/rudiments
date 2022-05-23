// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

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

int main(int argc, const char **argv) {

	const char	*filename="/home/dmuse/testfile.txt";

	// create the file
	file	f;
#ifdef ONLYREAD
	if (!f.open(filename,O_RDWR)) {
		stdoutput.printf("open failed\n");
		process::exit(1);
	}
#else
	if (!f.create(filename,permissions::evalPermString("rw-r--r--"))) {
		stdoutput.printf("create failed\n");
		process::exit(1);
	}
#endif

	// use optimium block size for buffers
	filesystem	fs;
	fs.open(filename);
	int64_t	blocksize=fs.getOptimumTransferBlockSize();
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
	stdoutput.printf("writing %s bytes...\n",hr);
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

	// read tests
	const char	*type[]={
		"unbuffered","buffered","mmap-buffered"
	};
	for (uint8_t i=0; i<3; i++) {
	// for now, skip unbuffered
	//for (uint8_t i=1; i<3; i++) {
	// for now, skip unbuffered and regular-buffered
	//for (uint8_t i=2; i<3; i++) {

		stdoutput.printf("reading %s...\n",type[i]);

		f.setPositionRelativeToBeginning(0);

		if (i==0) {
			f.setReadBufferSize(0);
		} else if (i==1) {
			f.setReadBufferSize(blocksize);
			f.setMmapBufferingEnabled(false);
		} else if (i==2) {
			f.setReadBufferSize(blocksize);
			f.setMmapBufferingEnabled(true);
		}

		char	ch;
		start.getSystemDateAndTime();
		for (uint64_t i=0; i<filesize; i++) {
			if (f.read(&ch)!=sizeof(char)) {
				stdoutput.printf("read failed at %lld\n",i);
				process::exit(1);
			}
			if (!(i%(1024*1024*50))) {
				stdoutput.printf("  %lld bytes read...\n",i);
			}
		}
		end.getSystemDateAndTime();

		stdoutput.printf("done reading %lld bytes\n",filesize);
		displayTime(&start,&end);
		stdoutput.printf("\n");
	}

	// manual mmap test
	memorymap	m;
	char		*p;
	char		ch;
	stdoutput.printf("reading manual mmap...\n");
	start.getSystemDateAndTime();
	for (uint64_t i=0; i<filesize; i++) {
		if (!(i%blocksize)) {
			if (i) {
				m.detach();
			}
			if (!m.attach(f.getFileDescriptor(),
				i,blocksize,PROT_READ,MAP_PRIVATE)) {
				stdoutput.printf(
					"  mmap attach failed\n");
				process::exit(1);
			}
			p=(char *)m.getData();
			if (!(i%(1024*1024*50))) {
				stdoutput.printf(
					"  %lld bytes read...\n",i);
			}
		}
		ch=*p;
	}
	m.detach();
	end.getSystemDateAndTime();
	stdoutput.printf("done reading %lld bytes\n",filesize);
	displayTime(&start,&end);
	stdoutput.printf("\n");
	// avoids set but not used warning
	ch++;

	// clean up
	f.close();
	//file::remove(filename);

	process::exit(0);
}
