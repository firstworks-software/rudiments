#include <rudiments/unixsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a server socket and listen on a unix socket file
	unixsocketserver	server;
	if (!server.listen("/tmp/testsocket",0,15)) {
		stdoutput.write("failed to listen on /tmp/testsocket\n");
		return 1;
	}
	stdoutput.write("listening on /tmp/testsocket...\n");


	// accept a client connection
	filedescriptor	*clientsock=server.accept();
	if (!clientsock) {
		stdoutput.write("failed to accept connection\n");
		return 1;
	}
	stdoutput.write("accepted a connection\n");


	// read a message from the client
	char	buf[1024];
	ssize_t	bytesread=clientsock->read(buf,sizeof(buf)-1);
	if (bytesread>0) {
		buf[bytesread]='\0';
		stdoutput.printf("received: %s\n",buf);
	}


	// send a response back to the client
	const char	*response="hello from the unix server";
	clientsock->write(response,charstring::getLength(response));
	stdoutput.write("sent response\n");


	// close and clean up
	delete clientsock;
	server.close();
}
