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

char safeRead(char *ptr) {
	char	ch;
	bytestring::copy(&ch,ptr,sizeof(char));
	return ch;
}
char storageBufferedRead(char *ptr) {
	return safeRead(ptr);
}
char bufferedRead(char *ptr) {
	return bufferedRead(ptr);
}
char read(char *ptr) {
	return storageBufferedRead(ptr);
}

int main(int argc, const char **argv) {

	const char	*filename="/home/dmuse/testfile.txt";

	// create the file
	file	f;
	if (!f.open(filename,O_RDONLY)) {
		stdoutput.printf("open failed\n");
		process::exit(1);
	}
	/*if (!f.create(filename,permissions::evalPermString("rw-r--r--"))) {
		stdoutput.printf("create failed\n");
		process::exit(1);
	}*/

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
#if 0
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
#endif

	// read tests
	const char	*type[]={
		"unbuffered","buffered","mmap-buffered"
	};
	//for (uint8_t i=0; i<3; i++) {
	// for now, skip unbuffered
	//for (uint8_t i=1; i<3; i++) {
	// for now, skip unbuffered and regular-buffered
	for (uint8_t i=2; i<3; i++) {

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

#if 0
	// manual mmap test
	memorymap	m;
	char		*p;
	char		ch;
	stdoutput.printf("reading manual mmap...\n");
	start.getSystemDateAndTime();
bool		isstream=false;
uint64_t	offset=0;
uint64_t	blockoffset=0;
unsigned char	*writebuffer;
unsigned char	*writebufferend;
unsigned char	*writebuffertail;
	for (uint64_t i=0; i<filesize; i++) {
		if (isstream) {
			continue;
		}
		if (!blocksize) {
			continue;
		}
ssize_t		bytestoread=1;
ssize_t		bytesread=0;
		for (;;) {
			if (!(i%blocksize)) {
				if (i) {
					m.detach();
				}
blockoffset=i;
f.getCurrentProperties();
				if (!m.attach(f.getFileDescriptor(),
					i,blocksize,PROT_READ,MAP_PRIVATE)) {
					stdoutput.printf(
						"  mmap attach failed\n");
					process::exit(1);
				}
writebuffer=(unsigned char *)m.getData();
writebufferend=writebuffer+blocksize;
writebuffertail=writebuffer;
				p=(char *)m.getData();
				if (!(i%(1024*1024*50))) {
					stdoutput.printf(
						"  %lld bytes read...\n",i);
				}
			}
unsigned char	*bufferhead=writebuffer+(offset-blockoffset);
ssize_t		bytesavailable=writebufferend-bufferhead;
			//ch=*p;
			//bytestring::copy(&ch,p,sizeof(char));
			ch=read(p);
bytesread++;
offset+=bytestoread;
// avoids set but not used warning
bufferhead++;
bytesavailable++;
			if (writebuffertail<writebufferend) {
				writebuffertail=bufferhead+bytesread;
			}
			if (bytesread==bytestoread) {
				break;
			}
		}
	}
	m.detach();
	end.getSystemDateAndTime();
	stdoutput.printf("done reading %lld bytes\n",filesize);
	displayTime(&start,&end);
	stdoutput.printf("\n");
	// avoids set but not used warning
	ch++;
writebuffertail++;
#endif

	// clean up
	f.close();
	//file::remove(filename);

	process::exit(0);
}
