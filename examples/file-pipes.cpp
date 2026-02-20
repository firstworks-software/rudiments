#include <rudiments/filedescriptor.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a pipe
	filedescriptor	readfd;
	filedescriptor	writefd;
	if (!filedescriptor::createPipe(&readfd,&writefd)) {
		stdoutput.write("failed to create pipe\n");
		return 1;
	}
	stdoutput.write("created pipe\n\n");


	// fork a child process
	pid_t	pid=process::fork();

	if (pid==0) {

		// child process...
		// close the read side
		readfd.close();

		// write a message to the parent
		writefd.write("hello from the child\n");
		writefd.close();

		process::exit(0);

	} else if (pid>0) {

		// parent process...
		// close the write side
		writefd.close();

		// read the message from the child
		char	buf[1024];
		ssize_t	bytesread=readfd.read(buf,sizeof(buf)-1);
		if (bytesread>0) {
			buf[bytesread]='\0';
			stdoutput.printf("parent received: %s",buf);
		}
		readfd.close();

		// wait for the child to exit
		process::wait(pid);

	} else {
		stdoutput.write("fork failed\n");
	}
}
