// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/commandline.h>
#include <rudiments/permissions.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/error.h>
#include <rudiments/file.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// process the command line
	commandline	cmdl(argc,argv);

	if (cmdl.isFound("help")) {
		stdoutput.printf("server [-port port] [-quiet]\n");
		return 0;
	}

	uint16_t	port=9000;
	if (cmdl.isFound("port")) {
		port=charstring::convertToUnsignedInteger(cmdl.getValue("port"));
	}
	bool	quiet=cmdl.isFound("quiet");


	// create an inet socket server
	inetsocketserver	iss;

	// listen
	if (iss.listen(NULL,port,0)) {

		for (;;) {

			// accept
			filedescriptor	*fd=iss.accept();
			if (!fd) {
				if (error::getErrorNumber()) {
					stdoutput.printf(
						"accept failed (1): %s\n",
						error::getErrorString());
				}
				continue;
			}
			fd->setWriteBufferSize(65536);
			fd->setReadBufferSize(65536);

			if (!quiet) {
				stdoutput.printf("clientSession {\n");
			}

			// read messages from the client...
			for (;;) {

				uint64_t	msgsize;
				ssize_t	sizeread=fd->read(&msgsize);
				if (sizeread<=0) {
					if (sizeread==0) {
						stdoutput.printf(
						"  read() size failed (0): "
						"eof\n");
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

				byte_t	*msg=new byte_t[msgsize];
				sizeread=fd->read(msg,msgsize);
				if (sizeread<=0) {
					if (sizeread==0) {
						stdoutput.printf(
						"  read() size failed (0): "
						"eof\n");
					} else if (error::getErrorNumber()) {
						stdoutput.printf(
						"  read() size failed (1): "
						"%s\n",error::getErrorString());
					} else {
						stdoutput.printf(
						"  read() size failed (2): "
						"unknown error\n");
					}
					delete[] msg;
					break;
				} else if (sizeread!=(ssize_t)msgsize) {
					stdoutput.printf(
						"  read() size failed (3): "
						"unknown error\n");
					delete[] msg;
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
					stdoutput.printf(
						"\n  Sending response...");
				}
	
				ssize_t	sizewritten=fd->write(msgsize);
				if (sizewritten<=0) {
					if (sizewritten==0) {
						stdoutput.printf(
						"  write() size failed (0): "
						"eof\n");
					} else if (error::getErrorNumber()) {
						stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",error::getErrorString());
					} else {
						stdoutput.printf(
						"  write() size failed (2): "
						"unknown error\n");
					}
					delete[] msg;
					break;
				} else if (sizewritten!=sizeof(uint64_t)) {
					stdoutput.printf(
						"  write() size failed (3): "
						"unknown error\n");
					delete[] msg;
					break;
				}

				sizewritten=fd->write(msg,msgsize);
				if (sizewritten<=0) {
					if (sizewritten==0) {
						stdoutput.printf(
						"  write() size failed (0): "
						"eof\n");
					} else if (error::getErrorNumber()) {
						stdoutput.printf(
						"  write() size failed (1): "
						"%s\n",error::getErrorString());
					} else {
						stdoutput.printf(
						"  write() size failed (2): "
						"unknown error\n");
					}
					delete[] msg;
					break;
				} else if (sizewritten!=(ssize_t)msgsize) {
					stdoutput.printf(
						"  write() size failed (3): "
						"unknown error\n");
					delete[] msg;
					break;
				}

				delete[] msg;

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
	}

	stdoutput.printf("error listening on port %d\n%s\n",
				port,error::getErrorString());
	return 1;
}
