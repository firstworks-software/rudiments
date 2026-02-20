#include <rudiments/device.h>
#include <rudiments/permissions.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	// create a character device node
	// (requires root privileges on most systems)
	if (device::createDeviceNode("/tmp/testdevice",
				permissions::parsePermString("rw-rw-rw-"),
				'c',1,3)) {
		stdoutput.write("created /tmp/testdevice\n");
	} else {
		stdoutput.write("failed to create /tmp/testdevice\n");
		stdoutput.write("(this operation requires root privileges)\n");
	}


	// open a device for write
	device	d;
	if (d.open("/dev/null",O_WRONLY)) {

		// write some data to /dev/null
		d.write("this goes nowhere\n");

		stdoutput.write("wrote to /dev/null\n");

		d.close();
	} else {
		stdoutput.write("failed to open /dev/null\n");
	}


	// open /dev/urandom for read
	if (d.open("/dev/urandom",O_RDONLY)) {

		// read some random bytes
		byte_t	buf[16];
		d.read(buf,sizeof(buf));

		stdoutput.write("random bytes: ");
		for (uint16_t i=0; i<sizeof(buf); i++) {
			stdoutput.printf("%02x",buf[i]);
		}
		stdoutput.write('\n');

		d.close();
	} else {
		stdoutput.write("failed to open /dev/urandom\n");
	}
}
