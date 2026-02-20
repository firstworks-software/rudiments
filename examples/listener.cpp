#include <rudiments/listener.h>
#include <rudiments/inetsocketserver.h>
#include <rudiments/unixsocketserver.h>
#include <rudiments/singlylinkedlist.h>
#include <rudiments/charstring.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create an inet socket server
	inetsocketserver	inetserver;
	if (!inetserver.listen(NULL,8002,15)) {
		stdoutput.write("failed to listen on port 8002\n");
		return 1;
	}
	stdoutput.write("listening on port 8002\n");


	// create a unix socket server
	unixsocketserver	unixserver;
	if (!unixserver.listen("/tmp/testlistener",0,15)) {
		stdoutput.write("failed to listen on /tmp/testlistener\n");
		return 1;
	}
	stdoutput.write("listening on /tmp/testlistener\n\n");


	// listen for connections on both sockets
	listener	lsnr;
	lsnr.addReadFileDescriptor(&inetserver);
	lsnr.addReadFileDescriptor(&unixserver);

	stdoutput.write("waiting for a connection "
			"on either socket...\n");

	// wait up to 10 seconds for activity
	int32_t	result=lsnr.listen(10,0);
	if (result>0) {

		// check which socket(s) are ready
		singlylinkedlist<filedescriptor *>	*readylist=
						lsnr.getReadReadyList();

		for (listnode<filedescriptor *>
				*node=readylist->getFirst();
				node; node=node->getNext()) {

			if (node->getValue()==
				(filedescriptor *)&inetserver) {
				stdoutput.write("inet socket is ready\n");
			}
			if (node->getValue()==
				(filedescriptor *)&unixserver) {
				stdoutput.write("unix socket is ready\n");
			}
		}

	} else if (result==RESULT_TIMEOUT) {
		stdoutput.write("timed out\n");
	} else {
		stdoutput.write("error\n");
	}


	// clean up
	inetserver.close();
	unixserver.close();
}
