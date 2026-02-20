#include <rudiments/url.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	url	u;

	// open a url for read
	if (!u.open("http://www.google.com",O_RDONLY)) {
		stdoutput.printf("failed to open url: %s\n",u.getError());
		return 1;
	}


	// read and display the content
	char	buf[1024];
	for (;;) {

		ssize_t	bytesread=u.read(buf,sizeof(buf)-1);

		if (bytesread<=0) {
			break;
		}

		buf[bytesread]='\0';
		stdoutput.write(buf);
	}
	stdoutput.write('\n');

	u.close();
}
