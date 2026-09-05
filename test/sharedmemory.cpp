// Copyright (c) David Muse
// See the file COPYING for more information

#include <rudiments/sharedmemory.h>
#include <rudiments/charstring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/permissions.h>
#include <rudiments/file.h>
#include <rudiments/directory.h>
#include <rudiments/process.h>
#include <rudiments/stdio.h>
#include "test.cpp"

int main(int argc, const char **argv) {

        sharedmemory    shm;

	const char	*str="This string is in shared memory.";

	if (argc==1) {

		header("sharedmemory");

		if (!sharedmemory::isSupported()) {
			stdoutput.printf("	not supported\n\n");
			return 0;
		}

		stdoutput.write("setup...\n");

		// create the key file
		file::remove("shmkey");
		file	fd;
		test("key file",fd.create("shmkey",
				permissions::parsePermString("rw-------")));
		fd.close();

		// create a 128 byte shared memory segment
        	sharedmemory    shm;
        	test("create shm",
			shm.create(file::generateKey("shmkey",1),128,
                                permissions::parsePermString("rw-------")));

		// write a string into the shared memory
        	test("write",charstring::copy(
        			(char *)shm.getPointer(),
				"This string is in shared memory.")==
        			(char *)shm.getPointer());

		// spawn the second process (to write 2, 4)
		stringbuffer	cmd;
		char	*pwd=directory::getCurrentWorkingDirectory();
		cmd.append(pwd)->append("/sharedmemory");
		#ifdef _WIN32
			cmd.append(".exe");
		#endif
		delete[] pwd;
		const char	*args1[]={"sharedmemory","child",NULL};
		pid_t	childpid=process::spawn(cmd.getString(),args1,true);

		// Wait for the child rather than sleeping a fixed amount.
		// It needs the key file to still be there when it attaches,
		// and on a slow machine it can take longer than a second
		// just to start and link, so a sleep here is a race.
		int32_t	childstatus=0;
		bool	childok=process::wait(childpid,&childstatus) &&
							!childstatus;

		// clean up key file
		file::remove("shmkey");

		// the child's exit status is the only way its result gets
		// back to this process, and test() exits non-zero on failure
		test("child",childok);

	} else {

		// attach to the shared memory segment
        	test("attach to shm",
			shm.attach(file::generateKey("shmkey",1),128));

		// display the data contained in the shared memory segment
        	test("read",!charstring::compare((char *)shm.getPointer(),str));
        	stdoutput.printf("\n");
	}

	return 0;
}
