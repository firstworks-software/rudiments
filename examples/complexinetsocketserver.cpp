#include <rudiments/inetsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a server socket and listen on port 8001
	inetsocketserver	server;
	if (!server.listen(NULL,8001,15)) {
		stdoutput.write("failed to listen on port 8001\n");
		return 1;
	}
	stdoutput.write("listening on port 8001...\n");


	// handle 3 client connections using fork
	for (uint16_t i=0; i<3; i++) {

		// accept a client connection
		filedescriptor	*clientsock=server.accept();
		if (!clientsock) {
			continue;
		}
		stdoutput.write("accepted a connection\n");

		// fork a child to handle the client
		pid_t	pid=process::fork();

		if (pid==0) {

			// child process...
			// close the server socket in the child
			server.close();

			// read a message from the client
			char	buf[1024];
			ssize_t	bytesread=
				clientsock->read(buf,sizeof(buf)-1);
			if (bytesread>0) {
				buf[bytesread]='\0';
				stdoutput.printf("child received: %s\n",buf);
			}

			// send a response
			const char	*resp="hello from child";
			clientsock->write(resp,
					charstring::getLength(resp));

			delete clientsock;
			process::exit(0);
		}

		// parent process...
		// close the client socket in the parent
		delete clientsock;
	}


	// automatically wait on child processes to prevent zombies
	process::setWaitForChildren(true);

	server.close();
}
