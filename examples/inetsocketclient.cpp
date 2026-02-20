#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a client and connect to a server
	inetsocketclient	client;
	client.setHost("localhost");
	client.setPort(8000);
	client.setTimeoutSeconds(5);
	client.setTimeoutMicroseconds(0);
	client.setRetryWait(1);
	client.setTries(3);

	if (client.connect()!=RESULT_SUCCESS) {
		stdoutput.write("failed to connect to localhost:8000\n");
		return 1;
	}
	stdoutput.write("connected to localhost:8000\n");


	// send a message to the server
	const char	*message="hello from the client";
	client.write(message,charstring::getLength(message));
	stdoutput.write("sent message\n");


	// read the server's response
	char	buf[1024];
	ssize_t	bytesread=client.read(buf,sizeof(buf)-1);
	if (bytesread>0) {
		buf[bytesread]='\0';
		stdoutput.printf("received: %s\n",buf);
	}


	// close the connection
	client.close();
}
