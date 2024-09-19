// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/datetime.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/unixsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

static void usage() {
	stdoutput.printf("client "
			"[-host host] [-port port] [-socket socket] [-quiet] "
			"[-ccount count] [-mcount count] [-dcount count]\n");
}

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);
	if (cmdl.isFound("help")) {
		usage();
		return 0;
	}
	const char	*host="127.0.0.1";
	if (cmdl.isFound("host")) {
		host=cmdl.getValue("host");
	}
	uint16_t	port=9000;
	if (cmdl.isFound("port")) {
		port=charstring::convertToUnsignedInteger(
						cmdl.getValue("port"));
	}
	const char	*sock=NULL;
	if (cmdl.isFound("socket")) {
		sock=cmdl.getValue("socket");
	}
	const char	*msg="hello";
	if (cmdl.isFound("message")) {
		msg=cmdl.getValue("message");
	}
	bool	quiet=cmdl.isFound("quiet");
	int64_t	ccount=charstring::convertToInteger(cmdl.getValue("ccount"));
	if (ccount<0) {
		usage();
		return 1;
	} else if (!ccount) {
		ccount=1;
	}
	int64_t	mcount=charstring::convertToInteger(cmdl.getValue("mcount"));
	if (mcount<0) {
		usage();
		return 1;
	} else if (!mcount) {
		mcount=1;
	}
	int64_t	dcount=charstring::convertToInteger(cmdl.getValue("dcount"));
	if (dcount<0) {
		usage();
		return 1;
	} else if (!dcount) {
		dcount=1;
	}

	// duplicate the message the
	// specified number of times...
	bytebuffer	msgbuf;
	for (int64_t i=0; i<dcount; i++) {
		msgbuf.append(msg)->append(' ');
	}

{
	// create an inet socket client
	inetsocketclient	ifd;
	ifd.setHost(host);
	ifd.setPort(port);

	// create a unix socket client
	unixsocketclient	ufd;
	ufd.setFileName(sock);

	// decide which to use
	client	*fd=&ifd;
	if (sock) {
		fd=&ufd;
	}

	// set options
	//fd->setSocketWriteBufferSize(65536);
	//fd->setSocketReadBufferSize(65536);
	fd->setWriteBufferSize(65536);
	fd->setReadBufferSize(65536);
	fd->setNaglesAlgorithmEnabled(false);

	// get start time
	datetime	start;
	start.initFromSystemDateTime();

	// loop, having sessions with the server
	for (int64_t i=0; i<ccount; i++) {

		// connect 
		if (fd->connect()!=RESULT_SUCCESS) {
			if (error::getErrorNumber()) {
				stdoutput.printf("connect failed (1): %s\n",
						error::getErrorString());
			} else {
				stdoutput.printf("connect failed (2): "
							"unknown error\n");
			}
			continue;
		}

		if (!quiet) {
			stdoutput.printf("serverSession {\n");
		}

		// write the message to the server,
		// the specified number of times
		for (int64_t j=0; j<mcount; j++) {

			// write size
			ssize_t	sizewritten=fd->write((uint64_t)
						msgbuf.getSize());
			if (sizewritten<=0) {
				if (sizewritten==0) {
					stdoutput.printf(
						"  write() size failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  write() size failed (2): "
						"unknown error\n");
					break;
				}
			} else if (sizewritten!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  write() size failed (3): "
					"unknown error\n");
				break;
			}

			// write message
			sizewritten=fd->write(msgbuf.getBuffer(),
						msgbuf.getSize());
			if (sizewritten<=0) {
				if (sizewritten==0) {
					stdoutput.printf(
						"  write() msg failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() msg failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  write() msg failed (2): "
						"unknown error\n");
					break;
				}
			} else if (sizewritten!=(ssize_t)msgbuf.getSize()) {
				stdoutput.printf(
					"  write() msg failed (3): "
					"unknown error\n");
				break;
			}

			// flush write buffer
			if (!fd->flushWriteBuffer(-1,-1)) {
				stdoutput.printf("flushWriteBuffer() failed\n");
				break;
			}

			if (!quiet) {
				stdoutput.printf("\n  Sent message... "
					"(size=%d):\n  ",msgbuf.getSize());
				stdoutput.safePrint(msgbuf.getBuffer(),
					(msgbuf.getSize()<=80)?
						msgbuf.getSize():80);
				if (msgbuf.getSize()>80) {
					stdoutput.write("...");
				}
				stdoutput.write('\n');
				stdoutput.printf("\n  Receiving response...");
			}

			// read size
			uint64_t	msgsize;
			ssize_t	sizeread=fd->read(&msgsize);
			if (sizeread<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"  read() size failed (0): "
						"eof\n");
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",
						error::getErrorString());
					break;
				} else {
					stdoutput.printf(
						"  read() size failed (2): "
						"unknown error\n");
					break;
				}
			} else if (sizeread!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  read() size failed (3): "
					"unknown error\n");
				break;
			}

			// read message
			byte_t	*msg=new byte_t[msgsize];
			sizeread=fd->read(msg,msgsize);
			if (sizeread<=0) {
				if (sizeread==0) {
					stdoutput.printf(
						"  read() msg failed (0): "
						"eof\n");
					delete[] msg;
					break;
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() msg failed (1): "
						"%s\n",
						error::getErrorString());
					delete[] msg;
					break;
				} else {
					stdoutput.printf(
						"  read() msg failed (2): "
						"unknown error\n");
					delete[] msg;
					break;
				}
			} else if (sizeread!=(ssize_t)msgsize) {
				stdoutput.printf(
					"  read() msg failed (3): "
					"unknown error\n");
				delete[] msg;
				break;
			}

			if (!quiet) {
				stdoutput.printf("  success\n");
			}

			delete[] msg;
		}

		if (!quiet) {
			stdoutput.printf("}\n");
		}

		// close the connection to the server
		fd->close();
	}

	// get end time
	datetime	end;
	end.initFromSystemDateTime();

	// calculate total time
	uint32_t	sec=end.getEpoch()-start.getEpoch();
	int32_t		usec=end.getMicrosecond()-
					start.getMicrosecond();
	if (usec<0) {
		sec--;
		usec=usec+1000000;
	}

	float	totalsec=(float)sec+((float)usec/1000000.0);
	float	mbps=(((float)ccount*(float)mcount*(float)msgbuf.getSize())/
						1024.0/1024.0*8.0)/totalsec;
	stdoutput.printf("sec:  %18.2f\n",totalsec);
	stdoutput.printf("mbps: %18.2f\n",mbps);
}

	return 0;
}
