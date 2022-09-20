// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
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

int main(int argc, const char **argv) {

	// process command line
	commandline	cmdl(argc,argv);
	bool	write=cmdl.found("write");
	bool	read=cmdl.found("read");
	bool	unbuffered=cmdl.found("unbuffered");
	bool	tradbuffered=cmdl.found("tradbuffered");
	bool	mmapbuffered=cmdl.found("mmapbuffered");
	bool	showprogress=cmdl.found("showprogress");
	if ((!write && !read) ||
		(read && !unbuffered && !tradbuffered && !mmapbuffered)) {
		stderror.printf("usage: ./file [-write] [-read]\n"
				"	[-unbuffered] [-tradbuffered] "
				"[-mmapbuffered]\n"
				"	[-showprogress]\n"
				"	One or both of either -read or -write "
				"must be specified.\n"
				"	If -read is specified then one of "
				"either -unbuffered, -tradbuffered, or\n"
				"	-mmapbuffered must also be "
				"specified.\n");
		process::exit(1);
	}

	// determine the file name
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
	if (read) {
		if (!f.open(filename.getString(),O_RDONLY)) {
			stdoutput.printf("open failed\n");
			process::exit(1);
		}
	} else {
		if (!f.create(filename.getString(),
				permissions::evalPermString("rw-r--r--"))) {
			stdoutput.printf("create failed\n");
			process::exit(1);
		}
	}

	// use optimium block size for buffers
	filesystem	fs;
	fs.open(filename.getString());
	int64_t	blocksize=fs.getOptimumTransferBlockSize()*3;
	if (blocksize<=0) {
		stdoutput.printf("get optimum block size failed\n");
		process::exit(1);
	}
	stdoutput.printf("blocksize: %lld\n",blocksize);
	fs.close();
	f.setWriteBufferSize(blocksize);

	// declare start/end for timings
	datetime	start;
	datetime	end;

	// determine the file size
	uint64_t	filesize=(uint64_t)1024*(uint64_t)1024*(uint64_t)512;

	// write to the file
	if (write) {
		char	*hr=charstring::humanReadable(filesize);
		stdoutput.printf("writing %s bytes...",hr);
		if (showprogress) {
			stdoutput.printf("\n");
		} else {
			stdoutput.flush();
		}

		delete[] hr;
		start.initFromSystemDateTime();
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
		end.initFromSystemDateTime();
		stdoutput.printf("done writing %lld bytes\n",filesize);
		displayTime(&start,&end);
		stdoutput.printf("\n");
		if (!read) {
			process::exit(0);
		}
	}

	// read from the file
	for (int64_t readsize=1; readsize<=blocksize; readsize*=2) {

		char	*buf=new char[readsize];

		const char	*type[]={
			"   unbuffered",
			"trad-buffered",
			"mmap-buffered"
		};
		for (uint8_t i=0; i<3; i++) {

			if (!unbuffered && i==0) {
				continue;
			}
			if (!tradbuffered && i==1) {
				continue;
			}
			if (!mmapbuffered && i==2) {
				continue;
			}

			stdoutput.printf("reading: %s "
						"(readsize=%04lld)...  ",
						type[i],readsize);
			if (showprogress) {
				stdoutput.printf("\n");
			} else {
				stdoutput.flush();
			}

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

			start.initFromSystemDateTime();
			for (uint64_t j=0; j<filesize; j+=readsize) {
				if (f.read(buf,readsize)!=readsize) {
					stdoutput.printf(
						"read failed at %lld\n",i);
					process::exit(1);
				}
				if (showprogress) {
					if (!(j%(1024*1024*50))) {
						stdoutput.printf(
						"  %lld bytes read...\n",i);
					}
				}
			}
			end.initFromSystemDateTime();

			if (showprogress) {
				stdoutput.printf("done reading %lld bytes\n",
								filesize);
			}
			displayTime(&start,&end);
			if (showprogress) {
				stdoutput.printf("\n");
			}
		}

		stdoutput.printf("\n");
		delete[] buf;
	}

	// clean up
	f.close();
	if (write && read) {
		file::remove(filename.getString());
	}

	process::exit(0);
}
