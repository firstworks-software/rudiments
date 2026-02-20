#include <rudiments/filesystem.h>
#include <rudiments/stdio.h>

int main(int argc, const char **argv) {

	filesystem	fs;

	// open the filesystem containing the root directory
	if (!fs.open("/")) {
		stdoutput.write("failed to open filesystem\n");
		return 1;
	}


	// print filesystem properties
	stdoutput.printf("type:              %lld\n",fs.getType());
	stdoutput.printf("type name:         %s\n",fs.getTypeName());
	stdoutput.printf("block size:        %lld\n",fs.getBlockSize());
	stdoutput.printf("optimum block:     %lld\n",
					fs.getOptimumTransferBlockSize());
	stdoutput.printf("total blocks:      %lld\n",fs.getTotalBlocks());
	stdoutput.printf("free blocks:       %lld\n",fs.getFreeBlocks());
	stdoutput.printf("available blocks:  %lld\n",
					fs.getAvailableBlocks());
	stdoutput.printf("reserved blocks:   %lld\n",
					fs.getReservedBlocks());
	stdoutput.write('\n');


	// print file node info
	stdoutput.printf("total file nodes:      %lld\n",
					fs.getTotalFileNodes());
	stdoutput.printf("free file nodes:       %lld\n",
					fs.getFreeFileNodes());
	stdoutput.printf("available file nodes:  %lld\n",
					fs.getAvailableFileNodes());
	stdoutput.printf("reserved file nodes:   %lld\n",
					fs.getReservedFileNodes());
	stdoutput.write('\n');


	// print other properties
	stdoutput.printf("max filename length:   %lld\n",
					fs.getMaximumFileNameLength());
	stdoutput.printf("mount point:           %s\n",
					fs.getMountPoint());
	stdoutput.printf("device name:           %s\n",
					fs.getDeviceName());

	fs.close();
}
