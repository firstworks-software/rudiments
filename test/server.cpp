// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/permissions.h>
#include <rudiments/listener.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

#ifndef _WIN32
const char	*sock="/tmp/socket.sck";
#else
const char	*sock="socket.sck";
#endif

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);

	if (cmdl.isFound("help")) {
		stdoutput.printf("server [-port port] "
				"[-socket socket] [-quiet]\n");
		return 0;
	}

	uint16_t	port=9000;
	if (cmdl.isFound("port")) {
		port=charstring::convertToUnsignedInteger(
					cmdl.getValue("port"));
	}
	if (cmdl.isFound("socket")) {
		sock=cmdl.getValue("socket");
	}
	bool	quiet=cmdl.isFound("quiet");


	// create socket servers
	inetsocketserver	iss;
	if (!iss.listen(NULL,port,0)) {
		stdoutput.printf("error listening on port %d\n%s\n",
					port,error::getErrorString());
		process::exit(0);
	}
	unixsocketserver	uss;
	if (!uss.listen(sock,0000,15)) {
		stdoutput.printf("error listening on socket %s\n%s\n",
					sock,error::getErrorString());
		process::exit(0);
	}

	// create a listener and add the 2 sockets to it
	listener	pool;
	pool.addReadFileDescriptor(&iss);
	pool.addReadFileDescriptor(&uss);

	// buffers
	uint64_t	msgsize;
	byte_t		msg[5*1024*1024];

	for (;;) {

		// listen
		if (pool.listen(-1,-1)!=RESULT_SUCCESS) {
			stdoutput.printf("error listening on pool\n%s\n",
						error::getErrorString());
			process::exit(0);
		}

		// get ready file descrptor
		filedescriptor	*readyfd=pool.getReadReadyList()->
						getFirst()->getValue();

		// accept
		filedescriptor	*fd=NULL;
		if (readyfd==&iss) {
			fd=iss.accept();
		} else if (readyfd==&uss) {
			fd=uss.accept();
		}
		if (!fd) {
			if (error::getErrorNumber()) {
				stdoutput.printf(
					"accept failed (1): %s\n",
					error::getErrorString());
			}
			continue;
		}
		//fd->setSocketWriteBufferSize(65536);
		//fd->setSocketReadBufferSize(65536);
		fd->setWriteBufferSize(65536);
		fd->setReadBufferSize(65536);
		fd->setNaglesAlgorithmEnabled(false);

		if (!quiet) {
			stdoutput.printf("clientSession {\n");
		}

		// read messages from the client...
		for (;;) {

			ssize_t	sizeread=fd->read(&msgsize,10,0);
			if (sizeread<=0) {
				if (sizeread==0) {
					if (!quiet) {
						stdoutput.printf(
							"  read() size failed "
							"(0): eof\n");
					}
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",error::getErrorString());
				} else {
					stdoutput.printf(
						"  read() size failed (2): "
						"unknown error\n");
				}
				break;
			} else if (sizeread!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  read() size failed (3): "
					"unknown error\n");
				break;
			}

			sizeread=fd->read(msg,msgsize,10,0);
			if (sizeread<=0) {
				if (sizeread==0) {
					if (!quiet) {
						stdoutput.printf(
							"  read() size failed "
							"(0): eof\n");
					}
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",error::getErrorString());
				} else {
					stdoutput.printf(
						"  read() size failed (2): "
						"unknown error\n");
				}
				break;
			} else if (sizeread!=(ssize_t)msgsize) {
				stdoutput.printf(
					"  read() size failed (3): "
					"unknown error\n");
				break;
			}

			if (!quiet) {
				stdoutput.printf(
					"\nReceived message... "
					"(size=%d):\n",msgsize);
				stdoutput.safePrint(msg,
					(msgsize<=70)?msgsize:70);
				if (msgsize>70) {
					stdoutput.write("...");
				}
				stdoutput.write('\n');
				stdoutput.printf("\n  Sending response...");
			}

			ssize_t	sizewritten=fd->write(msgsize,10,0);
			if (sizewritten<=0) {
				if (sizewritten==0) {
					if (!quiet) {
						stdoutput.printf(
							"  write() size failed "
							"(0): eof\n");
					}
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",error::getErrorString());
				} else {
					stdoutput.printf(
						"  write() size failed (2): "
						"unknown error\n");
				}
				break;
			} else if (sizewritten!=sizeof(uint64_t)) {
				stdoutput.printf(
					"  write() size failed (3): "
					"unknown error\n");
				break;
			}

			sizewritten=fd->write(msg,msgsize,10,0);
			if (sizewritten<=0) {
				if (sizewritten==0) {
					if (!quiet) {
						stdoutput.printf(
							"  write() size failed "
							"(0): eof\n");
					}
				} else if (error::getErrorNumber()) {
					stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",error::getErrorString());
				} else {
					stdoutput.printf(
						"  write() size failed (2): "
						"unknown error\n");
				}
				break;
			} else if (sizewritten!=(ssize_t)msgsize) {
				stdoutput.printf(
					"  write() size failed (3): "
					"unknown error\n");
				break;
			}

			if (!fd->flushWriteBuffer(-1,-1)) {
				stdoutput.printf(
					"\n  flushWriteBuffer() msg "
					"failed\n");
				break;
			}

			if (!quiet) {
				stdoutput.printf("  success\n");
			}
		}

		if (!quiet) {
			stdoutput.printf("}\n");
		}

		// close and delete the client socket
		delete fd;
	}
	return 1;
}
