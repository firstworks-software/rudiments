#include <rudiments/chat.h>
#include <rudiments/device.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// define a chat script to initialize a modem
	const char	*chatscript=
		"<script>"
		"  <timeout seconds=\"5\"/>"
		"  <abort string=\"ERROR\"/>"
		"  <abort string=\"NO CARRIER\"/>"
		"  <send string=\"ATZ\\r\\n\"/>"
		"  <expect string=\"OK\"/>"
		"  <send string=\"ATQ0V1E1\\r\\n\"/>"
		"  <expect string=\"OK\"/>"
		"</script>";


	// open a serial device to chat with
	// (will fail without a real serial device)
	device	d;
	if (!d.open("/dev/ttyS0",O_RDWR|O_NOCTTY)) {
		stdoutput.write("failed to open /dev/ttyS0\n");
		stdoutput.write("(no serial device available)\n\n");
	} else {

		// create a chat instance and run the script
		chat	c;
		c.setReadAndWriteFileDescriptor(&d);

		stdoutput.write("running chat script...\n");

		char	*abortstr=NULL;
		int32_t	result=c.runScript(chatscript,&abortstr);

		if (result==RESULT_SUCCESS) {
			stdoutput.write("chat script succeeded\n");
		} else if (result==RESULT_TIMEOUT) {
			stdoutput.write("chat script timed out\n");
		} else if (result==RESULT_ERROR) {
			stdoutput.write("chat script error\n");
		} else {
			stdoutput.printf("chat script aborted: %s\n",
								abortstr);
		}

		delete[] abortstr;
		d.close();
	}


	// display the chat script
	stdoutput.write("chat script:\n");
	stdoutput.printf("%s\n",chatscript);
}
