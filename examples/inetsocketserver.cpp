#include <rudiments/inetsocketserver.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a server socket and listen on port 8000
	inetsocketserver	server;
	if (!server.listen(NULL,8000,15)) {
		stdoutput.write("failed to listen on port 8000\n");
		return 1;
	}
	stdoutput.write("listening on port 8000...\n");


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
	const char	*response="hello from the server";
	clientsock->write(response,charstring::getLength(response));
	stdoutput.write("sent response\n");


	// close and clean up
	delete clientsock;
	server.close();
}
